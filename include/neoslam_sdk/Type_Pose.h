//
// Created by huang on 18-4-12.
//

#ifndef NEO_BRIDGE_TYPE_POSE_H
#define NEO_BRIDGE_TYPE_POSE_H

namespace Neo_Type{

    typedef struct {
        float x;
        float y;
    }POINT;

    typedef struct {
       float x;
       float y;
      float z;
      float Quaternion[4];
    }POSE;
}
#endif //NEO_BRIDGE_TYPE_POSE_H
