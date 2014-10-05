#ifndef REACTOR_H
#define REACTOR_H

#include <memory>
#include <map>
#include "eventhandler.h"
#include "listener.h"
#include "selector.h"

class Reactor
{
public:
	typedef std::map<int, std::shared_ptr<EventHandler> > HandlerListType;

	Reactor();
	virtual ~Reactor();

	bool init(const std::string& service, int backLog, Selector* s);
	virtual void eventLoop() = 0;
	void addEventHandler(int fd, std::shared_ptr<EventHandler> pHandler);
	void removeEventHandler(int fd);

protected:
	std::shared_ptr<Selector> pSelector;
	Listener listener;
	HandlerListType handlerList;
};

#endif
