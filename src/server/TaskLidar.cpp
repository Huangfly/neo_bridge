//
// Created by huang on 18-3-20.
//
#include <neo_bridge/TaskLidar.h>
#include <neo_bridge/CRosNode.h>
#include <neoslam_sdk/Type_Lidar.h>

extern CShareMem shm_ack;
static sensor_msgs::LaserScan ScanDatas;
static Neo_Type::POSE LaserToMap_pose;


CLidarTask::CLidarTask(int fd, P_HEAD *bus_head, char *buf, int Len) {
    memcpy(&this->head,bus_head,sizeof(P_HEAD));
    memcpy(&this->package_recv,buf,Len);
    this->fd = fd;
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
    unsigned char ack_buf[2200] = {0};
    Neo_Type::LIDAR_PACKAGE_ACK ack_package = {0};
    int Size = 0;
    int len = 0;
    CPacketStream packet;

    if( package_recv.package_num == 0 ){
        ScanDatas = CRosNode::scan_;
        memcpy(&LaserToMap_pose, &CRosNode::laser_pose,sizeof(Neo_Type::POSE));
    }

    if(ScanDatas.ranges.size() > 0){
        sensor_msgs::LaserScan *lidar_ptr = &ScanDatas;

      if( package_recv.serial_num != lidar_ptr->header.stamp.nsec ){
          //package_recv.package_num = 0;
          ack_package.serial_num = lidar_ptr->header.stamp.nsec;
      }
      //printf("recv%d:\n",package_recv.package_num);

      ack_package.angle_max = lidar_ptr->angle_max;
      ack_package.angle_min = lidar_ptr->angle_min;
      ack_package.angle_increment = lidar_ptr->angle_increment;
      memcpy(&ack_package.pose,&LaserToMap_pose,sizeof(Neo_Type::POSE));

      ack_package.package_sum = lidar_ptr->ranges.size()%LIDARDATA_SIZE > 0 ? lidar_ptr->ranges.size()/LIDARDATA_SIZE+1 : lidar_ptr->ranges.size()/LIDARDATA_SIZE;
      ack_package.package_num = this->package_recv.package_num+1;

      if ( ack_package.package_sum < ack_package.package_num ){
          return;
      }

      float *lidar_readPos = (float*)( &(lidar_ptr->ranges[ (ack_package.package_num - 1)*LIDARDATA_SIZE ]) );
      //printf("lidar %d %d\n",ack_package.package_sum, ack_package.package_num);
      if(ack_package.package_sum==ack_package.package_num)
      {
          if( lidar_ptr->ranges.size()%LIDARDATA_SIZE == 0 ){
              ack_package.rangs_size = LIDARDATA_SIZE;
              //memcpy(ack_package.rangs, lidar_readPos, LIDARDATA_SIZE*sizeof(float));

          }else{
              ack_package.rangs_size = lidar_ptr->ranges.size()%LIDARDATA_SIZE;
              //memcpy(ack_package.rangs, lidar_readPos, (lidar_ptr->ranges.size()%LIDARDATA_SIZE)*sizeof(float));
          }
          //memcpy(ack_package.rangs, lidar_readPos, (lidar_ptr->ranges.size()%LIDARDATA_SIZE == 0 ? LIDARDATA_SIZE : lidar_ptr->ranges.size()%LIDARDATA_SIZE));
          //CRosNode::robot_status.updateMap = 0;//update map success
      } else {
          ack_package.rangs_size = LIDARDATA_SIZE;
          //memcpy(ack_package.rangs, lidar_readPos, LIDARDATA_SIZE*sizeof(float));
      }
      pushScan(ack_package.rangs, lidar_ptr->ranges, (ack_package.package_num - 1)*LIDARDATA_SIZE, ack_package.rangs_size);
    }



    packet.Packet(ack_buf, &Size, PACK_LIDAR, &ack_package, sizeof(LIDAR_PACKAGE_ACK), head.trans_id, head.msg_code);
    shm_ack.Write((char*)ack_buf, Size, fd);
}
