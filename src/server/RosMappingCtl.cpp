//
// Created by huang on 18-2-28.
//

#include <neo_bridge/RosMappingCtl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>


RosMappingCtl::RosMappingCtl():CommandExecutor("") {
    this->isRun = false;
    this->cmd_ = "bash ~/.neoware/bashfile/mapping.sh";//"roslaunch micvision_mapping mapping_neo.launch";
    this->cmd_kill_ = "bash ~/.neoware/bashfile/mapping_kill.sh";
}

RosMappingCtl::~RosMappingCtl() {

}

bool RosMappingCtl::Done() {
    if(isRun)return true;
    printf("Start Mapping\n");
    this->pid = CmdProcessOpen(this->cmd_.c_str(),NULL);
    //popen("rostopic pub /sim_ctl std_msgs/String \"data: 'robot|run'\" ","r");
    printf("success %d\n",this->pid);
    isRun = true;
    return true;
}

int RosMappingCtl::ReturnValue() {
    std::string str = "";
    FILE* fp = popen("rostopic info /map","r");
    char ch;
    if(fp == NULL)return 0;

    while( (ch = fgetc(fp)) > 0)
    {
        if(ch <= 0)break;

        str.push_back(ch);
    }
    //printf("--------------\n%s\n",str.c_str());
    pclose(fp);
    return 1;
}

bool RosMappingCtl::Kill() {
    if(!isRun)return true;
    this->pid = CmdProcessOpen(this->cmd_kill_.c_str(),NULL);
    //popen("rostopic pub /sim_ctl std_msgs/String \"data: 'robot|kill'\" ","r");
    isRun = false;
    return true;
}
