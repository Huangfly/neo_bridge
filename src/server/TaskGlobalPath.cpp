//
// Created by huang on 18-4-26.
//

#include <neoslam_sdk/Type_GlobalPath.hpp>
#include <neoslam_sdk/Type_Pose.h>
#include "TaskGlobalPath.h"
#include "CBackServer.h"


extern CShareMem shm_ack;
//static sensor_msgs::LaserScan ScanDatas;
static Neo_Type::POSE LaserToMap_pose;


CGlobalPathTask::CGlobalPathTask(int fd, P_HEAD *bus_head, char *buf, int Len) {
    memcpy(&this->head,bus_head,sizeof(P_HEAD));
    //memcpy(&this->package_recv,buf,Len);
    this->fd = fd;
}

CGlobalPathTask::~CGlobalPathTask() {

}

void CGlobalPathTask::doAction() {
    geometry_msgs::PoseStamped pose;
    nav_msgs::Path path;
    CPacketStream packet;
    int Size = 0;
    GLOBALPATH_PACKAGE_ACK ack_package = {0};
    char ack_buf[600] = {0};


            CRosNode::GetGlobalPath(path);
            int size = path.poses.size();

            if(size > PATH_MAX_SIZE){
                float step = size / (PATH_MAX_SIZE * 1.0f);
                int step_ = (int)(step + 0.95);

                int i, j;
                for (i = 0, j = 0; i < size && j < (PATH_MAX_SIZE); i+=step_, j++) {
                    ack_package.path[j].x = path.poses[i].pose.position.x;
                    ack_package.path[j].y = path.poses[i].pose.position.y;
                }
                ack_package.path_size = j;
            }else{
                ack_package.path_size = size;
                for (int i = 0; i < size; ++i) {
                    ack_package.path[i].x = path.poses[i].pose.position.x;
                    ack_package.path[i].y = path.poses[i].pose.position.y;
                }
            }

    packet.Packet((unsigned char*)ack_buf, &Size, PACK_GLOBALPATH, &ack_package, sizeof(GLOBALPATH_PACKAGE_ACK), head.trans_id, head.msg_code);
    shm_ack.Write((char*)ack_buf, Size, fd);
}