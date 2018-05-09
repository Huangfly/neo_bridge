//
// Created by huang on 18-5-8.
//

#ifndef NEO_BRIDGE_THREADCLIENTEPOLL_H
#define NEO_BRIDGE_THREADCLIENTEPOLL_H

#include "CShareMem.h"
#include "CThreadBase.h"
#include "CClientEpoll.h"

class CClientEpollThread:public CThreadBase
{
public:
    CClientEpollThread(CClientEpoll *client_epoll);
    void Run();
private:
    CClientEpoll *client_epoll;
};

#endif //NEO_BRIDGE_THREADCLIENTEPOLL_H
