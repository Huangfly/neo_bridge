//
// Created by huang on 18-5-7.
//

#ifndef NEO_BRIDGE_CUNDATAPACKTASK_H
#define NEO_BRIDGE_CUNDATAPACKTASK_H

#include "CThreadPool.h"

class CUnDataPackTask: public CTask
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

#endif //NEO_BRIDGE_CUNDATAPACKTASK_H
