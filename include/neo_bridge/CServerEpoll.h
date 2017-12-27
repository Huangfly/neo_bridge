#ifndef _CSERVEREPOLL_H_
#define _CSERVEREPOLL_H_

#include "CEpollBase.h"
#include "CTcpServer.h"
#include "CTcpClient.h"
#include "CThreadPool.h"
#include "CClientEpoll.h"
#include <map>

class CServerEpoll:public CEpollBase
{
public:
	CServerEpoll(CHostAddr *addr, int nListen, CClientEpoll *client_epoll);
	~CServerEpoll();
	bool Connect();
	void onEvent();
	virtual void onConnect(char *buf, int fd, int nRead);
protected:
	int nlisten;
	CHostAddr *addr;
	CTcpServer server;
protected:
	CClientEpoll *client_epoll;
};


#endif
