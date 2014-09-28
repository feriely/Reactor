#include <cstring>
#include <iostream>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "util.h"
#include "listener.h"
using namespace std;

Listener::Listener() : listenfd(-1)
{
}

bool Listener::init(const string& service, int backLog)
{
	int ret;
	struct addrinfo hints, *res, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;

	if ((ret = getaddrinfo(NULL, service.c_str(), &hints, &res)) != 0)
	{
		cerr << "getaddrinfo error:" << gai_strerror(ret) << endl;
		return false;
	}

	int yes = 1;
	for (p = res; p != NULL; p = p->ai_next)
	{
		if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
			continue;
		if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
			return false;
		if (bind(listenfd, p->ai_addr, p->ai_addrlen) < 0)
			continue;
		break;
	}

	if (p == NULL)
	{
		cerr << "Failed to bind service:" << service << endl;
		return false;
	}

	freeaddrinfo(res);

	if (listen(listenfd, backLog) < 0)
	{
		cerr << "Listen at fd:" << listenfd << " error:" << strerror(errno) << endl;
		return false;
	}

	return true;
}

int Listener::acceptConnection()
{
	int ret;
	struct sockaddr_storage client;
	socklen_t clilen;
	
	clilen = sizeof(client);
	ret = accept(listenfd, (struct sockaddr *)&client, &clilen);
	if (ret < 0)
	{
		cerr << "Accept from fd:" << listenfd << " error:" << strerror(errno) << endl;
	}
	else
	{
		cout << "Accept " << Util::getIpAndPort((struct sockaddr *)&client) << endl;
	}

	return ret;
}
