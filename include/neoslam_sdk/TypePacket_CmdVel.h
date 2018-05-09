//
// Created by huang on 18-3-19.
//

#ifndef NEO_BRIDGE_TYPE_CMDVEL_H
#define NEO_BRIDGE_TYPE_CMDVEL_H
namespace Neo_Packet {
    typedef struct {
        float x;
        float y;
        float z;
    } CMDVEL_PACKAGE_POP;

    typedef struct {
        char ack;
    } CMDVEL_PACKAGE_ACK;
}
#endif //NEO_BRIDGE_TYPE_CMDVEL_H
