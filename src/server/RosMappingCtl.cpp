//
// Created by huang on 18-2-28.
//

#include <neo_bridge/RosMappingCtl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <neo_bridge/CConfig.h>
#include <neo_bridge/CRosNode.h>

using namespace Neo_Config;

RosMappingCtl::RosMappingCtl():CommandExecutor("") {
    this->isRun = false;
    Neo_Config::ConfigParamer *config_ptr = Neo_Config::GetConfigParamer();
    this->cmd_ = config_ptr->mappingDir_;//"roslaunch micvision_mapping mapping_neo.launch";
    this->cmd_kill_ = config_ptr->mappingKillDir_;//"bash ~/.neoware/bashfile/mapping_kill.sh";
}

RosMappingCtl::~RosMappingCtl() {

}

bool RosMappingCtl::Done() {
    if(isRun)return true;
    if(!CRosNode::waitForUnAction()) {
        isRun = true;
        return true;
    }
    printf("Start Mapping\n");
    this->pid = CmdProcessOpen(this->cmd_.c_str(),NULL);
    printf("success %d\n",this->pid);

    if(!CRosNode::waitForAction()){
        return false;
    }

    isRun = true;
    return true;
}

bool RosMappingCtl::isRunning() {
    return isRun;
}

bool RosMappingCtl::Kill() {
    if(!isRun)return true;
    if(!CRosNode::waitForAction()) {
        isRun = false;
        return true;
    }
    this->pid = CmdProcessOpen(this->cmd_kill_.c_str(),NULL);

    if(!CRosNode::waitForUnAction()){
        return false;
    }

    isRun = false;
    return true;
}
