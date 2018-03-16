//
// Created by huang on 17-12-11.
//
/*
GoalID goal_id\n\
uint8 status\n\
uint8 PENDING         = 0   # The goal has yet to be processed by the action server\n\
uint8 ACTIVE          = 1   # The goal is currently being processed by the action server\n\
uint8 PREEMPTED       = 2   # The goal received a cancel request after it started executing\n\
                            #   and has since completed its execution (Terminal State)\n\
uint8 SUCCEEDED       = 3   # The goal was achieved successfully by the action server (Terminal State)\n\
uint8 ABORTED         = 4   # The goal was aborted during execution by the action server due\n\
                            #    to some failure (Terminal State)\n\
uint8 REJECTED        = 5   # The goal was rejected by the action server without being processed,\n\
                            #    because the goal was unattainable or invalid (Terminal State)\n\
uint8 PREEMPTING      = 6   # The goal received a cancel request after it started executing\n\
                            #    and has not yet completed execution\n\
uint8 RECALLING       = 7   # The goal received a cancel request before it started executing,\n\
                            #    but the action server has not yet confirmed that the goal is canceled\n\
uint8 RECALLED        = 8   # The goal received a cancel request before it started executing\n\
                            #    and was successfully cancelled (Terminal State)\n\
uint8 LOST            = 9   # An action client can determine that a goal is LOST. This should not be\n\
                            #    sent over the wire by an action server\n\
*/
#ifndef NEO_BRIDGE_ROBOT_STATUS_H
#define NEO_BRIDGE_ROBOT_STATUS_H

#include "CThreadPool.h"
#include "CShareMem.h"
#include "packet.h"
#include "config.h"

typedef struct {
    char isAck;
}STATUS_PACKAGE_POP;

typedef struct {
    float x;
    float y;
    float z;
    float Quaternion[4];
    char updateMap;
    char movebase_status;
    char ref[2];
}STATUS_PACKAGE_ACK;

class CRobotStatusTask:public CTask
{
private:

    packet_head head;

    int fd;
public:

    CRobotStatusTask(int fd, P_HEAD *bus_head, char *buf, int Len);
    ~CRobotStatusTask();
    void doAction();



};

#endif //NEO_BRIDGE_ROBOT_STATUS_H
