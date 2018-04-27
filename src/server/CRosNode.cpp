//
// Created by huang on 17-12-7.
//

#include <neo_bridge/robot_status.h>
#include <neo_bridge/CRosNode.h>
#include "CRosNode.h"
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include <neo_bridge/CConfig.h>

using namespace Neo_Config;

extern bool systerm_exit;
extern CShareMem shm_bus;
static geometry_msgs::PoseStamped st_MoveGoal;
static geometry_msgs::PoseStamped st_InitPose;
static actionlib_msgs::GoalID st_CancelGoal;
static ROS_PUB_FLAG st_pub_flag;
nav_msgs::OccupancyGrid CRosNode::map_base_ = nav_msgs::OccupancyGrid();
sensor_msgs::LaserScan CRosNode::scan_ = sensor_msgs::LaserScan();
nav_msgs::Path CRosNode::path_ = nav_msgs::Path();
STATUS_PACKAGE_ACK CRosNode::robot_status = {0};
STATUS_PACKAGE_ACK CRosNode::laser_pose = {0};
bool CRosNode::isAction = false;

ros::Publisher  pub_cmdVel_;

pthread_mutex_t *mutex_update_Path = NULL;


CRosNode::CRosNode()
        :nh("~")
        ,nhp("~")
{
    std::string config_name;
    if( !nh.getParam("config_lua", config_name) ){
        Neo_Config::CConfig config;
        //config.LoadFille(config_name.c_str());
    }

    Neo_Config::ConfigParamer *config_ptr = Neo_Config::GetConfigParamer();

    if( config_ptr == NULL ){
        printf("error: no found config file.\n");
        return;
    }

    sub_map_ = nh.subscribe(config_ptr->mapTopic_,0.01,&CRosNode::cbMap, this);
    sub_scan_ = nh.subscribe(config_ptr->scanTopic_,10,&CRosNode::cbScan,this);
    sub_moveStatus_ = nh.subscribe("/move_base/status",5,&CRosNode::cbMoveStatus, this);
    sub_path_ = nh.subscribe(config_ptr->pathTopic_,10,&CRosNode::cbPath, this);

    pub_goal_ = nh.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal",1);
    pub_cancelGoal_ = nh.advertise<actionlib_msgs::GoalID>("/move_base/cancel",1);

    pub_cmdVel_ = nh.advertise<geometry_msgs::Twist>(config_ptr->cmdvelTopic_,10);

    tf_listener = new tf::TransformListener(nh);
    //m_pub_flag = {0};
    st_pub_flag.isPub_Goal = false;
    st_pub_flag.isPub_InitPose = false;
    st_pub_flag.isPub_CancelGoal = false;

    if(mutex_update_Path == NULL){
        mutex_update_Path = new pthread_mutex_t;
        pthread_mutex_init(mutex_update_Path,NULL);
    }
}

void CRosNode::Run()
{
    //tf::TransformListener tf_listener;

    Neo_Config::ConfigParamer *config_ptr = Neo_Config::GetConfigParamer();

    while (ros::ok() && config_ptr != NULL )
    {
        tf::StampedTransform transform_map_odom;
        tf::StampedTransform transform_map_laser;
        if(systerm_exit)break;
        if(st_pub_flag.isPub_Goal) {
            st_pub_flag.isPub_Goal = false;
            st_MoveGoal.header.stamp = ros::Time::now();
            st_MoveGoal.header.frame_id = config_ptr->mapFrameId_;
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
            tf_listener->lookupTransform(config_ptr->mapFrameId_,config_ptr->robotFrameId_,ros::Time(0.),transform_map_odom);

            tf_listener->lookupTransform(config_ptr->mapFrameId_,config_ptr->scanFrameId_,ros::Time(0.),transform_map_laser);

            //printf("----------------------- tf\n");
            isAction = true;
            //printf("x:%f\ny:%f\n",robot_status.x,robot_status.y);
            //printf("x:%f\ny:%f\nz:%f\nw:%f\n",robot_status.Quaternion[0] ,robot_status.Quaternion[1] ,robot_status.Quaternion[2] ,robot_status.Quaternion[3]);
            //tf_listener.waitForTransform("map","odom",ros::Time(0.1));
        }catch (tf::TransformException &ex){
            //printf("error tf\n");
            isAction = false;
        }
        if(isAction){
            robot_status.x = transform_map_odom.getOrigin().getX();
            robot_status.y = transform_map_odom.getOrigin().getY();
            robot_status.z = transform_map_odom.getOrigin().getZ();
            robot_status.Quaternion[0] = transform_map_odom.getRotation().getX();
            robot_status.Quaternion[1] = transform_map_odom.getRotation().getY();
            robot_status.Quaternion[2] = transform_map_odom.getRotation().getZ();
            robot_status.Quaternion[3] = transform_map_odom.getRotation().getW();

            laser_pose.x = transform_map_laser.getOrigin().getX();
            laser_pose.y = transform_map_laser.getOrigin().getY();
            laser_pose.z = transform_map_laser.getOrigin().getZ();
            laser_pose.Quaternion[0] = transform_map_laser.getRotation().getX();
            laser_pose.Quaternion[1] = transform_map_laser.getRotation().getY();
            laser_pose.Quaternion[2] = transform_map_laser.getRotation().getZ();
            laser_pose.Quaternion[3] = transform_map_laser.getRotation().getW();
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
    //pthread_mutex_lock(mutex_update_Path);
    this->scan_ = msg;
    //pthread_mutex_lock(mutex_update_Path);
    //printf("scan:%d\n",this->scan_.ranges.size());
}



void CRosNode::cbPath(const nav_msgs::Path &msg){
    pthread_mutex_lock(mutex_update_Path);
    this->path_ = msg;
    pthread_mutex_unlock(mutex_update_Path);
    //printf("get path.%d\n",(unsigned int)this->path_.poses.size());
}

void CRosNode::GetGlobalPath(nav_msgs::Path &out){
    pthread_mutex_lock(mutex_update_Path);
    out = CRosNode::path_;
    pthread_mutex_unlock(mutex_update_Path);
}

bool CRosNode::IsAction(){
    return CRosNode::isAction;
}



#endif