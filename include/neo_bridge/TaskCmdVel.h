//
// Created by huang on 18-3-19.
//

#ifndef NEO_BRIDGE_TASKCMDVEL_H
#define NEO_BRIDGE_TASKCMDVEL_H

#include "CThreadPool.h"
#include "CShareMem.h"
#include "packet.h"
#include "config.h"
#include <neoslam_sdk/Type_CmdVel.h>


class CCmdVelTask:public CTask
{
private:

    packet_head head;
    CMDVEL_PACKAGE_POP packet_body;
    int fd;
public:

    CCmdVelTask(int fd, P_HEAD *bus_head, char *buf, int Len);
    ~CCmdVelTask();
    void doAction();



};

#endif //NEO_BRIDGE_TASKCMDVEL_H
