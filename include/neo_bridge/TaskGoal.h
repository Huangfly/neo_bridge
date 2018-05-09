//
// Created by huang on 17-12-14.
//

#ifndef NEO_BRIDGE_GOAL_H
#define NEO_BRIDGE_GOAL_H

#include "CRosNode.h"
#include "CThreadPool.h"
#include "CShareMem.h"
#include "packet.h"
#include <geometry_msgs/PoseStamped.h>
#include <neoslam_sdk/TypePacket_SendGoal.h>
#include <neo_bridge/CDebug.h>

class CGoalTask:public CTask
{
private:

    Neo_Packet::HEAD head;
    Neo_Packet::GOAL_PACKAGE_POP ack_pack;
    int fd;
    CShareMem *shm_ack;
public:

    CGoalTask(int fd, Neo_Packet::HEAD *bus_head, char *buf, int Len);
    ~CGoalTask();
    void doAction();



};
#endif //NEO_BRIDGE_GOAL_H
