//
// Created by huang on 17-12-14.
//

#ifndef NEO_BRIDGE_GOAL_H
#define NEO_BRIDGE_GOAL_H

#include "CRosNode.h"
#include "CThreadPool.h"
#include "CShareMem.h"
#include "packet.h"
#include "config.h"
#include <geometry_msgs/PoseStamped.h>

typedef struct {
    float x;
    float y;
    float z;
    float Quaternion[4];
}GOAL_PACKAGE_POP;

typedef struct {
    char isSuccess;
}GOAL_PACKAGE_ACK;

class CGoalTask:public CTask
{
private:

    packet_head head;

    int fd;
public:

    CGoalTask(int fd, P_HEAD *bus_head, char *buf, int Len);
    ~CGoalTask();
    void doAction();

    GOAL_PACKAGE_POP ack_pack;

};
#endif //NEO_BRIDGE_GOAL_H
