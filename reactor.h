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
	enum RESULT_TYPE
	{
		RESULT_CLOSE,
		RESULT_CONTINUE,
	};
	typedef std::map<int, std::shared_ptr<EventHandler> > HandlerListType;

	Reactor();
	virtual ~Reactor();
	
	bool init(const std::string& service, int backLog, Selector* s, std::shared_ptr<EventHandler> h);
	void eventLoop();
	void addEventHandler(int fd, std::shared_ptr<EventHandler> pHandler);
	void removeEventHandler(int fd);

protected:
	std::shared_ptr<Selector> pSelector;
	Listener listener;
	HandlerListType handlerList;
};

#endif