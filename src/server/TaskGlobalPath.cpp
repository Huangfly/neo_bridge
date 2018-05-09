//
// Created by huang on 18-4-26.
//

#include <neoslam_sdk/TypePacket_GlobalPath.h>
#include <neoslam_sdk/Type_Pose.h>
#include "TaskGlobalPath.h"
#include "CBackServer.h"
#include <neo_bridge/CBackServer.h>

//static sensor_msgs::LaserScan ScanDatas;
static Neo_Type::POSE LaserToMap_pose;


CGlobalPathTask::CGlobalPathTask(int fd, Neo_Packet::HEAD *bus_head, char *buf, int Len) {
    memcpy(&this->head,bus_head,sizeof(Neo_Packet::HEAD));
    //memcpy(&this->package_recv,buf,Len);
    this->fd = fd;
    this->shm_ack = CBackServer::GetAckShareMemery();
}

CGlobalPathTask::~CGlobalPathTask() {

}

void CGlobalPathTask::doAction() {
    geometry_msgs::PoseStamped pose;
    nav_msgs::Path path;
    CPacketStream packet;
    int Size = 0;
    Neo_Packet::GLOBALPATH_PACKAGE_ACK ack = {0};
    char ack_buf[600] = {0};


            CRosNode::GetGlobalPath(path);
            int size = path.poses.size();

            if(size > PATH_MAX_SIZE){
                float step = size / (PATH_MAX_SIZE * 1.0f);
                int step_ = (int)(step + 0.95);

                int i, j;
                for (i = 0, j = 0; i < size && j < (PATH_MAX_SIZE); i+=step_, j++) {
                    ack.path[j].x = path.poses[i].pose.position.x;
                    ack.path[j].y = path.poses[i].pose.position.y;
                }
                ack.path_size = j;
            }else{
                ack.path_size = size;
                for (int i = 0; i < size; ++i) {
                    ack.path[i].x = path.poses[i].pose.position.x;
                    ack.path[i].y = path.poses[i].pose.position.y;
                }
            }

    packet.Packet((unsigned char*)ack_buf, &Size, this->head.function_id, &ack, sizeof(ack), head.ref, head.device_id);
    shm_ack->Write((char*)ack_buf, Size, fd);
}