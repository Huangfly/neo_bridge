//
// Created by huang on 18-4-24.
//

#ifndef NEO_BRIDGE_ROSLOCATIONCTL_H
#define NEO_BRIDGE_ROSLOCATIONCTL_H


#include <neo_bridge/CCommandExecutor.h>
#include <neo_bridge/CDebug.h>

class RosLocalizationCtl:public CommandExecutor
{
private:
    bool isRun;
public:
    RosLocalizationCtl();
    ~RosLocalizationCtl();

    bool Done();
    bool isRunning();
    bool Kill();
};

#endif //NEO_BRIDGE_ROSLOCATIONCTL_H
