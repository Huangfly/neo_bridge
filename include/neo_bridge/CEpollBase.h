#ifndef _CEPOLLBASE_H_
#define _CEPOLLBASE_H_

#include <sys/epoll.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

class CEpollBase
{
public:
	CEpollBase(unsigned int maxEvent = 1000);
	virtual ~CEpollBase();
	bool AddEvent(int fd, __uint32_t events);
	bool ModEvent(int fd, __uint32_t events);
	bool DelEvent(int fd, __uint32_t events);
	bool Create();
	bool Wait(int timeout);
	void Start(int timeout);
	int GetResults();
	void Stop();
	virtual void onEvent() = 0;
protected:
	struct epoll_event *m_retEvents;
	int m_maxEvent;
	int m_result;
	int m_epollfd;
	bool m_bStop;
};

#endif