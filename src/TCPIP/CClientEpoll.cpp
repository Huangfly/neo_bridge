#include "CClientEpoll.h"
#include "CUnpackTask.h"

extern map<int,int> user_list;
extern map<int,int> connect_list;
extern CShareMem shm_bus;
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

//ͻ˵ļͻ˶ϿӺĴ
void CClientEpoll::onQuit(char *buf, int fd, int nRead)
{
	//printf("onQuie.\n");
	if (user_list.find(fd) != user_list.end())
	{
		user_list.erase(fd);
		user_Count--;
	}
	else if (connect_list.find(fd) != connect_list.end())
	{
		connect_list.erase(fd);
	}
	else
	{
		return;
	}
	DelEvent(fd, EPOLLOUT);
	close(fd);
	connect_Count--;
}

//ͻ˵ļȡİڴ˴
void CClientEpoll::onData(char *buf, int fd, int nRead)
{
	int Len = 0;
	int pos = 0;
	int funcId = 0;

	//൱ڼʼmapֵ
	map<int,int>::iterator it;
	it = user_list.find(fd);
	if (it != user_list.end())
	{
		it->second = 5;
	}

	it = connect_list.find(fd);
	if (it != connect_list.end())
	{
		it->second = 10;
	}
	m_BusPool->addTask(new CUnDataPackTask(buf, nRead, fd, m_BusPool));
}

int* CClientEpoll::GetnConnect()
{	
	return &m_nConnect;
}

//============================================================================
//
//  CBusTask
//
//============================================================================
CBusTask::CBusTask(char *buf, int fd, int Len)
:CTask()
{
    this->buf = new char[Len];
    memcpy(this->buf, buf, Len);
    this->Len = Len;
	this->fd = fd;
}

CBusTask::~CBusTask()
{
	delete []buf;
}

void CBusTask::doAction()
{
	shm_bus.Write(buf, Len, fd);
}

//============================================================================
//
//  CUnDataPackTask
//
//============================================================================
CUnDataPackTask::CUnDataPackTask(char *bus_buf, int Len, int fd, CThreadPool *busctl_pool)
:CTask()
{
	this->m_BusPool = busctl_pool;
	this->buf = new char[Len];
	memcpy(this->buf, bus_buf, Len);
	this->nRead = Len;
	this->fd = fd;
}

CUnDataPackTask::~CUnDataPackTask()
{
	delete []buf;
}

void CUnDataPackTask::doAction()
{
	P_HEAD head = {0};
	char head_str[2] = {HEAD_CHR, HEAD_CHR};
	char end_str[2] = {END_CHR, END_CHR};
	
	for (int i = 0; i < nRead; i++)
	{
        if(memcmp(buf + i, head_str, HEAD_LEN) == 0)
		{
			//ȡheadȡsize
            memcpy(&head, buf + i + HEAD_LEN, sizeof(P_HEAD));
			//printf("%d %d %d %d\n",head.funcId,head.size,head.msg_code,head.trans_id);
            //DEBUG_LOG((unsigned char*)(buf+i),20);
            if (head.size > nRead - i)
			{
				continue;
			}
            //DEBUG_LOG((unsigned char*)(buf+i),25);
			//ƫsizeжǷаβ
            if (memcmp(buf + i + head.size + HEAD_LEN + 1, end_str, HEAD_LEN) == 0)//ȡİ
			{
				//printf("receive package.\n");
				//m_BusPool->addTask(new CUnpackTask(buf, head.size, fd, m_BusPool));
				m_BusPool->addTask(new CBusTask(buf + i + HEAD_LEN, fd, head.size));
				i += (head.size - 1);

				rcv_Count++;				
				if (*(buf + i + 1) != HEAD_CHR)
				{
					return;
				}
				continue;
			}
			else//
			{
				continue;
			}
		}
	}
}
