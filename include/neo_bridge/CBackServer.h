#ifndef _CBACKSERVER_H_
#define _CBACKSERVER_H_

#include "CShareMem.h"
#include "CThreadPool.h"
#include "packet.h"
#include "CUnpackTask.h"
#include "CThreadBase.h"
#include "CRosNode.h"
#include "config.h"
#include "neoslam_sdk/Type_UserData.h"
#include "neoslam_sdk/neo_map.hpp"
#ifdef USE_ROS
#include <ros/ros.h>
#include <nav_msgs/OccupancyGrid.h>
#endif

using namespace Neo;
using namespace Neo_Type;

class CBackServer
{
public:
	CBackServer();
	~CBackServer();
	void Exec(int argc,char **argv);
	static Neo::MutexMap<unsigned int, Neo_Type::UserData> UserDatas;
private:


	CThreadPool *busctl_pool;
};

#endif
