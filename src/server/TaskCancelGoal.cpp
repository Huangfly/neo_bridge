//
// Created by huang on 17-12-26.
//
#include <neo_bridge/TaskCancelGoal.h>
#include "TaskCancelGoal.h"
#include "CRosNode.h"

extern CShareMem shm_ack;

//STATUS_PACKAGE_ACK CRobotStatusTask::robot_status = {0};


CCancelGoalTask::CCancelGoalTask(int fd, P_HEAD *bus_head, char *buf, int Len)
{
    memcpy(&this->head,bus_head,sizeof(P_HEAD));
    this->fd = fd;
}

CCancelGoalTask::~CCancelGoalTask()
{

}

void CCancelGoalTask::doAction() {
    CPacketStream packet;
    CANCELGOAL_PACKAGE_ACK ack;
    char ack_buf[60] = {0};
    int Size = 0;
    ack.ack = 1;
    CRosNode::PopCancelGoal();
    packet.Packet((unsigned char *)ack_buf, &Size, PACK_HEARD, &ack, sizeof(ack), head.trans_id, head.msg_code);

    shm_ack.Write((char*)ack_buf, Size, fd);
}