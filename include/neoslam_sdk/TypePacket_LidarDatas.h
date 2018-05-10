//
// Created by huang on 18-3-20.
//

#ifndef NEO_BRIDGE_TYPE_LIDAR_H
#define NEO_BRIDGE_TYPE_LIDAR_H

#define LIDARDATA_SIZE 100
#include "Type_Pose.h"


namespace Neo_Packet {

    typedef struct {
        unsigned int serial_num;
        unsigned int package_sum;// map package total sum.
        unsigned int package_num;// map package num.
    } LIDARDATAS_PACKET_REQUEST;

    typedef struct {
        unsigned int serial_num;
        unsigned int package_sum;// map package total sum.
        unsigned int package_num;// map package num.
        float angle_min;
        float angle_max;
        float angle_increment;
        Neo_Type::POSE pose;
        unsigned int rangs_size;
        float rangs[LIDARDATA_SIZE];
    } LIDARDATAS_PACKET_RESPONSE;

}
#endif //NEO_BRIDGE_TYPE_LIDAR_H

