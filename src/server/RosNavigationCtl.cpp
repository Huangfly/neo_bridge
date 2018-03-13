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
    printf("Start Navigation\n");
    this->pid = CmdProcessOpen("roslaunch micvision_mapping mapping_turtlebot2.launch","mapping_turtlebot2.log");
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
    killProcess(this->pid);
    //popen("rostopic pub /sim_ctl std_msgs/String \"data: 'robot|kill'\" ","r");
    isRun = false;
    return true;
}
