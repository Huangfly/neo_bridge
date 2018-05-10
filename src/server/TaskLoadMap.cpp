//
// Created by huang on 18-4-20.
//

#include <neoslam_sdk/TypePacket_MapUpload.h>
#include <neoslam_sdk/Type_Pose.h>
#include <neo_bridge/packet.h>
#include "TaskLoadMap.h"
#include "CBackServer.h"
#include "tf2/LinearMath/Matrix3x3.h"
#include "neo_bridge/CConfig.h"
#include <neo_bridge/CBackServer.h>


CLoadMapTask::CLoadMapTask(int fd, Neo_Packet::HEAD *bus_head, char *buf, int Len) {
    memcpy(&this->head,bus_head,sizeof(Neo_Packet::HEAD));
    memcpy(&this->recv_packet,buf,Len);
    this->fd = fd;
    this->shm_ack = CBackServer::GetAckShareMemery();
}

CLoadMapTask::~CLoadMapTask() {

}

static bool SaveMap(int ID){
    if( CBackServer::UserDatas.findKey(ID) ) {
        nav_msgs::OccupancyGrid *map = NULL;
        Neo_Type::UserData *user = NULL;
        CBackServer::UserDatas.get(ID, &user);
        if(user != NULL) {

            map = &user->LoadMapDatas;
            NeoDebug("Received a %d X %d map @ %.3f m/pix\n",
                   map->info.width,
                   map->info.height,
                   map->info.resolution);
            Neo_Config::ConfigParamer *config_ptr = Neo_Config::GetConfigParamer();
            std::string mapname_;
            if(config_ptr != NULL && config_ptr->mapSaveDir_ != ""){
                mapname_ = config_ptr->mapSaveDir_;
            }else{
                mapname_ = "navigation";
            }
            std::string mapdatafile = mapname_ + ".pgm";
            NeoDebug("Writing map occupancy data to %s\n", mapdatafile.c_str());
            FILE *out = fopen(mapdatafile.c_str(), "w");
            if (!out) {
                NeoDebug("Couldn't save map file to %s\n", mapdatafile.c_str());
                return false;
            }

            fprintf(out, "P5\n# CREATOR: Map_generator.cpp %.3f m/pix\n%d %d\n255\n",
                    map->info.resolution, map->info.width, map->info.height);
            for (unsigned int y = 0; y < map->info.height; y++) {
                for (unsigned int x = 0; x < map->info.width; x++) {
                    unsigned int i = x + (map->info.height - y - 1) * map->info.width;
                    if (map->data[i] >= 0 && map->data[i] < 25) { //occ [0,0.25)
                        fputc(254, out);
                    } else if (map->data[i] > 65) { //occ (0.65,1]
                        fputc(000, out);
                    } else { //occ [0.25,0.65]
                        fputc(205, out);
                    }
                }
            }

            fclose(out);


            std::string mapmetadatafile = mapname_ + ".yaml";
            NeoDebug("Writing map occupancy data to %s\n", mapmetadatafile.c_str());
            FILE *yaml = fopen(mapmetadatafile.c_str(), "w");

            /*
        resolution: 0.100000
        origin: [0.000000, 0.000000, 0.000000]
        #
        negate: 0
        occupied_thresh: 0.65
        free_thresh: 0.196

             */

            geometry_msgs::Quaternion orientation = map->info.origin.orientation;
            tf2::Matrix3x3 mat(tf2::Quaternion(
                    orientation.x,
                    orientation.y,
                    orientation.z,
                    orientation.w
            ));
            double yaw, pitch, roll;
            mat.getEulerYPR(yaw, pitch, roll);

            fprintf(yaml,
                    "image: %s\nresolution: %f\norigin: [%f, %f, %f]\nnegate: 0\noccupied_thresh: 0.65\nfree_thresh: 0.196\n\n",
                    mapdatafile.c_str(), map->info.resolution, map->info.origin.position.x, map->info.origin.position.y,
                    yaw);

            fclose(yaml);
            return true;
        }
    }
    return false;
}

void CLoadMapTask::doAction() {
    unsigned char response_buf[800] = {0};
    Neo_Packet::MAPUPLOAD_PACKET_RESPONSE response = {0};
    int Size = 0;
    int len = 0;
    CPacketStream packet;

    if( CBackServer::UserDatas.findKey(head.device_id) ){
        nav_msgs::OccupancyGrid *map_ptr = NULL;
        Neo_Type::UserData *user = NULL;
        CBackServer::UserDatas.get(head.device_id,&user);
        //printf("LoadMap id:%d\n",user->id);
        if(user != NULL) {

            map_ptr = &user->LoadMapDatas;


            if (recv_packet.package_num == 1) {
                map_ptr->data.clear();
            }

            map_ptr->info.height = recv_packet.height;
            map_ptr->info.width = recv_packet.width;
            map_ptr->info.resolution = recv_packet.resolution;
            map_ptr->info.origin.position.x = recv_packet.map_pose.x;
            map_ptr->info.origin.position.y = recv_packet.map_pose.y;
            map_ptr->info.origin.position.z = recv_packet.map_pose.z;
            map_ptr->info.origin.orientation.x = recv_packet.map_pose.Quaternion[0];
            map_ptr->info.origin.orientation.y = recv_packet.map_pose.Quaternion[1];
            map_ptr->info.origin.orientation.z = recv_packet.map_pose.Quaternion[2];
            map_ptr->info.origin.orientation.w = recv_packet.map_pose.Quaternion[3];


            map_ptr->data.insert(map_ptr->data.end(), recv_packet.data, recv_packet.data + recv_packet.data_size);

            response.package_num = recv_packet.package_num + 1;
            response.package_sum = recv_packet.package_sum;
            //printf("sum:%d num:%d\n",ack_package.package_sum,ack_package.package_num);
        }
    }else{
        NeoDebug("No User %d\n",CBackServer::UserDatas.size());
    }





    packet.Packet(response_buf, &Size, this->head.function_id, &response, sizeof(response), head.ref, head.device_id);
    shm_ack->Write((char*)response_buf, Size, fd);

    if( response.package_num > response.package_sum && response.package_sum != 0){
        //NeoDebug("LoadMap end\n");
        SaveMap(head.device_id);
    }
}
