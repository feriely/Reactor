#ifndef LFREACTOR_H
#define LFREACTOR_H

#include <vector>
#include <memory>
#include "reactor.h"
#include "lock.h"

class ReactorRunner;
class LeaderFollowersReactor : public Reactor
{
public:
	typedef std::vector<std::shared_ptr<ReactorRunner> > ReactorRunnerListType;
	LeaderFollowersReactor();

	virtual void eventLoop();
	void createThread();

private:
	Lock lockForEventLoop;
	ReactorRunnerListType reactorRunners;
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
