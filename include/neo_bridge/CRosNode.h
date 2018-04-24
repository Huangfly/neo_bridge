//
// Created by huang on 17-12-7.
//

#ifndef NEO_BRIDGE_CROSNODE_H
#define NEO_BRIDGE_CROSNODE_H

#include "CShareMem.h"
#include "CThreadBase.h"
#include "packet.h"
#include "config.h"
#include "robot_status.h"

#include <ros/ros.h>
#include <ros/time.h>
#include <nav_msgs/OccupancyGrid.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseStamped.h>
#include <actionlib_msgs/GoalStatusArray.h>
#include <sensor_msgs/LaserScan.h>
#include <tf/tf.h>
#include <tf/transform_listener.h>

typedef struct {
    bool isPub_Goal;
    bool isPub_InitPose;
    bool isPub_CancelGoal;
}ROS_PUB_FLAG;

class CRosNode:public CThreadBase
{
public:
    CRosNode();
    void Run();

    static void PopMoveGoal(geometry_msgs::PoseStamped goal);
    static void PopInitialPose(geometry_msgs::PoseStamped pose);
    static void PopCmdVel(float x,float y,float z);
    static void PopCancelGoal();
    void cbMap(const nav_msgs::OccupancyGrid::ConstPtr &msg);
    void cbOdom(const nav_msgs::Odometry &msg);
    void cbMoveStatus(const actionlib_msgs::GoalStatusArray &msg);
    void cbScan(const sensor_msgs::LaserScan &msg);

    ros::NodeHandle nh;
    ros::NodeHandle nhp;
    ros::Subscriber sub_map_;
    ros::Subscriber sub_odom_;
    ros::Subscriber sub_moveStatus_;
    ros::Subscriber sub_scan_;
    ros::Publisher  pub_goal_;
    ros::Publisher  pub_initialPose_;
    ros::Publisher  pub_cancelGoal_;

    tf::TransformListener *tf_listener;

    std::string workdir;
    static nav_msgs::OccupancyGrid map_base_;
    static sensor_msgs::LaserScan scan_;
    static STATUS_PACKAGE_ACK robot_status;
    static STATUS_PACKAGE_ACK laser_pose;
};

#endif //NEO_BRIDGE_CROSNODE_H
