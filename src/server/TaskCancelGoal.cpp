//
// Created by huang on 17-12-26.
//
#include <neo_bridge/TaskCancelGoal.h>
#include <neo_bridge/packet.h>
#include "TaskCancelGoal.h"
#include "CRosNode.h"
#include <neo_bridge/CBackServer.h>

//STATUS_PACKAGE_ACK CRobotStatusTask::robot_status = {0};


CCancelGoalTask::CCancelGoalTask(int fd, Neo_Packet::HEAD *bus_head, char *buf, int Len)
{
    memcpy(&this->head,bus_head,sizeof(Neo_Packet::HEAD));
    this->fd = fd;
    this->shm_ack = CBackServer::GetAckShareMemery();
}

CCancelGoalTask::~CCancelGoalTask()
{

}

void CCancelGoalTask::doAction() {
    CPacketStream packet;
    Neo_Packet::CANCELGOAL_PACKAGE_ACK ack;
    char ack_buf[60] = {0};
    int Size = 0;
    ack.ack = 1;
    CRosNode::PopCancelGoal();
    packet.Packet((unsigned char *)ack_buf, &Size, this->head.function_id, &ack, sizeof(ack), head.ref, head.device_id);

    shm_ack->Write((char*)ack_buf, Size, fd);
}