//
// Created by huang on 18-2-28.
//


#include <neo_bridge/RosLidarCtl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>

RosLidarCtl::RosLidarCtl():CommandExecutor("") {
    this->isRun = false;
}

RosLidarCtl::~RosLidarCtl() {

}

bool RosLidarCtl::Done() {
    if(isRun)return true;
    popen("rostopic pub /sim_ctl std_msgs/String \"data: 'lidar|run'\" ","r");
    isRun = true;
    return true;
}

int RosLidarCtl::ReturnValue() {
    std::string str = "";
    FILE* fp = popen("rostopic info /scan","r");
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

bool RosLidarCtl::Kill() {
    if(!isRun)return true;
    popen("rostopic pub /sim_ctl std_msgs/String \"data: 'lidar|kill'\" ","r");
    isRun = false;
    return true;
}