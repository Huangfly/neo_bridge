//
// Created by huang on 17-12-26.
//

#ifndef NEO_BRIDGE_TASKNODECTL_H
#define NEO_BRIDGE_TASKNODECTL_H

#include "CThreadPool.h"
#include "CShareMem.h"
#include "packet.h"
#include "config.h"

typedef struct {
    char enable;//0 enable  1 unable
    char node_name[20];
}NODECTL_PACKAGE_POP;

typedef struct {
    char ack[10];
}NODECTL_PACKAGE_ACK;

class CNodeCtlTask:public CTask
{
private:

    packet_head head;
    NODECTL_PACKAGE_POP pop;
    int fd;
public:

    CNodeCtlTask(int fd, P_HEAD *bus_head, char *buf, int Len);
    ~CNodeCtlTask();
    void doAction();



};

#endif //NEO_BRIDGE_TASKNODECTL_H
