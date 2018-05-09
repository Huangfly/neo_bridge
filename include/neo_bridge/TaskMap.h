#ifndef _MAP_H_
#define _MAP_H_

#include "CThreadPool.h"
#include "CShareMem.h"
#include "packet.h"
#include "TaskRobotStatus.h"
#include <ros/ros.h>
#include <nav_msgs/OccupancyGrid.h>
#include <neoslam_sdk/TypePacket_MapDatas.h>
#include <neo_bridge/CDebug.h>


class CMapTask:public CTask
{
private:
    Neo_Packet::HEAD head;
    Neo_Packet::MAP_PACKAGE_POP package_pop;
    int fd;
    CShareMem *shm_ack;
public:
    CMapTask(int fd, Neo_Packet::HEAD *bus_head, char *buf, int Len);
    nav_msgs::OccupancyGrid *map_ptr;
    ~CMapTask();
    void doAction();
};

#endif
