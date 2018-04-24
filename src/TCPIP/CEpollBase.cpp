#include "CEpollBase.h"
#include "CUnpackTask.h"

CEpollBase::CEpollBase(unsigned int maxEvent /* = 1000 */)
{
	m_retEvents = new struct epoll_event[maxEvent];
	m_maxEvent = maxEvent;
	m_result = 0;
	m_epollfd = -1;
}

CEpollBase::~CEpollBase()
{
	delete []m_retEvents;
}

bool CEpollBase::Create()
{
	if (m_epollfd != -1)//epollѾ
	{
		perror("m_epollfd existed.");
		exit(1);
	}
	m_epollfd = epoll_create(m_maxEvent);
	if (m_epollfd < 0)
	{
		perror("epoll_create fail");
		exit(1);
	}
	return true;
}

bool CEpollBase::AddEvent(int fd, __uint32_t events)
{
	struct epoll_event event = {0};
	int ctl;
	event.events = events;
	event.data.fd = fd;
	ctl = epoll_ctl(m_epollfd, EPOLL_CTL_ADD, fd, &event);
	//printf("add %d\n",fd);
	if (ctl == -1)
	{
		perror("AddEvent fail.");
	}

	return (ctl == -1) ? false : true;
}

bool CEpollBase::DelEvent(int fd, __uint32_t events)
{
	struct epoll_event event = {0};
	int ctl;
	event.events = events;
	event.data.fd = fd;
	ctl = epoll_ctl(m_epollfd, EPOLL_CTL_DEL, fd, &event);
	if (ctl == -1)
	{
		perror("DelEvent fail.");
	}	
	return (ctl == -1) ? false : true;
}

bool CEpollBase::ModEvent(int fd, __uint32_t events)
{
	struct epoll_event event = {0};
	int ctl;
	event.events = events;
	event.data.fd = fd;
	ctl = epoll_ctl(m_epollfd, EPOLL_CTL_MOD, fd, &event);
	if (ctl == -1)
	{
		perror("ModEvent fail.");
	}
	return (ctl == -1) ? false : true;
}

bool CEpollBase::Wait(int timeout)
{
	m_result = epoll_wait(m_epollfd, m_retEvents, m_maxEvent, timeout);
	if (m_result == -1)
	{
		perror("Epoll wait fail.");
	}
	else if (m_result == 0)
	{
        //printf("Epoll Time out.\n");
	}
	else
	{
		//printf("onEvent %d.\n",m_epollfd);

		onEvent();
	}
	return (m_result == -1) ? false : true;
}

void CEpollBase::Start(int timeout)
{
	m_bStop = true;
	while (m_bStop)
	{
		Wait(timeout);
	}
}

void CEpollBase::Stop()
{
	m_bStop = false;
}

void CEpollBase::onEvent()
{
    printf("On Base Event...\n");
}

int CEpollBase::GetResults()
{
	return m_result;
}
