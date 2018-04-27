//
// Created by huang on 18-4-26.
//

#ifndef NEO_BRIDGE_TASKGLOBALPATH_H
#define NEO_BRIDGE_TASKGLOBALPATH_H
#include "CThreadPool.h"
#include "CShareMem.h"
#include "packet.h"
#include "config.h"
#include <neoslam_sdk/Type_GlobalPath.hpp>

//using namespace Neo_Type;

class CGlobalPathTask:public CTask
{
private:

    packet_head head;
    //Neo_Type::LIDAR_PACKAGE_POP package_recv;

    int fd;
public:

    CGlobalPathTask(int fd, P_HEAD *bus_head, char *buf, int Len);
    ~CGlobalPathTask();
    void doAction();
};
#endif //NEO_BRIDGE_TASKGLOBALPATH_H
