//
// Created by huang on 18-2-28.
//

#include <neo_bridge/RosRobotCtl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>

RosRobotCtl::RosRobotCtl():CommandExecutor("") {
    this->isRun = false;
}

RosRobotCtl::~RosRobotCtl() {

}

bool RosRobotCtl::Done() {
    if(isRun)return true;
    popen("rostopic pub /sim_ctl std_msgs/String \"data: 'robot|run'\" ","r");
    isRun = true;
    return true;
}

int RosRobotCtl::ReturnValue() {
    std::string str = "";
    FILE* fp = popen("rostopic info /odom","r");
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

bool RosRobotCtl::Kill() {
    if(!isRun)return true;
    popen("rostopic pub /sim_ctl std_msgs/String \"data: 'robot|kill'\" ","r");
    isRun = false;
    return true;
}
