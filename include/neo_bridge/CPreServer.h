#ifndef _CPRESERVER_H_
#define _CPRESERVER_H_

#include "CShareMem.h"
#include "CThreadBase.h"
#include "CServerEpoll.h"
#include "CClientEpoll.h"
#include "CRosNode.h"
#include "config.h"

#ifdef USE_ROS
#include <ros/ros.h>
#endif

class CPrintThread:public CThreadBase
{
public:
	CPrintThread(CClientEpoll *client_epoll);
	void Run();
private:
	CClientEpoll *client_epoll;
};

class CRcvAckThread:public CThreadBase
{
public:
	CRcvAckThread(CClientEpoll *client_epoll);
	void Run();
private:
	CThreadPool *sendack_poll;
	CClientEpoll *client_epoll;
};


class CClientEpollThread:public CThreadBase
{
public:
	CClientEpollThread(CClientEpoll *client_epoll);
	void Run();
private:
	CClientEpoll *client_epoll;
};

class CSendPackTask:public CTask
{
public:
	CSendPackTask(char *buf, int Len, int fd, P_HEAD *ack_head,CClientEpoll *client_epoll);
	~CSendPackTask();
	void doAction();
private:
	char *buf;
	int Len;
	int fd;
	P_HEAD ack_head;
	CClientEpoll *client_epoll;
};

class CPreServer
{
public:
	CPreServer();
	~CPreServer();
        void Exec(int argc,char **argv);
	void Restart();
private:
	bool isRestart;
};

#endif
