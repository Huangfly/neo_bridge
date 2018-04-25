//
// Created by huang on 18-4-24.
//

#include "RosLocationCtl.h"

RosLocationCtl::RosLocationCtl():CommandExecutor("") {
    this->isRun = false;
    this->cmd_ = "rosservice call /StartLocation";
}

RosLocationCtl::~RosLocationCtl() {

}

bool RosLocationCtl::Done() {
    //if(isRun)return true;
    printf("Start Location\n");
    //this->pid = CmdProcessOpen(this->cmd_.c_str(),"~/log/mapping_neo.log");
    int ret = system(this->cmd_.c_str());
    //popen("rostopic pub /sim_ctl std_msgs/String \"data: 'robot|run'\" ","r");
    printf("status %d\n",ret);
    //isRun = true;
    return true;
}

int RosLocationCtl::ReturnValue() {
    return 0;
}

bool RosLocationCtl::Kill() {
    if(!isRun)return true;

    isRun = false;
    return true;
}
