#include "CServerEpoll.h"


extern int connect_Count;
extern map<int,int> user_list;
extern map<int,int> connect_list;
Neo::MutexMap<int,int> CServerEpoll::_connect_fds_count(1024);


CServerEpoll::CServerEpoll(CHostAddr *addr, int nListen, CClientEpoll *client_epoll)
:CEpollBase(1)
{
	this->client_epoll = client_epoll;	
	this->addr = addr;
	this->nlisten = nListen;
}

CServerEpoll::~CServerEpoll()
{
	server.Close();
}

bool CServerEpoll::Connect()
{
	bool ret;
	ret = server.Create();
	if (ret == false)
	{
		return false;
	}
	ret = server.Bind(*addr);
	if (ret == false)
	{
		return false;
	}
	server.Listen(nlisten);
	this->Create();
	this->AddEvent(server.getFd(), EPOLLIN | EPOLLET);
	return true;
}

void CServerEpoll::onEvent()
{
	int nRead;
	char acBuf[1024] = {0};

	onConnect(acBuf, m_retEvents[0].data.fd, nRead);
}

void CServerEpoll::onConnect(char *buf, int fd, int nRead)
{
	int newfd;
	//printf("OnConnect.%d\n",fd);
	newfd = accept(server.getFd(), NULL, NULL);
    //printf("OnConnect1.%d\n",newfd);
    _connect_fds_count.inset(std::pair<int,int>(newfd, 10));
    //printf("OnConnect2.%d\n",newfd);
	client_epoll->AddEvent(newfd, EPOLLIN | EPOLLET);
}

