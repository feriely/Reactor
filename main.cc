#include <iostream>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include "eventhandler.h"
#include "lfreactor.h"
#include "selectselector.h"
using namespace std;

class EchoEventHandler : public EventHandler
{
public:
	EchoEventHandler(Reactor& r) : EventHandler(r) {}
	EchoEventHandler(Reactor& r, int fd) : EventHandler(r, fd) {}

	virtual HANDLE_RESULT handleRead()
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
			return HANDLE_RESULT_CLOSE;
		}

		write(handle, buf, nbytes);

		return HANDLE_RESULT_CONTINUE;
	}

	virtual HANDLE_RESULT handleWrite()
	{
		return HANDLE_RESULT_CONTINUE;
	}

	virtual HANDLE_RESULT handleError()
	{
		return HANDLE_RESULT_CONTINUE;
	}
};

int main(int argc, char* argv[])
{
	LeaderFollowersReactor reactor;

	std::shared_ptr<EchoEventHandler> pHandler(new EchoEventHandler(reactor));
	if (!reactor.initWithEventHandler<EchoEventHandler>("9877", 10, new SelectSelector()))
		exit(1);

	reactor.createThread();
	reactor.createThread();

	pause();

	return 0;
}
