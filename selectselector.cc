#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fcntl.h>
#include "util.h"
#include "selectselector.h"
using namespace std;

SelectSelector::SelectSelector() : maxfd(-1)
{
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_ZERO(&errorfds);
}

void SelectSelector::addSocket(int fd, EventType type)
{
	Util::setnonblocking(fd);
	if (type & READ)
	{
		FD_SET(fd, &readfds);
		FD_SET(fd, &errorfds);
		if (fd > maxfd)
			maxfd = fd;
		std::cout << "Add socket:" << fd << " for read" << std::endl;
	}
	if (type & WRITE)
	{
		FD_SET(fd, &writefds);
		FD_SET(fd, &errorfds);
		if (fd > maxfd)
			maxfd = fd;
		std::cout << "Add socket:" << fd << " for write" << std::endl;
	}
}

void SelectSelector::removeSocket(int fd)
{
	FD_CLR(fd, &readfds);
	FD_CLR(fd, &writefds);
	FD_CLR(fd, &errorfds);
	std::cout << "Remove socket:" << fd << std::endl;
}

int SelectSelector::waitForEvent(int timeout)
{
	timeval *pTimeout = NULL;
	timeval selectTimeout;
	if (timeout >= 0)
	{
		selectTimeout.tv_sec = timeout / 1000;
		selectTimeout.tv_usec = (timeout % 1000) * 1000;
		pTimeout = &selectTimeout;
	}
	readfdscopy = readfds;
	writefdscopy = writefds;
	errorfdscopy = errorfds;
	std::cout << "waiting for event" << std::endl;
	int n = select(maxfd + 1, &readfdscopy, &writefdscopy, &errorfdscopy, pTimeout);
	if (n == -1)
	{
		std::cerr << "select error" << std::endl;
		return n;
	}

	int ret = n;
	resultfds.clear();
	for (int fd = 0; fd <= maxfd; ++fd)
	{
		if (FD_ISSET(fd, &readfdscopy))
			resultfds.push_back(std::make_pair(fd, READ));
		if (FD_ISSET(fd, &writefdscopy))
			resultfds.push_back(std::make_pair(fd, WRITE));
		if (FD_ISSET(fd, &errorfdscopy))
			resultfds.push_back(std::make_pair(fd, ERROR));
	}

	return ret;
}
