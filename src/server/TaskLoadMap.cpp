//
// Created by huang on 18-4-20.
//

#include <neoslam_sdk/Type_LoadMap.h>
#include <neoslam_sdk/Type_Pose.h>
#include "TaskLoadMap.h"
#include "CBackServer.h"
#include "neoslam_sdk/mode.h"
#include "tf2/LinearMath/Matrix3x3.h"

extern CShareMem shm_ack;

CLoadMapTask::CLoadMapTask(int fd, P_HEAD *bus_head, char *buf, int Len) {
    memcpy(&this->head,bus_head,sizeof(P_HEAD));
    memcpy(&this->recv_packet,buf,Len);
    this->fd = fd;
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
            printf("Received a %d X %d map @ %.3f m/pix\n",
                   map->info.width,
                   map->info.height,
                   map->info.resolution);

            std::string mapname_ = "/home/huang/.neoware/maps/navigation";
            std::string mapdatafile = mapname_ + ".pgm";
            printf("Writing map occupancy data to %s\n", mapdatafile.c_str());
            FILE *out = fopen(mapdatafile.c_str(), "w");
            if (!out) {
                printf("Couldn't save map file to %s\n", mapdatafile.c_str());
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
            printf("Writing map occupancy data to %s\n", mapmetadatafile.c_str());
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
    unsigned char ack_buf[800] = {0};
    Neo_Type::LOADMAP_PACKAGE_ACK ack_package = {0};
    int Size = 0;
    int len = 0;
    CPacketStream packet;

    if( CBackServer::UserDatas.findKey(head.msg_code) ){
        nav_msgs::OccupancyGrid *map_ptr = NULL;
        Neo_Type::UserData *user = NULL;
        CBackServer::UserDatas.get(head.msg_code,&user);
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

            ack_package.package_num = recv_packet.package_num + 1;
            ack_package.package_sum = recv_packet.package_sum;
            //printf("sum:%d num:%d\n",ack_package.package_sum,ack_package.package_num);
        }
    }else{
        printf("No User %d\n",CBackServer::UserDatas.size());
    }





    packet.Packet(ack_buf, &Size, PACK_LOADMAP, &ack_package, sizeof(LOADMAP_PACKAGE_ACK), head.trans_id, head.msg_code);
    shm_ack.Write((char*)ack_buf, Size, fd);

    if( ack_package.package_num > ack_package.package_sum && ack_package.package_sum != 0){
        printf("LoadMap end\n");
        SaveMap(head.msg_code);
    }
}
