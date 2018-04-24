//
// Created by huang on 17-12-11.
//
#include <neo_bridge/robot_status.h>
#include "robot_status.h"
#include "CRosNode.h"

extern CShareMem shm_ack;

static int aaaa = 0;

//STATUS_PACKAGE_ACK CRobotStatusTask::robot_status = {0};


CRobotStatusTask::CRobotStatusTask(int fd, P_HEAD *bus_head, char *buf, int Len)
{
    memcpy(&this->head,bus_head,sizeof(P_HEAD));
    memcpy(&this->recv_data,buf,Len);
    this->fd = fd;
}

CRobotStatusTask::~CRobotStatusTask()
{

}

void CRobotStatusTask::doAction() {
    CPacketStream packet;
    char ack_buf[60] = {0};
    int Size = 0;
    if(recv_data.isAck == 0){ return; }

    //printf("-----------%d\n",recv_data.isAck);
    packet.Packet((unsigned char *)ack_buf, &Size, PACK_HEARD, &CRosNode::robot_status, sizeof(STATUS_PACKAGE_ACK), head.trans_id, head.msg_code);
    //DEBUG_LOG((unsigned char*)ack_buf,Size);
    //printf("recv %d  %d send %d: %d\n ",CRosNode::robot_status.updateMap,sizeof(STATUS_PACKAGE_ACK),fd,Size);
    //aaaa++;
    shm_ack.Write((char*)ack_buf, Size, fd);
}

