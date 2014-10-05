#ifndef LISTENHANDLER_H
#define LISTENHANDLER_H

#include <memory>
#include "eventhandler.h"
#include "listener.h"
#include "reactor.h"

template <typename EventHandlerType>
class ListenHandler : public EventHandler
{
public:
	ListenHandler(Reactor& r, Listener& l);

	virtual HANDLE_RESULT handleRead();
	virtual HANDLE_RESULT handleWrite();
	virtual HANDLE_RESULT handleError();

protected:
	Listener& listener;
	std::shared_ptr<EventHandler> handler;
};

template <typename EventHandlerType>
ListenHandler<EventHandlerType>::ListenHandler(Reactor& r, Listener& l) : EventHandler(r, l.getSocket()), listener(l)
{
}

template <typename EventHandlerType>
EventHandler::HANDLE_RESULT ListenHandler<EventHandlerType>::handleRead()
{
	int connfd = listener.acceptConnection();
	if (connfd < 0)
		return HANDLE_RESULT_ERROR;

	std::shared_ptr<EventHandler> pEventHandler(new EventHandlerType(reactor, connfd));
	reactor.addEventHandler(connfd, pEventHandler);

	return HANDLE_RESULT_CONTINUE;
}

template <typename EventHandlerType>
EventHandler::HANDLE_RESULT ListenHandler<EventHandlerType>::handleWrite()
{
	return HANDLE_RESULT_CONTINUE;
}

template <typename EventHandlerType>
EventHandler::HANDLE_RESULT ListenHandler<EventHandlerType>::handleError()
{
	return HANDLE_RESULT_CONTINUE;
}

#endif
