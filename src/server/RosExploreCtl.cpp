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
    int ret = system(this->cmd_.c_str());
    printf("status %d\n",ret);
    isRun = true;
    return true;
}

int RosExploreCtl::ReturnValue() {
    return 1;
}

bool RosExploreCtl::Kill() {
    if(!isRun)return true;
    int ret = system("rosservice call /Stop");
    printf("status %d\n",ret);
    isRun = false;
    return true;
}