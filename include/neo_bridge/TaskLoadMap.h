//
// Created by huang on 18-4-20.
//

#ifndef NEO_BRIDGE_TASKLOADMAP_H
#define NEO_BRIDGE_TASKLOADMAP_H

#include "CThreadPool.h"
#include "CShareMem.h"
#include "packet.h"
#include <neoslam_sdk/TypePacket_MapUpload.h>
#include "sensor_msgs/LaserScan.h"
#include <neo_bridge/CDebug.h>

using namespace Neo_Type;

class CLoadMapTask:public CTask
{
private:

    Neo_Packet::HEAD head;
    Neo_Packet::LOADMAP_PACKAGE_POP recv_packet;
    CShareMem *shm_ack;
    int fd;
public:

    CLoadMapTask(int fd, Neo_Packet::HEAD *bus_head, char *buf, int Len);
    ~CLoadMapTask();
    void doAction();
};



#endif //NEO_BRIDGE_TASKLOADMAP_H
