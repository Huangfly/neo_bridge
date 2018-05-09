//
// Created by huang on 17-12-11.
//

#ifndef NEO_BRIDGE_ROBOT_STATUS_H
#define NEO_BRIDGE_ROBOT_STATUS_H

#include "CThreadPool.h"
#include "CShareMem.h"
#include "packet.h"
#include <neoslam_sdk/TypePacket_RobotStatus.h>
#include <neo_bridge/CDebug.h>

class CRobotStatusTask:public CTask
{
private:
    Neo_Packet::HEAD head;
    Neo_Packet::STATUS_PACKAGE_POP recv_data;
    int fd;
    CShareMem *shm_ack;
public:

    CRobotStatusTask(int fd, Neo_Packet::HEAD *bus_head, char *buf, int Len);
    ~CRobotStatusTask();
    void doAction();



};

#endif //NEO_BRIDGE_ROBOT_STATUS_H
