//#include <neo_bridge/map.h>
#include <neo_bridge/CRosNode.h>
#include <neo_bridge/map.h>
#include "map.h"

extern CShareMem shm_ack;


CMapTask::CMapTask(int fd, P_HEAD *bus_head, char *buf, int Len)
{
    memcpy(&this->head,bus_head,sizeof(P_HEAD));
    memcpy(&this->package_pop,buf,Len);
    this->fd = fd;
}

CMapTask::~CMapTask()
{

}

void CMapTask::doAction()
{
    unsigned char ack_buf[600] = {0};
    MAP_PACKAGE_ACK ack_package = {0};
    int Size = 0;
    int len = 0;
    CPacketStream packet;
    this->map_ptr = &CRosNode::map_base_;
    ack_package.package_sum = map_ptr->data.size()%512 > 0 ? map_ptr->data.size()/512+1 : map_ptr->data.size()/512;
    ack_package.package_num = this->package_pop.package_num+1;
    ack_package.width = map_ptr->info.width;
    ack_package.hight = map_ptr->info.height;
    ack_package.resolution = map_ptr->info.resolution;

    ack_package.x = map_ptr->info.origin.position.x;
    ack_package.y = map_ptr->info.origin.position.y;
    ack_package.z = map_ptr->info.origin.position.z;
    ack_package.Quaternion[0] = map_ptr->info.origin.orientation.x;
    ack_package.Quaternion[1] = map_ptr->info.origin.orientation.y;
    ack_package.Quaternion[2] = map_ptr->info.origin.orientation.z;
    ack_package.Quaternion[3] = map_ptr->info.origin.orientation.w;

    //printf("pack num:%d\n",ack_package.package_num);
    if(ack_package.package_sum<ack_package.package_num)return;
    ack_package.size = this->map_ptr->data.size();
    signed char *mappage = (signed char*)(&(map_ptr->data[(ack_package.package_num - 1) * 512]));
    //signed char *mappage = (signed char*)(&(map_ptr->data[0]));
    if(ack_package.package_sum==ack_package.package_num)
    {

        memcpy(ack_package.data, mappage, (map_ptr->data.size()%512 == 0 ? 512 : map_ptr->data.size()%512));
        CRosNode::robot_status.updateMap = 0;//update map success
    } else {

        memcpy(ack_package.data, mappage, 512);
    }

    //ack_package.data[511] = 0xDD;
    packet.Packet(ack_buf, &Size, PACK_MAP, &ack_package, sizeof(MAP_PACKAGE_ACK), head.trans_id, head.msg_code);
    shm_ack.Write((char*)ack_buf, Size, fd);
}
