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

bool Reactor::init(const string& service, int backLog, Selector* s)
{
	if (!listener.init(service, backLog))
	{
		cerr << "Failed to init reactor" << endl;
		return false;
	}
	pSelector.reset(s);

	return true;
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
