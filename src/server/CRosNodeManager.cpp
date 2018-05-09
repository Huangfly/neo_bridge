//
// Created by huang on 17-12-26.
//
#include <neo_bridge/CRosNodeManager.h>
#include <neo_bridge/RosMappingCtl.h>
#include <neo_bridge/RosNavigationCtl.h>
#include <neo_bridge/CCommandExecutor.h>
#include "CMappingNode.h"
#include <neo_bridge/RosExploreCtl.h>
#include <neo_bridge/RosLocalizationCtl.h>


static bool isActionNode = false;
vector<NODE_INFO> CRosNodeManager::ctlNodeList = vector<NODE_INFO>();
RosMappingCtl  *rosMappingCtl = NULL;
RosNavigationCtl *rosNavigationCtl = NULL;
RosExploreCtl *rosExploreCtl = NULL;
RosLocalizationCtl *rosLocationCtl = NULL;

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


    if(rosMappingCtl == NULL) rosMappingCtl  = new RosMappingCtl();
    if(rosNavigationCtl == NULL) rosNavigationCtl = new RosNavigationCtl();
    if(rosExploreCtl == NULL) rosExploreCtl = new RosExploreCtl();
    if(rosLocationCtl == NULL) rosLocationCtl = new RosLocalizationCtl();
}

void CRosNodeManager::setStatus(NODE_INFO *info, char status)
{
    info->status = status;
}


void CRosNodeManager::Run()
{

    while(1)
    {
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
    //for (int i = 0; i < nodeList.size(); ++i) printf("----%s\n",nodeList[i].c_str());
    return nodeList;
}

bool CRosNodeManager::funcCtlNode(std::string node_str,bool enable)
{
    if( enable ){
        if(node_str == "mapping"){
            if(rosNavigationCtl->isRunning()){
                return false;
            }
            return rosMappingCtl->Done();
        }else if(node_str == "navigation"){
            if(rosMappingCtl->isRunning()){
                return false;
            }
            return rosNavigationCtl->Done();
        }else if(node_str == "explore"){
            return rosExploreCtl->Done();
        }else if(node_str == "location"){
            return rosLocationCtl->Done();
        }else{
            return false;
        }
    }else {
        if(node_str == "mapping"){
            return rosMappingCtl->Kill();
        }else if(node_str == "navigation"){
            return rosNavigationCtl->Kill();
        }else if(node_str == "explore"){
            return rosExploreCtl->Kill();
        }else if(node_str == "location"){
            return rosLocationCtl->Kill();
        }else{
            return false;
        }
    }
    return false;
}
