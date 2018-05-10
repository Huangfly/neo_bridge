//#include <neo_bridge/map.h>
#include <neo_bridge/CRosNode.h>
#include <neo_bridge/TaskMap.h>
#include <neo_bridge/packet.h>
#include "TaskMap.h"
#include <neo_bridge/CBackServer.h>


CMapTask::CMapTask(int fd, Neo_Packet::HEAD *bus_head, char *buf, int Len)
{
    memcpy(&this->head,bus_head,sizeof(Neo_Packet::HEAD));
    memcpy(&this->package_pop,buf,Len);
    this->fd = fd;
    this->shm_ack = CBackServer::GetAckShareMemery();
}

CMapTask::~CMapTask()
{

}

void CMapTask::doAction()
{
    unsigned char response_buf[800] = {0};
    Neo_Packet::MAPDATAS_PACKET_RESPONSE response = {0};
    int Size = 0;
    int len = 0;
    CPacketStream packet;

    if( CBackServer::UserDatas.findKey(head.device_id) ){
        Neo_Type::UserData *user = NULL;
        CBackServer::UserDatas.get(head.device_id,&user);
        if(user != NULL) {

            if(package_pop.package_sum == 0 && package_pop.package_num == 0){
                //map_Datas = CRosNode::map_base_;
                CRosNode::GetMap(user->MapDatas);
            }

            this->map_ptr = &user->MapDatas;
            if(this->map_ptr->data.size() > 0) {

                response.package_sum =
                        map_ptr->data.size() % 512 > 0 ? map_ptr->data.size() / 512 + 1 : map_ptr->data.size() / 512;
                response.package_num = this->package_pop.package_num + 1;
                response.width = map_ptr->info.width;
                response.hight = map_ptr->info.height;
                response.resolution = map_ptr->info.resolution;

                response.x = map_ptr->info.origin.position.x;
                response.y = map_ptr->info.origin.position.y;
                response.z = map_ptr->info.origin.position.z;
                response.Quaternion[0] = map_ptr->info.origin.orientation.x;
                response.Quaternion[1] = map_ptr->info.origin.orientation.y;
                response.Quaternion[2] = map_ptr->info.origin.orientation.z;
                response.Quaternion[3] = map_ptr->info.origin.orientation.w;

                //printf("pack num:%d\n",ack_package.package_num);
                if (response.package_sum < response.package_num)return;
                response.size = this->map_ptr->data.size();
                signed char *mappage = (signed char *) (&(map_ptr->data[(response.package_num - 1) * 512]));
                if (response.package_sum == response.package_num) {

                    memcpy(response.data, mappage, (map_ptr->data.size() % 512 == 0 ? 512 : map_ptr->data.size() % 512));
                    CRosNode::robot_status.updateMap = 0;//update map success
                } else {

                    memcpy(response.data, mappage, 512);
                }
            }
            //NeoInfo("send Map\n");
        }
    }else{
        NeoInfo("No User %d\n",CBackServer::UserDatas.size());
    }


    packet.Packet(response_buf, &Size, this->head.function_id, &response, sizeof(Neo_Packet::MAPDATAS_PACKET_RESPONSE), head.ref, head.device_id);
    shm_ack->Write((char*)response_buf, Size, fd);
}
