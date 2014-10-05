#ifndef NOTIFYPIPE_H
#define NOTIFYPIPE_H

#include <memory>
#include "eventhandler.h"

class NotifyPipe;
class NotifyHandler : public EventHandler
{
public:
	NotifyHandler(Reactor& r, NotifyPipe& pipe);

	virtual HANDLE_RESULT handleRead();
	virtual HANDLE_RESULT handleWrite();
	virtual HANDLE_RESULT handleError();

private:
	NotifyPipe& notifyPipe;
};

class NotifyPipe
{
public:
	NotifyPipe();

	void send();
	void recv();
	int getSocket();

private:
	int pipefd[2];
	std::shared_ptr<NotifyHandler> pNotifyHandler;
};

#endif
