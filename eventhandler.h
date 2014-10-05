#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

class Reactor;

class EventHandler
{
public:
	enum HANDLE_RESULT
	{
		HANDLE_RESULT_CONTINUE,
		HANDLE_RESULT_CLOSE,
		HANDLE_RESULT_ERROR,
	};

	EventHandler(Reactor& r);
	EventHandler(Reactor& r, int fd);
	virtual ~EventHandler();

	virtual HANDLE_RESULT handleRead() = 0;
	virtual HANDLE_RESULT handleWrite() = 0;
	virtual HANDLE_RESULT handleError() = 0;
	
	void setHandle(int fd) { handle = fd; }
	
protected:
	Reactor& reactor;
	int handle;
};

#endif
