//
// Created by huang on 18-2-28.
//
#include <neo_bridge/RosNavigationCtl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <neo_bridge/CConfig.h>

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
    printf("asdas111\n");
    this->pid = CmdProcessOpen(this->cmd_.c_str(),NULL);
    //popen("rostopic pub /sim_ctl std_msgs/String \"data: 'robot|run'\" ","r");
    printf("success %d\n",this->pid);
    isRun = true;
    return true;
}

int RosNavigationCtl::ReturnValue() {
    std::string str = "";
    FILE* fp = popen("rostopic info /map","r");
    char ch;
    if(fp == NULL)return 0;

    while( (ch = fgetc(fp)) > 0)
    {
        if(ch <= 0)break;

        str.push_back(ch);
    }
    if (str.find("ERROR") == std::string::npos){
        return 0;
    }
    //printf("--------------\n%s\n",str.c_str());
    pclose(fp);
    return 1;
}

bool RosNavigationCtl::Kill() {
    if(!isRun)return true;
    //this->pid = CmdProcessOpen(this->cmd_.c_str(),NULL);
    CmdProcessOpen((char *) this->cmd_kill_.c_str(),NULL);
    //popen("rostopic pub /sim_ctl std_msgs/String \"data: 'robot|kill'\" ","r");
    printf("Stop navigation\n");
    isRun = false;
    return true;
}
