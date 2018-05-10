//
// Created by huang on 18-4-26.
//

#ifndef NEO_BRIDGE_TYPE_GLOBALPATH_HPP
#define NEO_BRIDGE_TYPE_GLOBALPATH_HPP

#include "Type_Pose.h"
#define PATH_MAX_SIZE 50

namespace Neo_Packet {
    typedef struct {
        unsigned int serial_num;
    } GLOBALPLAN_PACKET_REQUEST;

    typedef struct {
        unsigned int serial_num;
        unsigned int path_size;
        Neo_Type::POINT path[PATH_MAX_SIZE];
    } GLOBALPLAN_PACKET_RESPONSE;
}

#endif //NEO_BRIDGE_TYPE_GLOBALPATH_HPP
