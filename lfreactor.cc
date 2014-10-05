#include <iostream>
#include "thread.h"
#include "lfreactor.h"
using namespace std;

LeaderFollowersReactor::LeaderFollowersReactor() : Reactor()
{
}

void LeaderFollowersReactor::eventLoop()
{
	AutoLock auEventLoopLock(lockForEventLoop);
	cout << "Thread " << Thread::getThreadId() << " get event loop lock" << endl;
	AutoLock auSelectorLock(lockForSelector);
	cout << "Thread " << Thread::getThreadId() << " get selector lock" << endl;

	for (;;)
	{
		int ret = pSelector->waitForEvent(-1);
		if (ret < 0)
			break;
		const Selector::ResultListType& resultList = pSelector->getResultList();
		Selector::ResultListType::const_iterator it(resultList.begin()), itEnd(resultList.end());
		bool dispatch = false;
		for (; it != itEnd; ++it)
		{
			int fd = it->first;
			Selector::EventType type = it->second;
			if (fd == notifyPipe.getSocket() || fd == listener.getSocket())
			{
				if (type & Selector::READ)
					handlerList[fd]->handleRead();
				else
					cerr << "Invalid event type:" << type << " for fd:" << fd << endl;
			}
			else
			{
				pSelector->removeSocket(fd);
				dispatch = true;
			}
		}
		if (dispatch)
		{
			auSelectorLock.release();
			cout << "Thread " << Thread::getThreadId() << " release selector lock" << endl;
			auEventLoopLock.release();
			cout << "Thread " << Thread::getThreadId() << " release event loop lock" << endl;
			for (it = resultList.begin(); it != itEnd; ++it)
			{
				int fd = it->first;
				Selector::EventType type = it->second;
				if (fd == notifyPipe.getSocket() || fd == listener.getSocket())
					continue;

				EventHandler::HANDLE_RESULT result;
				if (type & Selector::READ)
					result = handlerList[fd]->handleRead();
				else if (type & Selector::WRITE)
					result = handlerList[fd]->handleWrite();
				else
					result = handlerList[fd]->handleError();

				if (result == EventHandler::HANDLE_RESULT_CONTINUE)
				{
					AutoLock auLock(lockForSelector);
					cout << "Thread " << Thread::getThreadId() << " get selector lock" << endl;
					pSelector->addSocket(fd, Selector::READ);
					notifyPipe.send();
					cout << "Thread " << Thread::getThreadId() << " release selector lock" << endl;
				}
			}
		}
	}
	cout << "Thread " << Thread::getThreadId() << " release selector lock" << endl;
	cout << "Thread " << Thread::getThreadId() << " release event loop lock" << endl;
}

void LeaderFollowersReactor::createThread()
{
	std::shared_ptr<ReactorRunner> pReactorRunner(new ReactorRunner(*this));
	reactorRunners.push_back(pReactorRunner);
	Thread::createThread(*pReactorRunner);
}

ReactorRunner::ReactorRunner(Reactor& r) : reactor(r)
{
}

void ReactorRunner::operator()()
{
	reactor.eventLoop();
}
