//
// Created by huang on 17-12-26.
//

#ifndef NEO_BRIDGE_TASKNODECTL_H
#define NEO_BRIDGE_TASKNODECTL_H

#include "CThreadPool.h"
#include "CShareMem.h"
#include "packet.h"
#include <neoslam_sdk/TypePacket_RosControl.h>
#include <neo_bridge/CDebug.h>

class CRosControlTask:public CTask
{
private:

    Neo_Packet::HEAD head;
    Neo_Packet::NODECTL_PACKAGE_POP pop;
    int fd;
    CShareMem *shm_ack;
public:

    CRosControlTask(int fd, Neo_Packet::HEAD *bus_head, char *buf, int Len);
    ~CRosControlTask();
    void doAction();



};

#endif //NEO_BRIDGE_TASKNODECTL_H
