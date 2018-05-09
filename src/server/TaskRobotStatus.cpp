//
// Created by huang on 17-12-11.
//
#include <neo_bridge/TaskRobotStatus.h>
#include <neo_bridge/packet.h>
#include "CRosNode.h"
#include <neo_bridge/CBackServer.h>

CRobotStatusTask::CRobotStatusTask(int fd, Neo_Packet::HEAD *bus_head, char *buf, int Len)
{
    memcpy(&this->head,bus_head,sizeof(Neo_Packet::HEAD));
    memcpy(&this->recv_data,buf,Len);
    this->fd = fd;
    this->shm_ack = CBackServer::GetAckShareMemery();
}

CRobotStatusTask::~CRobotStatusTask()
{

}

void CRobotStatusTask::doAction() {
    CPacketStream packet;
    char ack_buf[60] = {0};
    int Size = 0;
    if(recv_data.isAck == 0){ return; }
    //NeoDebug("-------robotstatus\n");
    packet.Packet((unsigned char *)ack_buf, &Size, this->head.function_id, &CRosNode::robot_status, sizeof(Neo_Packet::STATUS_PACKAGE_ACK), head.ref, head.device_id);
    shm_ack->Write((char*)ack_buf, Size, fd);
}

