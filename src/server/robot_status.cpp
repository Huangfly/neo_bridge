//
// Created by huang on 17-12-11.
//
#include <neo_bridge/robot_status.h>
#include "robot_status.h"
#include "CRosNode.h"

extern CShareMem shm_ack;

//STATUS_PACKAGE_ACK CRobotStatusTask::robot_status = {0};


CRobotStatusTask::CRobotStatusTask(int fd, P_HEAD *bus_head, char *buf, int Len)
{
    memcpy(&this->head,bus_head,sizeof(P_HEAD));
    this->fd = fd;
}

CRobotStatusTask::~CRobotStatusTask()
{

}

void CRobotStatusTask::doAction() {
    CPacketStream packet;
    char ack_buf[60] = {0};
    int Size = 0;

    packet.Packet((unsigned char *)ack_buf, &Size, PACK_HEARD, &CRosNode::robot_status, sizeof(STATUS_PACKAGE_ACK), head.trans_id, head.msg_code);
    //DEBUG_LOG((unsigned char*)ack_buf,Size);
    //printf("recv %d\n",CRosNode::robot_status.updateMap);
    shm_ack.Write((char*)ack_buf, Size, fd);
}

