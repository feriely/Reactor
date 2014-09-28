#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include "util.h"
using namespace std;

string Util::getIpAndPort(const struct sockaddr* addr)
{
	string result;
	if (addr->sa_family == AF_INET)
	{
		const struct sockaddr_in* sin = reinterpret_cast<const struct sockaddr_in*>(addr);
		char buf[INET6_ADDRSTRLEN];

		memset(buf, 0, sizeof(buf));
		if (inet_ntop(sin->sin_family, &sin->sin_addr, buf, sizeof(buf)) == NULL)
		{
			cerr << "inet_ntop error:" << strerror(errno) << endl;
			return "";
		}
		result = buf;
		stringstream stream;
		stream << sin->sin_port;
		result += ":" + stream.str();
	}

	return result;
}

bool Util::setnonblocking(int fd)
{
	int flags;

	if ((flags = fcntl(fd, F_GETFL)) < 0)
	{
		cerr << "fcntl get flag error" << endl;
		return false;
	}
	flags |= O_NONBLOCK;

	if (fcntl(fd, F_SETFL, flags) < 0)
	{
		cerr << "fcntl set flag error" << endl;
		return false;
	}
	return true;
}
