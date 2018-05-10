//
// Created by huang on 18-3-19.
//

#include <neo_bridge/TaskCmdVel.h>
#include <neo_bridge/CRosNode.h>
#include <neoslam_sdk/TypePacket_CmdVel.h>
#include <neo_bridge/CBackServer.h>

CCmdVelTask::CCmdVelTask(int fd, Neo_Packet::HEAD *bus_head, char *buf, int Len) {
    memcpy(&this->head,bus_head,sizeof(Neo_Packet::HEAD));
    memcpy(&this->packet_body,buf,Len);
    this->fd = fd;
    this->shm_ack = CBackServer::GetAckShareMemery();
}

CCmdVelTask::~CCmdVelTask() {

}

void CCmdVelTask::doAction() {
    CPacketStream packet;
    Neo_Packet::CMDVEL_PACKET_RESPONSE response = {0};
    char response_buf[60] = {0};
    int Size = 0;
    response.ack = 1;
    printf("x = %f,y = %f,z = %f\n",packet_body.x,packet_body.y,packet_body.z);
    CRosNode::PopCmdVel(packet_body.x,packet_body.y,packet_body.z);
    packet.Packet((unsigned char *)response_buf, &Size, this->head.function_id, &response, sizeof(response), head.ref, head.device_id);

    shm_ack->Write((char*)response_buf, Size, fd);
}
