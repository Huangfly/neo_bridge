//
// Created by huang on 17-12-27.
//

#ifndef NEO_BRIDGE_CMAPPINGNODE_H
#define NEO_BRIDGE_CMAPPINGNODE_H

#include "CThreadBase.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <list>
#include <vector>

class CMappingNode:public CThreadBase
{
public:
    CMappingNode();
    ~CMappingNode(){

    };
    void Run();


    //static bool is
};
#endif //NEO_BRIDGE_CMAPPINGNODE_H
