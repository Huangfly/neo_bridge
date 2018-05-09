//
// Created by huang on 18-5-7.
//

#ifndef NEO_BRIDGE_THREADTIMERTASK_H
#define NEO_BRIDGE_THREADTIMERTASK_H

#include "CShareMem.h"
#include "CThreadBase.h"
#include "CClientEpoll.h"
#include "CServerEpoll.h"
#include <neo_bridge/CDebug.h>

class CPrintThread:public CThreadBase
{
public:
    CPrintThread(CClientEpoll *client_epoll);
    void Run();
private:
    CClientEpoll *client_epoll;
};


#endif //NEO_BRIDGE_THREADTIMERTASK_H
