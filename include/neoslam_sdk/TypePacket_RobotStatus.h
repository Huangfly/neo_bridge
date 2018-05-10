//
// Created by huang on 18-5-3.
//

#ifndef NEO_BRIDGE_TYPE_ROBOTSTATUS_HPP
#define NEO_BRIDGE_TYPE_ROBOTSTATUS_HPP

namespace Neo_Packet{
    typedef struct {
        char isAck;
    }ROBOTSTATUS_PACKET_REQUEST;

    typedef struct {
        float x;
        float y;
        float z;
        float Quaternion[4];
        char updateMap;
        char movebase_status;
        char ref[2];
    }ROBOTSTATUS_PACKET_RESPONSE;
}

#endif //NEO_BRIDGE_TYPE_ROBOTSTATUS_HPP
