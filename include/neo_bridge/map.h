#ifndef _MAP_H_
#define _MAP_H_

#include "CThreadPool.h"
#include "CShareMem.h"
#include "packet.h"
#include "config.h"
#include "robot_status.h"
#include <ros/ros.h>
#include <nav_msgs/OccupancyGrid.h>


typedef struct {
    unsigned int package_sum;// map package total sum.
    unsigned int package_num;// map package num.
}MAP_PACKAGE_POP;

typedef struct {
    unsigned int package_sum;// map package total sum.
    unsigned int package_num;// map package num.
    float resolution;
    float x;
    float y;
    float z;
    float Quaternion[4];
    unsigned int size;
    unsigned int width;
    unsigned int hight;
    char data[512];
}MAP_PACKAGE_ACK;



class CMapTask:public CTask
{
private:

    packet_head head;
    MAP_PACKAGE_POP package_pop;
    int fd;
public:
#ifdef USE_ROS
    CMapTask(int fd, P_HEAD *bus_head, char *buf, int Len);
    nav_msgs::OccupancyGrid *map_ptr;
#else
    CMapTask(int fd, P_HEAD *bus_head, char *buf, int Len);
#endif
    ~CMapTask();
    void doAction();
};

#endif
