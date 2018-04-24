#include "CUnpackTask.h"
#include "map.h"
#include "CBackServer.h"
#include "CRosNode.h"
#include "robot_status.h"
#include "goal.h"
#include "TaskCancelGoal.h"
#include "TaskNodeCtl.h"
#include <neo_bridge/TaskCmdVel.h>
#include <neo_bridge/TaskLidar.h>
#include <neo_bridge/TaskLoadMap.h>
#include <neoslam_sdk/mode.h>

CUnpackTask::CUnpackTask(char *bus_buf, int Len, int fd, CThreadPool *busctl_pool)
:CTask()
{
	this->buf = new char[Len];
	this->Len = Len;
	memcpy(buf, bus_buf, Len);
	this->busctl_pool = busctl_pool;
	this->fd = fd;
}

CUnpackTask::~CUnpackTask()
{
	delete []buf;
}

void CUnpackTask::  doAction()
{
	P_HEAD head = {0};

	memcpy(&head, buf, sizeof(P_HEAD));
	//printf("pack id:%d  fd:%d\n",head.funcId,fd);
	//DEBUG_LOG((unsigned char*)buf,Len);

	if( !CBackServer::UserDatas.findKey(head.msg_code) ){
        printf("add user %d\n",head.msg_code);
        Neo_Type::UserData user(head.msg_code);
        CBackServer::UserDatas.set(head.msg_code, user);
        printf("size:%d\n",CBackServer::UserDatas.size());
	}

	switch(head.funcId)
	{
		case PACK_HEARD:
			busctl_pool->addTask( new CRobotStatusTask(fd,&head,(buf+ sizeof(P_HEAD)), sizeof(STATUS_PACKAGE_POP)) );
			break;
        case PACK_MAP:
            busctl_pool->addTask( new CMapTask(fd,&head,(buf+sizeof(P_HEAD)), sizeof(MAP_PACKAGE_POP)) );
            break;
		case PACK_GOAL:
			busctl_pool->addTask( new CGoalTask(fd,&head,(buf+sizeof(P_HEAD)), sizeof(GOAL_PACKAGE_POP)) );
			break;
		case PACK_CANCELGOAL:
			busctl_pool->addTask( new CCancelGoalTask(fd,&head,(buf+sizeof(P_HEAD)), sizeof(CANCELGOAL_PACKAGE_POP)) );
			break;
        case PACK_NODECTL:
            busctl_pool->addTask( new CNodeCtlTask(fd,&head,(buf+sizeof(P_HEAD)), sizeof(NODECTL_PACKAGE_POP)) );
            break;
		case PACK_CMDVEL:
			busctl_pool->addTask( new CCmdVelTask(fd,&head,(buf+sizeof(P_HEAD)), sizeof(CMDVEL_PACKAGE_POP)) );
			break;
		case PACK_LIDAR:
			busctl_pool->addTask( new CLidarTask(fd,&head,(buf+sizeof(P_HEAD)), sizeof(LIDAR_PACKAGE_POP)) );
			break;
        case PACK_LOADMAP:
            busctl_pool->addTask( new CLoadMapTask(fd,&head,(buf+sizeof(P_HEAD)), sizeof(LOADMAP_PACKAGE_POP)) );
	default:
		break;
	}
}
