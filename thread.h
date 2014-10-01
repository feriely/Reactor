#ifndef THREAD_H
#define THREAD_H

#include <iostream>
#include <pthread.h>
#include <string.h>
#include <errno.h>

template <typename ArgType>
class ThreadWithArgType;

class Thread
{
public:
	template <typename ArgType>
		static bool createThread(ArgType& arg)
		{
			return ThreadWithArgType<ArgType>::createThread(arg);
		}

	static pthread_t getThreadId() { return pthread_self(); }
};

template <typename ArgType>
class ThreadWithArgType
{
public:
	static bool createThread(ArgType& arg)
	{
		pthread_t tid;
		int ret = pthread_create(&tid, NULL, &threadFunction, &arg);
		if (ret != 0)
		{
			std::cerr << "Create thread error:" << strerror(errno) << std::endl;
			return false;
		}
		return true;
	}

private:
	static void *threadFunction(void *arg)
	{
		ArgType *pArg = reinterpret_cast<ArgType *>(arg);
		(*pArg)();

		return NULL;
	}
};

#endif
