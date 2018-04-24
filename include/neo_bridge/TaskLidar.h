//
// Created by huang on 18-3-20.
//

#ifndef NEO_BRIDGE_TASKLIDAR_H
#define NEO_BRIDGE_TASKLIDAR_H

#include "CThreadPool.h"
#include "CShareMem.h"
#include "packet.h"
#include "config.h"
#include <neoslam_sdk/Type_Lidar.h>
#include <vector>

using namespace Neo_Type;

class CLidarTask:public CTask
{
private:

    packet_head head;
    Neo_Type::LIDAR_PACKAGE_POP package_recv;

    int fd;
public:

    CLidarTask(int fd, P_HEAD *bus_head, char *buf, int Len);
    ~CLidarTask();
    void doAction();
    void pushScan( float* rangs, std::vector<float> vec, int inset_num, int size );



};
#endif //NEO_BRIDGE_TASKLIDAR_H
