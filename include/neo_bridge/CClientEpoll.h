#ifndef _CCLIENTEPOLL_H_
#define _CCLIENTEPOLL_H_

#include "CEpollBase.h"
#include "CTcpServer.h"
#include "CThreadPool.h"
#include "CShareMem.h"
#include "packet.h"
#include <map>

class CClientEpoll:public CEpollBase
{
public:
	CClientEpoll(int size = 1000);
	~CClientEpoll();
	void onEvent();
	virtual void onQuit(char *buf, int fd, int nRead);
	virtual void onData(char *buf, int fd, int nRead);
	int* GetnConnect();
protected:
	int m_nConnect;//ǰӣĸ
	CThreadPool *m_BusPool;
};

class CBusTask:public CTask
{
public:
	CBusTask(char *buf, int fd, int Len);
	~CBusTask();
	void doAction();
private:
	char *buf;
	int fd;
	int Len;
};

class CUnDataPackTask:public CTask
{
public:
	CUnDataPackTask(char *buf, int fd, int Len, CThreadPool *m_BusPool);
	~CUnDataPackTask();
	void doAction();
private:
	char *buf;
	int fd;
	int nRead;
	CThreadPool *m_BusPool;
};

#endif
