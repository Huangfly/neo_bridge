//
// Created by huang on 18-5-7.
//

#ifndef NEO_BRIDGE_THREADRECEIVEACK_H
#define NEO_BRIDGE_THREADRECEIVEACK_H

#include "CShareMem.h"
#include "CThreadBase.h"
#include "CClientEpoll.h"

class CSendPackTask:public CTask
{
public:
    CSendPackTask(char *buf, int Len, int fd, Neo_Packet::HEAD *ack_head,CClientEpoll *client_epoll);
    ~CSendPackTask();
    void doAction();
private:
    char *buf;
    int Len;
    int fd;
    Neo_Packet::HEAD ack_head;
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


#endif //NEO_BRIDGE_THREADRECEIVEACK_H
