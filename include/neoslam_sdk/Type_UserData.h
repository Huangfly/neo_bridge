//
// Created by huang on 18-4-20.
//

#ifndef NEO_BRIDGE_TYPE_USERDATA_H
#define NEO_BRIDGE_TYPE_USERDATA_H

#include "Type_Pose.h"
#include <sensor_msgs/LaserScan.h>
#include <nav_msgs/OccupancyGrid.h>
#include <nav_msgs/Path.h>

namespace Neo_Type{
    class UserData{
    public:
        UserData(){}
        UserData(unsigned int id){
            this->id = id;
            ScanDatas_SerialNum = 0;
            MapDatas_SerialNum = 0;
        }
        UserData(const UserData &_copy){
            this->id = _copy.id;
            ScanDatas_SerialNum = 0;
            MapDatas_SerialNum = 0;
            printf("add user ID:%d\n",id);
        }
        ~UserData(){}
        Neo_Type::POSE m_RobotPose;
        Neo_Type::POSE m_LaserPose;
        unsigned int id;

        unsigned long ScanDatas_SerialNum;
        sensor_msgs::LaserScan ScanDatas;

        nav_msgs::OccupancyGrid LoadMapDatas;

        unsigned long MapDatas_SerialNum;
        nav_msgs::OccupancyGrid MapDatas;


        unsigned long GlobalPaths_SerialNum;
        nav_msgs::Path GlobalPaths;

    };
}

#endif //NEO_BRIDGE_TYPE_USERDATA_H
