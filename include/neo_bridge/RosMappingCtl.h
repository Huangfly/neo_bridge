//
// Created by huang on 18-2-28.
//

#ifndef NEO_BRIDGE_ROSSLAMCTL_H
#define NEO_BRIDGE_ROSSLAMCTL_H

#include <neo_bridge/CCommandExecutor.h>

class RosMappingCtl:public CommandExecutor
{
private:
    bool isRun;
public:
    RosMappingCtl();
    ~RosMappingCtl();

    bool Done();
    int ReturnValue();
    bool Kill();
};




#endif //NEO_BRIDGE_ROSSLAMCTL_H
