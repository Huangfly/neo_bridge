//
// Created by huang on 18-3-20.
//

#ifndef NEO_BRIDGE_TASKLIDAR_H
#define NEO_BRIDGE_TASKLIDAR_H

#include "CThreadPool.h"
#include "CShareMem.h"
#include "packet.h"
#include <neoslam_sdk/TypePacket_LidarDatas.h>
#include <vector>
#include <neo_bridge/CDebug.h>

class CLidarTask:public CTask
{
private:

    Neo_Packet::HEAD head;
    Neo_Packet::LIDARDATAS_PACKET_REQUEST package_recv;
    CShareMem *shm_ack;
    int fd;
public:

    CLidarTask(int fd, Neo_Packet::HEAD *bus_head, char *buf, int Len);
    ~CLidarTask();
    void doAction();
    void pushScan( float* rangs, std::vector<float> vec, int inset_num, int size );



};
#endif //NEO_BRIDGE_TASKLIDAR_H
