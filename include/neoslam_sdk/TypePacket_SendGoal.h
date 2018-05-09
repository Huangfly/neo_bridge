//
// Created by huang on 18-5-3.
//

#ifndef NEO_BRIDGE_TYPE_GOAL_HPP
#define NEO_BRIDGE_TYPE_GOAL_HPP

namespace Neo_Packet{
    typedef struct {
        float x;
        float y;
        float z;
        float Quaternion[4];
    }GOAL_PACKAGE_POP;

    typedef struct {
        char isSuccess;
    }GOAL_PACKAGE_ACK;
}

#endif //NEO_BRIDGE_TYPE_GOAL_HPP
