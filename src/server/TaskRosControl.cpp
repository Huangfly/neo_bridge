//
// Created by huang on 17-12-26.
//
#include <neo_bridge/TaskRosControl.h>
#include <neo_bridge/packet.h>
#include "TaskRosControl.h"
#include "CRosNodeManager.h"
#include <neo_bridge/CBackServer.h>

//STATUS_PACKAGE_ACK CRobotStatusTask::robot_status = {0};


CRosControlTask::CRosControlTask(int fd, Neo_Packet::HEAD *bus_head, char *buf, int Len)
{
    memcpy(&this->head,bus_head,sizeof(Neo_Packet::HEAD));
    memcpy(&this->pop,buf,Len);
    this->fd = fd;
    this->shm_ack = CBackServer::GetAckShareMemery();
}

CRosControlTask::~CRosControlTask()
{

}

void CRosControlTask::doAction() {
    CPacketStream packet;
    Neo_Packet::NODECTL_PACKAGE_ACK ack = {0};
    char ack_buf[60] = {0};
    int Size = 0;
    string str;

    str = pop.node_name;

    NeoDebug("action node :%s %d\n",pop.node_name,pop.enable);
    if(CRosNodeManager::funcCtlNode(str,pop.enable) == false)
    {
        sprintf(ack.ack,"fail.");
        printf("node fail.\n");
    }
    else
    {
        sprintf(ack.ack,"ok.");
        printf("node ok.\n");
    }


    packet.Packet((unsigned char *)ack_buf, &Size, this->head.function_id, &ack, sizeof(ack), head.ref, head.device_id);

    shm_ack->Write((char*)ack_buf, Size, fd);
}
