#include "CUnpackTask.h"
#include "TaskMap.h"
#include "CBackServer.h"
#include "CRosNode.h"
#include "TaskRobotStatus.h"
#include "TaskGoal.h"
#include "TaskCancelGoal.h"
#include "TaskRosControl.h"
#include <neo_bridge/TaskCmdVel.h>
#include <neo_bridge/TaskLidar.h>
#include <neo_bridge/TaskLoadMap.h>
#include <neo_bridge/TaskGlobalPath.h>
#include <neo_bridge/packet.h>

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
	Neo_Packet::HEAD head = {0};

	memcpy(&head, buf, sizeof(Neo_Packet::HEAD));
	//DEBUG_LOG((unsigned char*)buf,Len);

	if( head.device_id >= 0 && head.device_id < 10 && !CBackServer::UserDatas.findKey(head.device_id) ){
        NeoDebug("add user %d\n",head.device_id);
        Neo_Type::UserData user(head.device_id);
        CBackServer::UserDatas.inset(std::pair<int,Neo_Type::UserData>(head.device_id, user));
        //printf("size:%d\n",CBackServer::UserDatas.size());
	}
	Neo_Packet::PacketType type = (Neo_Packet::PacketType) head.function_id;
	switch(type)
	{
		case Neo_Packet::PacketType::ROBOTSTATUS :
			busctl_pool->addTask( new CRobotStatusTask(fd,&head,(buf+ sizeof(Neo_Packet::HEAD)), sizeof(Neo_Packet::ROBOTSTATUS_PACKET_REQUEST)) );
			break;
		case Neo_Packet::PacketType::MAPDATAS :
			busctl_pool->addTask( new CMapTask(fd,&head,(buf+sizeof(Neo_Packet::HEAD)), sizeof(Neo_Packet::MAPDATAS_PACKET_REQUEST)) );
			break;
		case Neo_Packet::PacketType::SENDGOAL :
			busctl_pool->addTask( new CGoalTask(fd,&head,(buf+sizeof(Neo_Packet::HEAD)), sizeof(Neo_Packet::SENDGOAL_PACKET_REQUEST)) );
			break;
		case Neo_Packet::PacketType::CANCELGOAL :
			busctl_pool->addTask( new CCancelGoalTask(fd,&head,(buf+sizeof(Neo_Packet::HEAD)), sizeof(Neo_Packet::CANCELGOAL_PACKET_REQUEST)) );
			break;
		case Neo_Packet::PacketType::ROSCONTROL :
			busctl_pool->addTask( new CRosControlTask(fd,&head,(buf+sizeof(Neo_Packet::HEAD)), sizeof(Neo_Packet::ROSCONTROL_PACKET_REQUEST)) );
			break;
		case Neo_Packet::PacketType::CMDVEL :
			busctl_pool->addTask( new CCmdVelTask(fd,&head,(buf+sizeof(Neo_Packet::HEAD)), sizeof(Neo_Packet::CMDVEL_PACKET_REQUEST)) );
			break;
		case Neo_Packet::PacketType::LIDARDATAS :
			busctl_pool->addTask( new CLidarTask(fd,&head,(buf+sizeof(Neo_Packet::HEAD)), sizeof(Neo_Packet::LIDARDATAS_PACKET_REQUEST)) );
			break;
		case Neo_Packet::PacketType::MAPUPLOAD :
			busctl_pool->addTask( new CLoadMapTask(fd,&head,(buf+sizeof(Neo_Packet::HEAD)), sizeof(Neo_Packet::MAPUPLOAD_PACKET_REQUEST)) );
			break;
		case Neo_Packet::PacketType::GLOBALPLAN :
			busctl_pool->addTask( new CGlobalPathTask(fd,&head,(buf+sizeof(Neo_Packet::HEAD)), sizeof(Neo_Packet::GLOBALPLAN_PACKET_REQUEST)) );
			break;
		default:
			break;
	}
/*	switch(head.function_id)
	{
		case PACK_HEARD:
			busctl_pool->addTask( new CRobotStatusTask(fd,&head,(buf+ sizeof(Neo_Packet::HEAD)), sizeof(Neo_Packet::STATUS_PACKAGE_POP)) );
			break;
        case PACK_MAP:
            busctl_pool->addTask( new CMapTask(fd,&head,(buf+sizeof(Neo_Packet::HEAD)), sizeof(Neo_Packet::MAP_PACKAGE_POP)) );
            break;
		case PACK_GOAL:
			busctl_pool->addTask( new CGoalTask(fd,&head,(buf+sizeof(Neo_Packet::HEAD)), sizeof(Neo_Packet::GOAL_PACKAGE_POP)) );
			break;
		case PACK_CANCELGOAL:
			busctl_pool->addTask( new CCancelGoalTask(fd,&head,(buf+sizeof(Neo_Packet::HEAD)), sizeof(Neo_Packet::CANCELGOAL_PACKAGE_ACK)) );
			break;
        case PACK_NODECTL:
            busctl_pool->addTask( new CRosControlTask(fd,&head,(buf+sizeof(Neo_Packet::HEAD)), sizeof(Neo_Packet::NODECTL_PACKAGE_POP)) );
            break;
		case PACK_CMDVEL:
			busctl_pool->addTask( new CCmdVelTask(fd,&head,(buf+sizeof(Neo_Packet::HEAD)), sizeof(Neo_Packet::CMDVEL_PACKAGE_POP)) );
			break;
		case PACK_LIDAR:
			busctl_pool->addTask( new CLidarTask(fd,&head,(buf+sizeof(Neo_Packet::HEAD)), sizeof(Neo_Packet::LIDAR_PACKAGE_POP)) );
			break;
        case PACK_LOADMAP:
            busctl_pool->addTask( new CLoadMapTask(fd,&head,(buf+sizeof(Neo_Packet::HEAD)), sizeof(Neo_Packet::LOADMAP_PACKAGE_POP)) );
			break;
		case PACK_GLOBALPATH:
			busctl_pool->addTask( new CGlobalPathTask(fd,&head,(buf+sizeof(Neo_Packet::HEAD)), sizeof(Neo_Packet::GLOBALPATH_PACKAGE_POP)) );
			break;
	default:
		break;
	}*/
}
