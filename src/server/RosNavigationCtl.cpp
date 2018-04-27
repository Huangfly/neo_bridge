//
// Created by huang on 18-2-28.
//
#include <neo_bridge/RosNavigationCtl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <neo_bridge/CConfig.h>
#include <neo_bridge/CRosNode.h>

using namespace Neo_Config;

RosNavigationCtl::RosNavigationCtl():CommandExecutor("") {
    this->isRun = false;
    Neo_Config::ConfigParamer *config_ptr = Neo_Config::GetConfigParamer();
    this->cmd_ = config_ptr->navigationDir_;
    this->cmd_kill_ = config_ptr->navigationKillDir_;
}

RosNavigationCtl::~RosNavigationCtl() {

}

bool RosNavigationCtl::Done() {
    if(isRun)return true;
    printf("Start Navigation\n");
    this->pid = CmdProcessOpen(this->cmd_.c_str(),NULL);

    int count = 100;
    while(count > 0){
        count--;
        if(CRosNode::IsAction()){
            break;
        }
        usleep(50000);
    }
    if(count == 0){
        return false;
    }

    isRun = true;
    return true;
}

int RosNavigationCtl::ReturnValue() {

    return 1;
}

bool RosNavigationCtl::Kill() {
    if(!isRun)return true;
    CmdProcessOpen((char *) this->cmd_kill_.c_str(),NULL);

    int count = 100;
    while(count > 0){
        count--;
        if(!CRosNode::IsAction()){
            break;
        }
        usleep(50000);
    }
    if(count == 0){
        return false;
    }

    printf("Stop navigation\n");
    isRun = false;
    return true;
}
