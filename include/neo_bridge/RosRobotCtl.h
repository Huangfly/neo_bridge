//
// Created by huang on 18-2-28.
//

#ifndef NEO_BRIDGE_ROSROBOTCTL_H
#define NEO_BRIDGE_ROSROBOTCTL_H

#include <neo_bridge/CCommandExecutor.h>

class RosRobotCtl:public CommandExecutor
{
private:
    bool isRun;
public:
    RosRobotCtl();
    ~RosRobotCtl();

    bool Done();
    int ReturnValue();
    bool Kill();
};

#endif //NEO_BRIDGE_ROSROBOTCTL_H
