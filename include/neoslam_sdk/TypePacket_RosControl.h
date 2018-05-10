//
// Created by huang on 18-5-3.
//

#ifndef NEO_BRIDGE_TYPE_ROSCTL_HPP
#define NEO_BRIDGE_TYPE_ROSCTL_HPP

namespace Neo_Packet{
    typedef struct {
        char enable;//0 enable  1 unable
        char node_name[20];
    }ROSCONTROL_PACKET_REQUEST;

    typedef struct {
        char ack[10];
    }ROSCONTROL_PACKET_RESPONSE;
}

#endif //NEO_BRIDGE_TYPE_ROSCTL_HPP
