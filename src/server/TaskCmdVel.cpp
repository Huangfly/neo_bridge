//
// Created by huang on 18-3-19.
//

#include <neo_bridge/TaskCmdVel.h>
#include <neo_bridge/CRosNode.h>
#include <neoslam_sdk/Type_CmdVel.h>

extern CShareMem shm_ack;

CCmdVelTask::CCmdVelTask(int fd, P_HEAD *bus_head, char *buf, int Len) {
    memcpy(&this->head,bus_head,sizeof(P_HEAD));
    memcpy(&this->packet_body,buf,sizeof(CMDVEL_PACKAGE_POP));
    this->fd = fd;
}

CCmdVelTask::~CCmdVelTask() {

}

void CCmdVelTask::doAction() {
    CPacketStream packet;
    CMDVEL_PACKAGE_ACK ack;
    char ack_buf[60] = {0};
    int Size = 0;
    ack.ack = 1;
    printf("x = %f,y = %f,z = %f\n",packet_body.x,packet_body.y,packet_body.z);
    CRosNode::PopCmdVel(packet_body.x,packet_body.y,packet_body.z);
    packet.Packet((unsigned char *)ack_buf, &Size, PACK_HEARD, &ack, sizeof(ack), head.trans_id, head.msg_code);

    shm_ack.Write((char*)ack_buf, Size, fd);
}
