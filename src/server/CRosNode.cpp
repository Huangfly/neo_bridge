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
sensor_msgs::LaserScan CRosNode::scan_ = sensor_msgs::LaserScan();
STATUS_PACKAGE_ACK CRosNode::robot_status = {0};
STATUS_PACKAGE_ACK CRosNode::laser_pose = {0};

ros::Publisher  pub_cmdVel_;

CRosNode::CRosNode()
        :nh("~")
        ,nhp("~")
{
    sub_map_ = nh.subscribe("/map",0.01,&CRosNode::cbMap, this);
    sub_scan_ = nh.subscribe("/scan",10,&CRosNode::cbScan,this);
    //sub_odom_ = nh.subscribe("/odom",5,&CRosNode::cbOdom, this);
    sub_moveStatus_ = nh.subscribe("/move_base/status",5,&CRosNode::cbMoveStatus, this);
    pub_goal_ = nh.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal",1);
    pub_cancelGoal_ = nh.advertise<actionlib_msgs::GoalID>("/move_base/cancel",1);
    pub_cmdVel_ = nh.advertise<geometry_msgs::Twist>("/cmd_vel",10);

    tf_listener = new tf::TransformListener(nh);
    //m_pub_flag = {0};
    st_pub_flag.isPub_Goal = false;
    st_pub_flag.isPub_InitPose = false;
    st_pub_flag.isPub_CancelGoal = false;
}

void CRosNode::Run()
{
    //tf::TransformListener tf_listener;



    while (ros::ok())
    {
        tf::StampedTransform transform_map_odom;
        tf::StampedTransform transform_map_laser;
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
        try {
            tf_listener->lookupTransform("map","base_link",ros::Time(0.),transform_map_odom);
            robot_status.x = transform_map_odom.getOrigin().getX();
            robot_status.y = transform_map_odom.getOrigin().getY();
            robot_status.z = transform_map_odom.getOrigin().getZ();
            robot_status.Quaternion[0] = transform_map_odom.getRotation().getX();
            robot_status.Quaternion[1] = transform_map_odom.getRotation().getY();
            robot_status.Quaternion[2] = transform_map_odom.getRotation().getZ();
            robot_status.Quaternion[3] = transform_map_odom.getRotation().getW();
            tf_listener->lookupTransform("map","base_laser_link",ros::Time(0.),transform_map_laser);
            laser_pose.x = transform_map_laser.getOrigin().getX();
            laser_pose.y = transform_map_laser.getOrigin().getY();
            laser_pose.z = transform_map_laser.getOrigin().getZ();
            laser_pose.Quaternion[0] = transform_map_laser.getRotation().getX();
            laser_pose.Quaternion[1] = transform_map_laser.getRotation().getY();
            laser_pose.Quaternion[2] = transform_map_laser.getRotation().getZ();
            laser_pose.Quaternion[3] = transform_map_laser.getRotation().getW();

            //printf("x:%f\ny:%f\n",robot_status.x,robot_status.y);
            //printf("x:%f\ny:%f\nz:%f\nw:%f\n",robot_status.Quaternion[0] ,robot_status.Quaternion[1] ,robot_status.Quaternion[2] ,robot_status.Quaternion[3]);
            //tf_listener.waitForTransform("map","odom",ros::Time(0.1));
        }catch (tf::TransformException &ex){
            //ROS_ERROR("%s",ex.what());
            //ros::spinOnce();
            //usleep(10000);
            //continue;
        }

        ros::spinOnce();
        usleep(10000);
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

void CRosNode::PopCmdVel(float x,float y,float z)
{
    geometry_msgs::Twist cmd_vel;
    cmd_vel.linear.x = x;
    cmd_vel.linear.y = y;
    cmd_vel.angular.z = z;
    pub_cmdVel_.publish(cmd_vel);
}

void CRosNode::PopCancelGoal()
{
    st_pub_flag.isPub_CancelGoal = true;
}

void CRosNode::cbMap(const nav_msgs::OccupancyGrid::ConstPtr &msg)
{
    static ros::Time pre_time = ros::Time::now();
    ros::Time cur_time = ros::Time::now();


    //printf("time %ld %ld\n",cur_time.toNSec(),pre_time.toNSec());
    pre_time = cur_time;
    this->map_base_ = *msg;
    robot_status.updateMap = 1;
    //printf("recv Map...%d\n",robot_status.updateMap );
}

void CRosNode::cbOdom(const nav_msgs::Odometry &msg) {
    /*robot_status.x = msg.pose.pose.position.x;
    robot_status.y = msg.pose.pose.position.y;
    robot_status.z = msg.pose.pose.position.z;
    robot_status.Quaternion[0] = msg.pose.pose.orientation.x;
    robot_status.Quaternion[1] = msg.pose.pose.orientation.y;
    robot_status.Quaternion[2] = msg.pose.pose.orientation.z;
    robot_status.Quaternion[3] = msg.pose.pose.orientation.w;*/
    //printf("recv odom %d\n",robot_status.updateMap);
}

void CRosNode::cbMoveStatus(const actionlib_msgs::GoalStatusArray &msg) {
    if(msg.status_list.size() > 0)
        robot_status.movebase_status = msg.status_list.begin().base()->status;
    //printf("MoveBaseStatus...%d\n",CRobotStatusTask::robot_status.movebase_status );
}

void CRosNode::cbScan(const sensor_msgs::LaserScan &msg) {
    this->scan_ = msg;
    //printf("scan:%d\n",this->scan_.ranges.size());
}

#endif