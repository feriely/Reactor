#include <iostream>
#include <string>
#include "reactor.h"
#include "listenhandler.h"
using namespace std;

Reactor::Reactor() 
{
}

Reactor::~Reactor()
{
}

bool Reactor::init(const string& service, int backLog, Selector* s, std::shared_ptr<EventHandler> handler)
{
	if (!listener.init(service, backLog))
	{
		cerr << "Failed to init reactor" << endl;
		return false;
	}
	pSelector.reset(s);
	std::shared_ptr<EventHandler> pHandler(new ListenHandler(listener, *this, handler));
	addEventHandler(listener.getSocket(), pHandler);

	return true;
}

void Reactor::eventLoop()
{
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
}

void Reactor::addEventHandler(int fd, std::shared_ptr<EventHandler> pHandler)
{
	handlerList.insert(std::make_pair(fd, pHandler));
	//pSelector->addSocket(fd, static_cast<Selector::EventType>(Selector::READ | Selector::WRITE));
	pSelector->addSocket(fd, Selector::READ);
}

void Reactor::removeEventHandler(int fd)
{
	handlerList.erase(fd);
	pSelector->removeSocket(fd);
}
