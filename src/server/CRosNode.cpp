//
// Created by huang on 17-12-7.
//
#include <neo_bridge/CRosNode.h>
#include <neo_bridge/TaskRobotStatus.h>
#include "CRosNode.h"
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include <neo_bridge/CConfig.h>
#include <neo_bridge/CBackServer.h>

using namespace Neo_Config;

extern bool systerm_exit;
static geometry_msgs::PoseStamped st_MoveGoal;
static geometry_msgs::PoseStamped st_InitPose;
static actionlib_msgs::GoalID st_CancelGoal;
static ROS_PUB_FLAG st_pub_flag;
nav_msgs::OccupancyGrid CRosNode::map_base_ = nav_msgs::OccupancyGrid();
sensor_msgs::LaserScan CRosNode::scan_ = sensor_msgs::LaserScan();
nav_msgs::Path CRosNode::path_ = nav_msgs::Path();
Neo_Packet::ROBOTSTATUS_PACKET_RESPONSE CRosNode::robot_status = {0};
Neo_Packet::ROBOTSTATUS_PACKET_RESPONSE CRosNode::laser_pose = {0};
bool CRosNode::isAction = false;

ros::Publisher  pub_cmdVel_;

pthread_mutex_t *mutex_update_Path = NULL;
pthread_mutex_t *mutex_update_Map = NULL;
pthread_mutex_t *mutex_update_Lidar = NULL;

CRosNode::CRosNode()
        :nh("~")
        ,nhp("~")
{
    Neo_Config::ConfigParamer *config_ptr = Neo_Config::GetConfigParamer();

    if( config_ptr == NULL ){
        printf("error: no found config file.\n");
        return;
    }

    sub_map_ = nh.subscribe(config_ptr->mapTopic_,0.01,&CRosNode::cbMap, this);
    sub_scan_ = nh.subscribe(config_ptr->scanTopic_,10,&CRosNode::cbScan,this);
    sub_moveStatus_ = nh.subscribe("/move_base/status",5,&CRosNode::cbMoveStatus, this);
    sub_path_ = nh.subscribe(config_ptr->pathTopic_,10,&CRosNode::cbPath, this);
    sub_tf_ = nh.subscribe("/tf",10,&CRosNode::cbTF, this);

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

    if(mutex_update_Map == NULL){
        mutex_update_Map = new pthread_mutex_t;
        pthread_mutex_init(mutex_update_Map,NULL);
    }

    if(mutex_update_Lidar == NULL){
        mutex_update_Lidar = new pthread_mutex_t;
        pthread_mutex_init(mutex_update_Lidar,NULL);
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
        }catch (tf::TransformException &ex){
        }

        ros::spinOnce();
        usleep(10000);
    }
    printf("CRosNode exit.\n");
    systerm_exit = true;
    CShareMem *shm_bus = NULL;
    shm_bus = CBackServer::GetBusShareMemery();
    shm_bus->Write((char*)&systerm_exit,1,0);
}

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
    pthread_mutex_lock(mutex_update_Map);
    this->map_base_ = *msg;
    pthread_mutex_unlock(mutex_update_Map);
}


void CRosNode::cbMoveStatus(const actionlib_msgs::GoalStatusArray &msg) {
    if(msg.status_list.size() > 0)
        robot_status.movebase_status = msg.status_list.begin().base()->status;
}

void CRosNode::cbScan(const sensor_msgs::LaserScan &msg) {
    pthread_mutex_lock(mutex_update_Lidar);
    this->scan_ = msg;
    pthread_mutex_unlock(mutex_update_Lidar);
}



void CRosNode::cbPath(const nav_msgs::Path &msg){
    pthread_mutex_lock(mutex_update_Path);
    this->path_ = msg;
    pthread_mutex_unlock(mutex_update_Path);
}

void CRosNode::cbTF(const tf2_msgs::TFMessage &msg){
    Neo_Config::ConfigParamer *config_ptr = Neo_Config::GetConfigParamer();
    for (int i = 0; i < msg.transforms.size(); ++i) {
        if( msg.transforms[i].header.frame_id == config_ptr->mapFrameId_ && msg.transforms[i].child_frame_id == "odom" ){
            isAction = true;
            //printf("-----tf\n");
        }
    }
}

void CRosNode::GetGlobalPath(nav_msgs::Path &out){
    pthread_mutex_lock(mutex_update_Path);
    out = CRosNode::path_;
    pthread_mutex_unlock(mutex_update_Path);
}

void CRosNode::GetMap(nav_msgs::OccupancyGrid &out){
    pthread_mutex_lock(mutex_update_Map);
    out = CRosNode::map_base_;
    pthread_mutex_unlock(mutex_update_Map);
}
void CRosNode::GetLidar(sensor_msgs::LaserScan &out,Neo_Type::POSE &pose){

    pthread_mutex_lock(mutex_update_Lidar);
    out = CRosNode::scan_;
    memcpy(&pose, &laser_pose, sizeof(Neo_Type::POSE));
    pthread_mutex_unlock(mutex_update_Lidar);

}
bool CRosNode::waitForAction(){
    CRosNode::isAction = false;
    int time_count = 5;
    while (!CRosNode::isAction && time_count > 0){
        sleep(1);
        time_count--;
    }
    if(time_count == 0){
        return false;
    }
    return true;
}

bool CRosNode::waitForUnAction(){
    CRosNode::isAction = true;
    int time_count = 5;
    while ( CRosNode::isAction && time_count > 0 ){
        CRosNode::isAction = false;
        sleep(1);
        time_count--;
    }
    if(time_count == 0){
        return false;
    }
    return true;
}

bool CRosNode::IsAction(){
    return CRosNode::isAction;
}
