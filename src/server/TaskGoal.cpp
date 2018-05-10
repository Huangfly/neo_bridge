//
// Created by huang on 17-12-14.
/// /
#include <neo_bridge/TaskGoal.h>
#include <neo_bridge/packet.h>
#include "TaskGoal.h"
#include <neo_bridge/CBackServer.h>


CGoalTask::CGoalTask(int fd, Neo_Packet::HEAD *bus_head, char *buf, int Len)
{
    this->fd = fd;
    memcpy(&this->head,bus_head,sizeof(Neo_Packet::HEAD));
    memcpy(&this->ack_pack,buf,Len);
    this->shm_ack = CBackServer::GetAckShareMemery();
}
CGoalTask::~CGoalTask()
{

}

void CGoalTask::doAction()
{
    geometry_msgs::PoseStamped pose;
    CPacketStream packet;
    Neo_Packet::SENDGOAL_PACKET_RESPONSE response;
    char response_buf[60] = {0};
    int Size = 0;

    response.isSuccess = 1;
    pose.pose.position.x = this->ack_pack.x;
    pose.pose.position.y = this->ack_pack.y;
    pose.pose.position.z = this->ack_pack.z;
    pose.pose.orientation.x = this->ack_pack.Quaternion[0];
    pose.pose.orientation.y = this->ack_pack.Quaternion[1];
    pose.pose.orientation.z = this->ack_pack.Quaternion[2];
    pose.pose.orientation.w = this->ack_pack.Quaternion[3];

    CRosNode::PopMoveGoal(pose);
    //printf("recv goal...\n");
    packet.Packet((unsigned char *)response_buf, &Size, this->head.function_id, &response, sizeof(response), head.ref, head.device_id);
    shm_ack->Write((char*)response_buf, Size, fd);
}