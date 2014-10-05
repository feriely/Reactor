#include <iostream>
#include <cstring>
#include <errno.h>
#include "lock.h"
using namespace std;

Lock::Lock()
{
	int ret = pthread_mutex_init(&mutex, NULL);
	if (ret != 0)
		cerr << "Failed to init mutex:" << strerror(errno) << endl;
}

Lock::~Lock()
{
	int ret = pthread_mutex_destroy(&mutex);
	if (ret != 0)
		cerr << "Failed to destroy mutex:" << strerror(errno) << endl;
}

void Lock::lock()
{
	int ret = pthread_mutex_lock(&mutex);
	if (ret != 0)
	{
		cerr << "Failed to lock:" << strerror(errno) << endl;
	}
}

void Lock::unlock()
{
	int ret = pthread_mutex_unlock(&mutex);
	if (ret != 0)
	{
		cerr << "Failed to unlock:" << strerror(errno) << endl;
	}
}

bool Lock::trylock()
{
	int ret = pthread_mutex_trylock(&mutex);
	if (ret != 0)
	{
		if (errno == EBUSY)
		{
			cout << "Lock is busy" << endl;
		}
		else 
		{
			cerr << "Failed to trylock:" << strerror(errno) << endl;
		}
		return false;
	}
	return true;
}

AutoLock::AutoLock(Lock& l) : lock(l)
{
	lock.lock();
}

AutoLock::~AutoLock()
{
	release();
}

void AutoLock::release()
{
	lock.unlock();
}
