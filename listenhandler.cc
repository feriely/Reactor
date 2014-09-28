#include "listenhandler.h"
#include "reactor.h"

ListenHandler::ListenHandler(Listener& l, Reactor& r, std::shared_ptr<EventHandler> h) : EventHandler(r, l.getSocket()), listener(l), handler(h)
{
}

bool ListenHandler::handleRead()
{
	int connfd = listener.acceptConnection();
	if (connfd < 0)
		return false;

	handler->setHandle(connfd);
	reactor.addEventHandler(connfd, handler);

	return true;
}

void ListenHandler::handleWrite()
{
}

void ListenHandler::handleError()
{
}
