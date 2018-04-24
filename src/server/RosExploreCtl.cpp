//
// Created by huang on 18-3-15.
//
#include <neo_bridge/RosExploreCtl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>


RosExploreCtl::RosExploreCtl():CommandExecutor("") {
    this->isRun = false;
    this->cmd_ = "rosservice call /StartExploration";
}

RosExploreCtl::~RosExploreCtl() {

}

bool RosExploreCtl::Done() {
    if(isRun)return true;
    printf("Start Explore\n");
    //this->pid = CmdProcessOpen(this->cmd_.c_str(),"~/log/mapping_neo.log");
    int ret = system(this->cmd_.c_str());
    //popen("rostopic pub /sim_ctl std_msgs/String \"data: 'robot|run'\" ","r");
    printf("status %d\n",ret);
    isRun = true;
    return true;
}

int RosExploreCtl::ReturnValue() {
    std::string str = "";

    //printf("--------------\n%s\n",str.c_str());
    return 1;
}

bool RosExploreCtl::Kill() {
    if(!isRun)return true;
    //killProcessRosLaunch((char *) this->cmd_.c_str());
    //popen("rostopic pub /sim_ctl std_msgs/String \"data: 'robot|kill'\" ","r");
    int ret = system("rosservice call /Stop");
    isRun = false;
    return true;
}