//
// Created by huang on 17-12-26.
//

#ifndef NEO_BRIDGE_TASKCANCELGOAL_H
#define NEO_BRIDGE_TASKCANCELGOAL_H

#include "CThreadPool.h"
#include "CShareMem.h"
#include "packet.h"
#include <neoslam_sdk/TypePacket_CancelGoal.h>
#include <neo_bridge/CDebug.h>


class CCancelGoalTask:public CTask
{
private:
    Neo_Packet::HEAD head;
    int fd;
    CShareMem *shm_ack;
public:
    CCancelGoalTask(int fd, Neo_Packet::HEAD *bus_head, char *buf, int Len);
    ~CCancelGoalTask();
    void doAction();
};

#endif //NEO_BRIDGE_TASKCANCELGOAL_H
