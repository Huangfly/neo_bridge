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
    if(!CRosNode::waitForUnAction()) {
        isRun = true;
        return true;
    }
    printf("Start Navigation\n");
    this->pid = CmdProcessOpen(this->cmd_.c_str(),NULL);

    if(!CRosNode::waitForAction()){
        return false;
    }

    isRun = true;
    return true;
}

bool RosNavigationCtl::isRunning() {

    return isRun;
}

bool RosNavigationCtl::Kill() {
    if(!isRun)return true;

    if(!CRosNode::waitForAction()) {
        isRun = false;
        return true;
    }

    CmdProcessOpen((char *) this->cmd_kill_.c_str(),NULL);
    printf("Stop navigation\n");

    if(!CRosNode::waitForUnAction()){
        return false;
    }

    isRun = false;
    return true;
}
