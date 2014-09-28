#include <iostream>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include "eventhandler.h"
#include "reactor.h"
#include "selectselector.h"
using namespace std;

class EchoEventHandler : public EventHandler
{
public:
	EchoEventHandler(Reactor& r) : EventHandler(r) {}
	EchoEventHandler(Reactor& r, int fd) : EventHandler(r, fd) {}

	virtual bool handleRead()
	{
		char buf[1024];
		int nbytes = read(handle, buf, sizeof(buf));
		if (nbytes < 0)
		{
			cerr << "Read from socket:" << handle << " error:" << strerror(errno) << endl;
		}
		else if (nbytes == 0)
		{
			reactor.removeEventHandler(handle);
			close(handle);
			cout << "Close socket:" << handle << endl;
		}

		write(handle, buf, nbytes);

		return true;
	}

	virtual void handleWrite()
	{
	}

	virtual void handleError()
	{
	}
};

/*
Reactor::RESULT_TYPE handler(int fd, Selector::EventType type)
{
	char buf[1024];

	if (type == Selector::READ)
	{
		int nbytes = read(fd, buf, sizeof(buf));
		if (nbytes < 0)
		{
			cerr << "Read from socket:" << fd << " error:" << strerror(errno) << endl;
		}
		else if (nbytes == 0)
		{
			return Reactor::RESULT_CLOSE;
		}

		write(fd, buf, nbytes);
	}
	else if (type == Selector::WRITE)
	{
		cout << "Socket:" << fd << " is writable" << endl;
	}
	else 
	{
		cout << "Socket:" << fd << " is error" << endl;
	}

	return Reactor::RESULT_CONTINUE;
}
*/

int main(int argc, char* argv[])
{
	Reactor reactor;

	std::shared_ptr<EchoEventHandler> pHandler(new EchoEventHandler(reactor));
	if (!reactor.init("9877", 10, new SelectSelector(), pHandler))
		exit(1);

	reactor.eventLoop();

	return 0;
}
