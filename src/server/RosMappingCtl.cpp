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
    if( CRosNode::IsAction() ){
        isRun = true;
        return true;
    }
    printf("Start Mapping\n");
    this->pid = CmdProcessOpen(this->cmd_.c_str(),NULL);
    printf("success %d\n",this->pid);

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

int RosMappingCtl::ReturnValue() {
    return 1;
}

bool RosMappingCtl::Kill() {
    if(!isRun)return true;
    if(!CRosNode::IsAction()){
        isRun = false;
        return true;
    }
    this->pid = CmdProcessOpen(this->cmd_kill_.c_str(),NULL);

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

    isRun = false;
    return true;
}
