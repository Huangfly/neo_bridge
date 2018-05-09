//
// Created by huang on 18-4-20.
//

#ifndef NEO_BRIDGE_TYPE_LOADMAP_H
#define NEO_BRIDGE_TYPE_LOADMAP_H

#define LOADMAP_SIZE 512
#include "Type_Pose.h"

namespace Neo_Packet {
    typedef struct {
        unsigned int serial_num;
        unsigned int package_sum;// map package total sum.
        unsigned int package_num;// map package num.
        Neo_Type::POSE map_pose;
        unsigned int width;
        unsigned int height;
        float resolution;
        unsigned int data_size;
        unsigned char data[LOADMAP_SIZE];
    } LOADMAP_PACKAGE_POP;


    typedef struct {
        unsigned int serial_num;
        unsigned int package_sum;// map package total sum.
        unsigned int package_num;// map package num.
    } LOADMAP_PACKAGE_ACK;
}

#endif //NEO_BRIDGE_TYPE_LOADMAP_H
