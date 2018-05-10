//
// Created by huang on 18-5-3.
//

#ifndef NEO_BRIDGE_TYPEPACKET_CANCELGOAL_H
#define NEO_BRIDGE_TYPEPACKET_CANCELGOAL_H


namespace Neo_Packet{
    typedef struct {
        char isAck;
    }CANCELGOAL_PACKET_REQUEST;

    typedef struct {
        char ack;
    }CANCELGOAL_PACKET_RESPONSE;
}

#endif //NEO_BRIDGE_TYPEPACKET_CANCELGOAL_H
