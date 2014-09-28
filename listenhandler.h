#ifndef LISTENHANDLER_H
#define LISTENHANDLER_H

#include <memory>
#include "eventhandler.h"
#include "listener.h"

class ListenHandler : public EventHandler
{
public:
	ListenHandler(Listener& l, Reactor& r, std::shared_ptr<EventHandler> h);

	virtual bool handleRead();
	virtual void handleWrite();
	virtual void handleError();

protected:
	Listener& listener;
	std::shared_ptr<EventHandler> handler;
};

#endif
