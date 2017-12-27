//
// Created by huang on 17-12-14.
/// /
#include <neo_bridge/goal.h>
#include "goal.h"


extern CShareMem shm_ack;

CGoalTask::CGoalTask(int fd, P_HEAD *bus_head, char *buf, int Len)
{
    this->fd = fd;
    memcpy(&this->head,bus_head,sizeof(P_HEAD));
    memcpy(&this->ack_pack,buf,sizeof(GOAL_PACKAGE_POP));


}
CGoalTask::~CGoalTask()
{

}

void CGoalTask::doAction()
{
    geometry_msgs::PoseStamped pose;
    CPacketStream packet;
    GOAL_PACKAGE_ACK ack_body;
    char ack_buf[60] = {0};
    int Size = 0;

    ack_body.isSuccess = 1;
    pose.pose.position.x = this->ack_pack.x;
    pose.pose.position.y = this->ack_pack.y;
    pose.pose.position.z = this->ack_pack.z;
    pose.pose.orientation.x = this->ack_pack.Quaternion[0];
    pose.pose.orientation.y = this->ack_pack.Quaternion[1];
    pose.pose.orientation.z = this->ack_pack.Quaternion[2];
    pose.pose.orientation.w = this->ack_pack.Quaternion[3];

    CRosNode::PopMoveGoal(pose);
    //printf("recv goal...\n");
    packet.Packet((unsigned char *)ack_buf, &Size, PACK_GOAL, &ack_body, sizeof(GOAL_PACKAGE_ACK), head.trans_id, head.msg_code);
    //DEBUG_LOG((unsigned char*)ack_buf,Size);
    shm_ack.Write((char*)ack_buf, Size, fd);
}