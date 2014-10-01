#include <iostream>
#include "thread.h"
#include "lfreactor.h"
using namespace std;

LeaderFollowersReactor::LeaderFollowersReactor() : Reactor()
{
}

void LeaderFollowersReactor::eventLoop()
{
	AutoLock auLock(lockForEventLoop);

	cout << "Thread " << Thread::getThreadId() << " get lock" << endl;
	for (;;)
	{
		int ret = pSelector->waitForEvent(-1);
		if (ret < 0)
			break;
		const Selector::ResultListType& resultList = pSelector->getResultList();
		Selector::ResultListType::const_iterator it(resultList.begin()), itEnd(resultList.end());
		for (; it != itEnd; ++it)
		{
			int fd = it->first;
			Selector::EventType type = it->second;
			HandlerListType::iterator itFind = handlerList.find(fd);
			if (itFind != handlerList.end())
			{
				std::shared_ptr<EventHandler> handler = itFind->second;
				if (type & Selector::READ)
					handler->handleRead();
				else if (type & Selector::WRITE)
					handler->handleWrite();
				else
					handler->handleError();

			}
			else
			{
				cerr << "Can't find socket:" << fd << " event handler" << endl;
			}
		}
	}
	cout << "Thread " << Thread::getThreadId() << " release lock" << endl;
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
