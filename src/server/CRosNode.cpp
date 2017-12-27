//
// Created by huang on 17-12-7.
//

#include <neo_bridge/robot_status.h>
#include <neo_bridge/CRosNode.h>
#include "CRosNode.h"
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
extern bool systerm_exit;
extern CShareMem shm_bus;
static geometry_msgs::PoseStamped st_MoveGoal;
static geometry_msgs::PoseStamped st_InitPose;
static actionlib_msgs::GoalID st_CancelGoal;
static ROS_PUB_FLAG st_pub_flag;
nav_msgs::OccupancyGrid CRosNode::map_base_ = nav_msgs::OccupancyGrid();
STATUS_PACKAGE_ACK CRosNode::robot_status = {0};

CRosNode::CRosNode()
        :nh("~")
        ,nhp("~")
{
    sub_map_ = nh.subscribe("/map",0.01,&CRosNode::cbMap, this);
    sub_odom_ = nh.subscribe("/odom",5,&CRosNode::cbOdom, this);
    sub_moveStatus_ = nh.subscribe("/move_base/status",5,&CRosNode::cbMoveStatus, this);
    pub_goal_ = nh.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal",1);
    pub_cancelGoal_ = nh.advertise<actionlib_msgs::GoalID>("/move_base/cancel",1);

    //m_pub_flag = {0};
    st_pub_flag.isPub_Goal = false;
    st_pub_flag.isPub_InitPose = false;
    st_pub_flag.isPub_CancelGoal = false;
}

void CRosNode::Run()
{
    while (ros::ok())
    {
        if(systerm_exit)break;
        if(st_pub_flag.isPub_Goal) {
            st_pub_flag.isPub_Goal = false;
            st_MoveGoal.header.stamp = ros::Time::now();
            st_MoveGoal.header.frame_id = "map";
            pub_goal_.publish(st_MoveGoal);
            printf("goal...\n");
        }

        if(st_pub_flag.isPub_InitPose) {
            st_pub_flag.isPub_InitPose = false;
            pub_initialPose_.publish(st_InitPose);
        }

        if(st_pub_flag.isPub_CancelGoal) {
            st_pub_flag.isPub_CancelGoal = false;
            pub_cancelGoal_.publish(st_CancelGoal);
            printf("Cancel Move.\n");
        }
        ros::spinOnce();
    }
    printf("CRosNode exit.\n");
    systerm_exit = true;
    shm_bus.Write((char*)&systerm_exit,1,0);
}
#ifdef USE_ROS

void CRosNode::PopMoveGoal(geometry_msgs::PoseStamped goal)
{
    st_MoveGoal = goal;
    st_pub_flag.isPub_Goal = true;
}

void CRosNode::PopInitialPose(geometry_msgs::PoseStamped pose)
{
    st_InitPose = pose;
    st_pub_flag.isPub_InitPose = true;
}

void CRosNode::PopCancelGoal()
{
    st_pub_flag.isPub_CancelGoal = true;
}

void CRosNode::cbMap(const nav_msgs::OccupancyGrid::ConstPtr &msg)
{
    //if(this->map_base_.data.size() != msg->data.size())
    {
        this->map_base_ = *msg;
        robot_status.updateMap = 1;
    }
    //printf("recv Map...%d\n",robot_status.updateMap );
}

void CRosNode::cbOdom(const nav_msgs::Odometry &msg) {
    robot_status.x = msg.pose.pose.position.x;
    robot_status.y = msg.pose.pose.position.y;
    robot_status.z = msg.pose.pose.position.z;
    robot_status.Quaternion[0] = msg.pose.pose.orientation.x;
    robot_status.Quaternion[1] = msg.pose.pose.orientation.y;
    robot_status.Quaternion[2] = msg.pose.pose.orientation.z;
    robot_status.Quaternion[3] = msg.pose.pose.orientation.w;
    //printf("recv odom %d\n",robot_status.updateMap);
}

void CRosNode::cbMoveStatus(const actionlib_msgs::GoalStatusArray &msg) {
    if(msg.status_list.size() > 0)
        robot_status.movebase_status = msg.status_list.begin().base()->status;
    //printf("MoveBaseStatus...%d\n",CRobotStatusTask::robot_status.movebase_status );
}
#endif