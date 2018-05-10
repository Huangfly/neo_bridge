//
// Created by huang on 18-3-19.
//

#ifndef NEO_BRIDGE_TASKCMDVEL_H
#define NEO_BRIDGE_TASKCMDVEL_H

#include "CThreadPool.h"
#include "CShareMem.h"
#include "packet.h"
#include <neoslam_sdk/TypePacket_CmdVel.h>
#include <neo_bridge/CDebug.h>

class CCmdVelTask:public CTask
{
private:

    Neo_Packet::HEAD head;
    Neo_Packet::CMDVEL_PACKET_REQUEST packet_body;
    int fd;
    CShareMem *shm_ack;
public:

    CCmdVelTask(int fd, Neo_Packet::HEAD *bus_head, char *buf, int Len);
    ~CCmdVelTask();
    void doAction();



};

#endif //NEO_BRIDGE_TASKCMDVEL_H
