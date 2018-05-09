//
// Created by huang on 18-2-28.
//

#ifndef NEO_BRIDGE_ROSNAVIGATIONCTL_H
#define NEO_BRIDGE_ROSNAVIGATIONCTL_H
#include <neo_bridge/CCommandExecutor.h>
#include <neo_bridge/CDebug.h>

class RosNavigationCtl:public CommandExecutor
{
private:
    bool isRun;

public:
    RosNavigationCtl();
    ~RosNavigationCtl();

    bool Done();
    bool isRunning();
    bool Kill();
};
#endif //NEO_BRIDGE_ROSNAVIGATIONCTL_H
