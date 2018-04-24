//
// Created by huang on 18-4-20.
//

#ifndef NEO_BRIDGE_TASKLOADMAP_H
#define NEO_BRIDGE_TASKLOADMAP_H

#include "CThreadPool.h"
#include "CShareMem.h"
#include "packet.h"
#include "config.h"
#include <neoslam_sdk/Type_LoadMap.h>
#include "sensor_msgs/LaserScan.h"


using namespace Neo_Type;

class CLoadMapTask:public CTask
{
private:

    packet_head head;
    Neo_Type::LOADMAP_PACKAGE_POP recv_packet;

    int fd;
public:

    CLoadMapTask(int fd, P_HEAD *bus_head, char *buf, int Len);
    ~CLoadMapTask();
    void doAction();
};



#endif //NEO_BRIDGE_TASKLOADMAP_H
