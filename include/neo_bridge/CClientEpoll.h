#ifndef _CCLIENTEPOLL_H_
#define _CCLIENTEPOLL_H_

#include "CEpollBase.h"
#include "CTcpServer.h"
#include "CThreadPool.h"
#include "CShareMem.h"
#include "packet.h"
#include <neoslam_sdk/Type_MutexMap.hpp>


#define CLIENTEPOLL_TIMEOUT 3

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
	int m_nConnect;//

	CThreadPool *m_BusPool;
};



#endif
