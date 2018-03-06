//
// Created by huang on 18-2-28.
//
#include <neo_bridge/RosNavigationCtl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>


RosNavigationCtl::RosNavigationCtl():CommandExecutor("") {
    this->isRun = false;
}

RosNavigationCtl::~RosNavigationCtl() {

}

bool RosNavigationCtl::Done() {
    if(isRun)return true;
    //CmdProcessOpen("roslaunch ","cartographer.log");
    //popen("rostopic pub /sim_ctl std_msgs/String \"data: 'robot|run'\" ","r");
    isRun = true;
    return true;
}

int RosNavigationCtl::ReturnValue() {
    std::string str = "";
    FILE* fp = popen("rostopic info /move_base","r");
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

bool RosNavigationCtl::Kill() {
    if(!isRun)return true;
    //popen("rostopic pub /sim_ctl std_msgs/String \"data: 'robot|kill'\" ","r");
    isRun = false;
    return true;
}