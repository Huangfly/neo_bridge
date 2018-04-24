//
// Created by huang on 18-4-24.
//

#ifndef NEO_BRIDGE_ROSLOCATIONCTL_H
#define NEO_BRIDGE_ROSLOCATIONCTL_H


#include <neo_bridge/CCommandExecutor.h>

class RosLocationCtl:public CommandExecutor
{
private:
    bool isRun;
public:
    RosLocationCtl();
    ~RosLocationCtl();

    bool Done();
    int ReturnValue();
    bool Kill();
};

#endif //NEO_BRIDGE_ROSLOCATIONCTL_H
