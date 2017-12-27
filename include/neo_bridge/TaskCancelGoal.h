//
// Created by huang on 17-12-26.
//

#ifndef NEO_BRIDGE_TASKCANCELGOAL_H
#define NEO_BRIDGE_TASKCANCELGOAL_H

#include "CThreadPool.h"
#include "CShareMem.h"
#include "packet.h"
#include "config.h"

typedef struct {
    char isAck;
}CANCELGOAL_PACKAGE_POP;

typedef struct {
    char ack;
}CANCELGOAL_PACKAGE_ACK;

class CCancelGoalTask:public CTask
{
private:

    packet_head head;

    int fd;
public:

    CCancelGoalTask(int fd, P_HEAD *bus_head, char *buf, int Len);
    ~CCancelGoalTask();
    void doAction();



};

#endif //NEO_BRIDGE_TASKCANCELGOAL_H
