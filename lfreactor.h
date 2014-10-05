#ifndef LFREACTOR_H
#define LFREACTOR_H

#include <vector>
#include <memory>
#include "reactor.h"
#include "lock.h"
#include "notifypipe.h"
#include "listenhandler.h"

class ReactorRunner;
class LeaderFollowersReactor : public Reactor
{
public:
	typedef std::vector<std::shared_ptr<ReactorRunner> > ReactorRunnerListType;

	LeaderFollowersReactor();

	template <typename EventHandlerType>
		bool initWithEventHandler(const std::string& service, int backLog, Selector* s)
		{
			if (!init(service, backLog, s))
				return false;

			std::shared_ptr<EventHandler> pListenHandler(new ListenHandler<EventHandlerType>(*this, listener));
			addEventHandler(listener.getSocket(), pListenHandler);

			std::shared_ptr<EventHandler> pNotifyHandler(new NotifyHandler(*this, notifyPipe));
			addEventHandler(notifyPipe.getSocket(), pNotifyHandler);

			return true;
		}

	virtual void eventLoop();
	void createThread();

private:
	ReactorRunnerListType reactorRunners;
	Lock lockForEventLoop;
	Lock lockForSelector;
	NotifyPipe notifyPipe;
};

class ReactorRunner
{
public:
	ReactorRunner(Reactor& r);

	void operator()();

private:
	Reactor& reactor;
};

#endif
