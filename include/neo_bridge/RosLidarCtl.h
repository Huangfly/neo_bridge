//
// Created by huang on 18-2-28.
//

#ifndef NEO_BRIDGE_ROSLIDARCTL_H
#define NEO_BRIDGE_ROSLIDARCTL_H

#include <neo_bridge/CCommandExecutor.h>

class RosLidarCtl:public CommandExecutor
{
private:
    bool isRun;
public:
    RosLidarCtl();
    ~RosLidarCtl();

    bool Done();
    int ReturnValue();
    bool Kill();
};

#endif //NEO_BRIDGE_ROSLIDARCTL_H
