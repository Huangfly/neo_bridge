//
// Created by huang on 18-5-3.
//

#ifndef NEO_BRIDGE_TYPEPACKET_CANCELGOAL_H
#define NEO_BRIDGE_TYPEPACKET_CANCELGOAL_H


namespace Neo_Packet{
    typedef struct {
        char isAck;
    }CANCELGOAL_PACKAGE_POP;

    typedef struct {
        char ack;
    }CANCELGOAL_PACKAGE_ACK;
}

#endif //NEO_BRIDGE_TYPEPACKET_CANCELGOAL_H
