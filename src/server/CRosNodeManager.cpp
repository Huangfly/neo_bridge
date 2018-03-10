//
// Created by huang on 17-12-26.
//
#include <neo_bridge/CRosNodeManager.h>
#include <neo_bridge/RosRobotCtl.h>
#include <neo_bridge/RosLidarCtl.h>
#include <neo_bridge/RosSlamCtl.h>
#include <neo_bridge/RosNavigationCtl.h>
#include <neo_bridge/CCommandExecutor.h>
#include "CMappingNode.h"

#define WORKSPACE_BASH_DIR "/home/huang/stage_ws"
//#define

static bool isActionNode = false;
vector<NODE_INFO> CRosNodeManager::ctlNodeList = vector<NODE_INFO>();
CMappingNode *mappingNode;
RosRobotCtl *rosRobotCtl = NULL;
RosLidarCtl *rosLidarCtl = NULL;
RosSlamCtl  *rosSlamCtl = NULL;
RosNavigationCtl *rosNavigationCtl = NULL;
CommandExecutor *rosNodeList[5];

std::vector<std::string> split(std::string str,std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str;
    int size=str.size();

    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}

bool macthNode(vector<string> list, std::string str)
{
    for (int i = 0; i < list.size(); ++i) {
        if(list[i] == str) return true;
    }
    return false;
}


CRosNodeManager::CRosNodeManager()
{
    NODE_INFO node = {0};
    nodeList.clear();
    ctlNodeList.clear();

    node.node_bashname = "/mapping";
    node.node_name = "/slam_gmapping";
    ctlNodeList.push_back(node);

    node.node_bashname = "/navigation";
    node.node_name = "/move_base";
    ctlNodeList.push_back(node);

    node.node_bashname = "/amcl";
    node.node_name = "/amcl";
    ctlNodeList.push_back(node);

    rosRobotCtl = new RosRobotCtl();
    rosLidarCtl = new RosLidarCtl();
    rosSlamCtl  = new RosSlamCtl();
    rosNavigationCtl = new RosNavigationCtl();
    //rosRobotCtl->ReturnValue();
}

void CRosNodeManager::setStatus(NODE_INFO *info, char status)
{
    info->status = status;
}


void CRosNodeManager::Run()
{

    while(1)
    {
        getNodeList();

        /*for (int i = 0; i < ctlNodeList.size(); ++i) {
            if( ctlNodeList[i].enable == 1 && false == macthNode(nodeList,ctlNodeList[i].node_name) )
            {
                if(ctlNodeList[i].status == NODE_CLOCE)
                {
                    setStatus(&ctlNodeList[i],NODE_OPENNING);
                    runNode(ctlNodeList[i]);

                    setStatus(&ctlNodeList[i],NODE_OPEN);
                    printf("---------%d\n",ctlNodeList[i].status);
                }
            }
            if( ctlNodeList[i].enable == 0 && true == macthNode(nodeList,ctlNodeList[i].node_name))
            {
                if(ctlNodeList[i].status == NODE_OPEN)
                {
                    setStatus(&ctlNodeList[i],NODE_CLOCEING);
                    killNode(ctlNodeList[i]);
                    setStatus(&ctlNodeList[i],NODE_CLOCE);
                }
            }
        }*/
        char count = 0;
        while(count<=10)
        {
            if(isActionNode)break;
            sleep(1);
            count++;
        }
        isActionNode = false;
    }
}

vector<string>  CRosNodeManager::getNodeList()
{
    string str = "";
    FILE* fp = popen("rosnode list","r");
    char ch;
    if(fp == NULL){
        nodeList.clear();
        return nodeList;
    }

    while( (ch = fgetc(fp)) > 0)
    {
        if(ch <= 0)break;

        str.push_back(ch);
    }

    pclose(fp);

    nodeList = split(str,"\n");
    for (int i = 0; i < nodeList.size(); ++i) printf("----%s\n",nodeList[i].c_str());
    return nodeList;
}

bool CRosNodeManager::funcCtlNode(std::string node_str,bool enable)
{
    if( enable ){
        if (node_str == "robot") {
            return rosRobotCtl->Done();
        }else if(node_str == "lidar"){
            return rosLidarCtl->Done();
        }else if(node_str == "slam"){
            return rosSlamCtl->Done();
        }else if(node_str == "navigation"){
            return rosNavigationCtl->Done();
        }else{
            return false;
        }
    }else {
        if (node_str == "robot") {
            return rosRobotCtl->Kill();
        }else if(node_str == "lidar"){
            return rosLidarCtl->Kill();
        }else if(node_str == "slam"){
            return rosSlamCtl->Kill();
        }else if(node_str == "navigation"){
            return rosNavigationCtl->Kill();
        }else{
            return false;
        }
    }
    //printf("%s fail.\n",node_str.c_str());
    return false;
}
