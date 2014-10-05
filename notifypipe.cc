#include <iostream>
#include <cstring>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include "notifypipe.h"
using namespace std;

NotifyHandler::NotifyHandler(Reactor& r, NotifyPipe& pipe) : EventHandler(r, pipe.getSocket()), notifyPipe(pipe)
{
}

EventHandler::HANDLE_RESULT NotifyHandler::handleRead()
{
	notifyPipe.recv();
	return HANDLE_RESULT_CONTINUE;
}

EventHandler::HANDLE_RESULT NotifyHandler::handleWrite()
{
	return HANDLE_RESULT_CONTINUE;
}

EventHandler::HANDLE_RESULT NotifyHandler::handleError()
{
	return HANDLE_RESULT_CONTINUE;
}

NotifyPipe::NotifyPipe()
{
	int ret = pipe2(pipefd, O_NONBLOCK);
	if (ret < 0)
		cerr << "Create pipe error:" << strerror(errno) << endl;
}

void NotifyPipe::send()
{
	int ret = write(pipefd[1], "", 1);
	if (ret != 1)
		cerr << "Send notify to pipe:" << pipefd[1] << " error:" << strerror(errno) << endl;
}

void NotifyPipe::recv()
{
	char buf[1];
	int ret = read(pipefd[0], buf, 1);
	if (ret != 1)
		cerr << "Receive notify from pipe:" << pipefd[0] << " error:" << strerror(errno) << endl;
}

int NotifyPipe::getSocket()
{
	return pipefd[0];
}
