//
// Created by huang on 18-3-15.
//

#ifndef NEO_BRIDGE_ROSEXPLORECTL_H
#define NEO_BRIDGE_ROSEXPLORECTL_H

#include "CCommandExecutor.h"
#include <neo_bridge/CDebug.h>

class RosExploreCtl:public CommandExecutor
{
private:
    bool isRun;
public:
    RosExploreCtl();
    ~RosExploreCtl();

    bool Done();
    bool isRunning();
    bool Kill();
};

#endif //NEO_BRIDGE_ROSEXPOLECTL_H
