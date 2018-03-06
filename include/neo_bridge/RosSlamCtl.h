//
// Created by huang on 18-2-28.
//

#ifndef NEO_BRIDGE_ROSSLAMCTL_H
#define NEO_BRIDGE_ROSSLAMCTL_H

#include <neo_bridge/CCommandExecutor.h>

class RosSlamCtl:public CommandExecutor
{
private:
    bool isRun;
public:
    RosSlamCtl();
    ~RosSlamCtl();

    bool Done();
    int ReturnValue();
    bool Kill();
};




#endif //NEO_BRIDGE_ROSSLAMCTL_H
