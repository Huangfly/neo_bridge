//
// Created by huang on 18-3-20.
//
#include <neo_bridge/TaskLidar.h>
#include <neo_bridge/CRosNode.h>
#include <neoslam_sdk/TypePacket_LidarDatas.h>
#include <neo_bridge/packet.h>
#include <neo_bridge/CBackServer.h>

//static sensor_msgs::LaserScan ScanDatas;
//static Neo_Type::POSE LaserToMap_pose;


CLidarTask::CLidarTask(int fd, Neo_Packet::HEAD *bus_head, char *buf, int Len) {
    memcpy(&this->head,bus_head,sizeof(Neo_Packet::HEAD));
    memcpy(&this->package_recv,buf,Len);
    this->fd = fd;
    this->shm_ack = CBackServer::GetAckShareMemery();
}

CLidarTask::~CLidarTask() {

}

void CLidarTask::pushScan( float* rangs, std::vector<float> vec, int inset_num, int size ){
    int i = 0;
    if(vec.size() < (inset_num + size)
            || rangs == NULL){
        return;
    }
    std::vector<float>::iterator it;
    it = vec.begin()+inset_num;
    for (i = 0; (i < size) && (it != vec.end()); ++i) {
        rangs[i] = *it;
        it++;
    }
}

void CLidarTask::doAction() {
    unsigned char response_buf[2200] = {0};
    Neo_Packet::LIDARDATAS_PACKET_RESPONSE response = {0};
    int Size = 0;
    int len = 0;
    CPacketStream packet;
    if( CBackServer::UserDatas.findKey(head.device_id) ){
        Neo_Type::UserData *user = NULL;
        CBackServer::UserDatas.get(head.device_id,&user);
        if(user != NULL) {

            if( package_recv.package_num == 0 ){
                //ScanDatas = CRosNode::scan_;
                //memcpy(&LaserToMap_pose, &CRosNode::laser_pose,sizeof(Neo_Type::POSE));
                CRosNode::GetLidar(user->ScanDatas, user->m_LaserPose);
            }

            if(user->ScanDatas.ranges.size() > 0){
                sensor_msgs::LaserScan *lidar_ptr = &user->ScanDatas;

                if( package_recv.serial_num != lidar_ptr->header.stamp.nsec ){
                    //package_recv.package_num = 0;
                    response.serial_num = lidar_ptr->header.stamp.nsec;
                }
                //printf("recv%d:\n",package_recv.package_num);

                response.angle_max = lidar_ptr->angle_max;
                response.angle_min = lidar_ptr->angle_min;
                response.angle_increment = lidar_ptr->angle_increment;
                memcpy(&response.pose,&user->m_LaserPose,sizeof(Neo_Type::POSE));

                response.package_sum = lidar_ptr->ranges.size()%LIDARDATA_SIZE > 0 ? lidar_ptr->ranges.size()/LIDARDATA_SIZE+1 : lidar_ptr->ranges.size()/LIDARDATA_SIZE;
                response.package_num = this->package_recv.package_num+1;

                if ( response.package_sum < response.package_num ){
                    return;
                }

                float *lidar_readPos = (float*)( &(lidar_ptr->ranges[ (response.package_num - 1)*LIDARDATA_SIZE ]) );
                //printf("lidar %d %d\n",ack_package.package_sum, ack_package.package_num);
                if(response.package_sum==response.package_num)
                {
                    if( lidar_ptr->ranges.size()%LIDARDATA_SIZE == 0 ){
                        response.rangs_size = LIDARDATA_SIZE;
                    }else{
                        response.rangs_size = lidar_ptr->ranges.size()%LIDARDATA_SIZE;
                    }
                } else {
                    response.rangs_size = LIDARDATA_SIZE;
                }
                pushScan(response.rangs, lidar_ptr->ranges, (response.package_num - 1)*LIDARDATA_SIZE, response.rangs_size);
            }
        }
    }else{
        NeoInfo("No User %d\n",CBackServer::UserDatas.size());
    }


    packet.Packet(response_buf, &Size, this->head.function_id, &response, sizeof(response), head.ref, head.device_id);
    shm_ack->Write((char*)response_buf, Size, fd);
}
