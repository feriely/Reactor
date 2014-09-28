#include "eventhandler.h"

EventHandler::EventHandler(Reactor& r) : reactor(r), handle(-1)
{
}

EventHandler::EventHandler(Reactor& r, int fd) : reactor(r), handle(fd)
{
}

EventHandler::~EventHandler()
{
}
