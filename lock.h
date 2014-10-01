#include <pthread.h>

class Lock
{
public:
	Lock();
	~Lock();

	void lock();
	void unlock();
	bool trylock();
	
private:
	pthread_mutex_t mutex;
};

class AutoLock
{
public:
	AutoLock(Lock& l);
	~AutoLock();

private:
	Lock& lock;
};
