#ifndef _CUNPACKTASK_H_
#define _CUNPACKTASK_H_

#include "CShareMem.h"
#include "CThreadPool.h"
#include "packet.h"
//////////////////////////////

class CUnpackTask:public CTask
{
public:
	CUnpackTask(char *bus_buf, int Len, int fd, CThreadPool *busctl_pool);
	~CUnpackTask();
	void doAction();
private:
	char *buf;
	int Len;
	int fd;
	CThreadPool *busctl_pool;
};

#endif
