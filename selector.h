#ifndef SELECTOR_H
#define SELECTOR_H

#include <vector>
#include <utility>

class Selector
{
public:
	enum EventType
	{
		READ = 0x01,
		WRITE = 0x02,
		ERROR = 0x04,
	};

	typedef std::pair<int, EventType> ResultType;
	typedef std::vector<ResultType> ResultListType;

	virtual void addSocket(int fd, EventType type) = 0;
	virtual void removeSocket(int fd) = 0;
	virtual int waitForEvent(int timeout) = 0;

	const ResultListType& getResultList() const { return resultfds; }

protected:
	ResultListType resultfds;
};

#endif
