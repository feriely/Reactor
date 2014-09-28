#ifndef SELECTSELECTOR_H
#define SELECTSELECTOR_H

#include <sys/select.h>
#include "selector.h"

class SelectSelector : public Selector
{
public:
	SelectSelector();

	virtual void addSocket(int fd, EventType type);
	virtual void removeSocket(int fd);
	virtual int waitForEvent(int timeout);

private:
	int maxfd;
	fd_set readfds;
	fd_set writefds;
	fd_set errorfds;
	fd_set readfdscopy;
	fd_set writefdscopy;
	fd_set errorfdscopy;

	void setnonblocking(int fd);
};

#endif
