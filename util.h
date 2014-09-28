#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <sys/socket.h>

class Util
{
public:
	static std::string getIpAndPort(const struct sockaddr* addr);
	static bool setnonblocking(int fd);
};

#endif
