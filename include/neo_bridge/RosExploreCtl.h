//
// Created by huang on 18-3-15.
//

#ifndef NEO_BRIDGE_ROSEXPLORECTL_H
#define NEO_BRIDGE_ROSEXPLORECTL_H

#include "CCommandExecutor.h"

class RosExploreCtl:public CommandExecutor
{
private:
    bool isRun;
public:
    RosExploreCtl();
    ~RosExploreCtl();

    bool Done();
    int ReturnValue();
    bool Kill();
};

#endif //NEO_BRIDGE_ROSEXPOLECTL_H
