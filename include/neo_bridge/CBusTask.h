//
// Created by huang on 18-5-7.
//

#ifndef NEO_BRIDGE_CBUSTASK_H
#define NEO_BRIDGE_CBUSTASK_H

#include "CThreadBase.h"
#include "CThreadPool.h"

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



#endif //NEO_BRIDGE_CBUSTASK_H
