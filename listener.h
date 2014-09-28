#ifndef LISTENER_H
#define LISTENER_H

#include <string>

class Listener
{
public:
	Listener();

	bool init(const std::string& service, int backLog);
	int acceptConnection();
	int getSocket() const { return listenfd; }

private:
	int listenfd;
};

#endif
