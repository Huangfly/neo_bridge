//
// Created by huang on 17-12-26.
//

#ifndef NEO_BRIDGE_CROSNODEMANAGER_H
#define NEO_BRIDGE_CROSNODEMANAGER_H

#include "CThreadBase.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <list>
#include <vector>

#define NODE_CLOCE      0
#define NODE_CLOCEING   1
#define NODE_OPEN       2
#define NODE_OPENNING   3
#define NODE_UNKNOW     4

typedef struct {
    char enable;//1 enable 0 unable
    char status;//
    string node_name;
    string node_bashname;
}NODE_INFO;

class CRosNodeManager:public CThreadBase
{
public:
    CRosNodeManager();
    //~CRosNodeManager();
    void Run();
    void getNodeList();
    void runNode(NODE_INFO node);
    bool killNode(NODE_INFO node);
    void setStatus(NODE_INFO *info, char status);

    static bool funcCtlNode(std::string node_str,char enable);


    vector<string> nodeList;
    static vector<NODE_INFO> ctlNodeList;


    //static bool is
};

#endif //NEO_BRIDGE_CROSNODEMANAGER_H
