#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

class Reactor;

class EventHandler
{
public:
	EventHandler(Reactor& r);
	EventHandler(Reactor& r, int fd);
	virtual ~EventHandler();

	virtual bool handleRead() = 0;
	virtual void handleWrite() = 0;
	virtual void handleError() = 0;
	//virtual void handleClose() = 0;
	
	void setHandle(int fd) { handle = fd; }
	
protected:
	Reactor& reactor;
	int handle;
};

#endif
