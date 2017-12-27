#ifndef _CBACKSERVER_H_
#define _CBACKSERVER_H_

#include "CShareMem.h"
#include "CThreadPool.h"
#include "packet.h"
#include "CUnpackTask.h"
#include "CThreadBase.h"
#include "CRosNode.h"
#include "config.h"
#ifdef USE_ROS
#include <ros/ros.h>
#include <nav_msgs/OccupancyGrid.h>
#endif
class CBackServer
{
public:
	CBackServer();
	~CBackServer();
	void Exec(int argc,char **argv);

private:


	CThreadPool *busctl_pool;
};

#endif
