#include "CClientEpoll.h"
#include "CUnpackTask.h"
#include "CServerEpoll.h"
#include "CUnDataPackTask.h"
//extern map<int,int> user_list;
//extern map<int,int> connect_list;
extern int rcv_Count;
extern int connect_Count;
extern int user_Count;

//============================================================================
//
//  CClientEpoll
//
//============================================================================
CClientEpoll::CClientEpoll(int size /* = 1000 */)
:CEpollBase(size)
{
	m_nConnect = 0;
	this->Create();
	m_BusPool = new CThreadPool(10);
}

CClientEpoll::~CClientEpoll()
{
	delete m_BusPool;
}

void CClientEpoll::onEvent()
{
	int nRead;
	int i = 0;
	char acBuf[1024] = {0};

	for (i = 0; i<m_result; i++)
	{
		nRead = read(m_retEvents[i].data.fd, acBuf, 1024);
		ModEvent(m_retEvents[i].data.fd,EPOLLIN | EPOLLET);
		if (nRead <= 0)
		{
			onQuit(acBuf, m_retEvents[i].data.fd, nRead);
		}
		else
		{
			onData(acBuf, m_retEvents[i].data.fd, nRead);
		}
	}
}

void CClientEpoll::onQuit(char *buf, int fd, int nRead)
{
	if (CServerEpoll::fds.erase(fd))
	{
		if(!DelEvent(fd, EPOLLOUT)){
			printf("DelEvent fail.\n");
			exit(1);
		}
		close(fd);
	}
	else
	{
		printf("error onQuie.%d  %d\n",fd,nRead);
        exit(1);
	}

}

void CClientEpoll::onData(char *buf, int fd, int nRead)
{
	int Len = 0;
	int pos = 0;
	int function_id = 0;

	if(CServerEpoll::fds.findKey(fd)){
		CServerEpoll::fds.set(fd,5);
	}

	m_BusPool->addTask(new CUnDataPackTask(buf, nRead, fd, m_BusPool));
}

int* CClientEpoll::GetnConnect()
{	
	//return &m_nConnect;
	return 0;
}
