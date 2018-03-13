//
// Created by huang on 17-12-26.
//
#include <neo_bridge/TaskNodeCtl.h>
#include "TaskNodeCtl.h"
#include "CRosNodeManager.h"

extern CShareMem shm_ack;

//STATUS_PACKAGE_ACK CRobotStatusTask::robot_status = {0};


CNodeCtlTask::CNodeCtlTask(int fd, P_HEAD *bus_head, char *buf, int Len)
{
    memcpy(&this->head,bus_head,sizeof(P_HEAD));
    memcpy(&this->pop,buf,sizeof(NODECTL_PACKAGE_POP));
    this->fd = fd;
}

CNodeCtlTask::~CNodeCtlTask()
{

}

void CNodeCtlTask::doAction() {
    CPacketStream packet;
    NODECTL_PACKAGE_ACK ack;
    char ack_buf[60] = {0};
    int Size = 0;
    string str;

    str = pop.node_name;

    printf("aaaa\n");
    if(CRosNodeManager::funcCtlNode(str,pop.enable) == false)
    {
        sprintf(ack.ack,"fail.");
    }
    else
    {
        sprintf(ack.ack,"ok.");
    }


    packet.Packet((unsigned char *)ack_buf, &Size, PACK_HEARD, &ack, sizeof(ack), head.trans_id, head.msg_code);

    shm_ack.Write((char*)ack_buf, Size, fd);
}
