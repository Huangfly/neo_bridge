cmake_minimum_required(VERSION 2.8.3)
project(neo_bridge)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#=================================================
#    find package
#=================================================
option(UseRos "UseRos" ON)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  tf
)
find_package(Threads REQUIRED)

find_package(Lua REQUIRED)

catkin_package(

)



include_directories(
        include
        ${catkin_INCLUDE_DIRS}
        ${LUA_INCLUDE_DIR}
        include/neo_bridge
)


message("   *** Lua include: ${LUA_INCLUDE_DIR}")
message("   *** Lua lib: ${LUA_LIBRARIES}")
#link_directories("/home/server/third/lib")

#=================================================
#    GLOB
#=================================================
file(GLOB INCLUDE_PATH_LIST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/neo_bridge/*.h)
file(GLOB INCLUDE_HPP_PATH_LIST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/neo_bridge/*.hpp)
file(GLOB NODE_CPP_PATH_LIST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
file(GLOB COMMON_CPP_PATH RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/common/*.cpp)
file(GLOB SERVER_CPP_PATH RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/server/*.cpp)
file(GLOB TCPIP_CPP_PATH RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/TCPIP/*.cpp)
message(STATUS "common path" ${COMMON_CPP_PATH})
message(STATUS "tcpip path" ${TCPIP_CPP_PATH})
#=================================================
#    library
#=================================================
#SET_TARGET_PROPERTIES(common PROPERTIES "-pthread")  

add_library(common ${COMMON_CPP_PATH} src/common/CDebug.cpp)
target_link_libraries(common pthread)

add_library(tcpip ${TCPIP_CPP_PATH} src/TCPIP/CBusTask.cpp src/TCPIP/CUnDataPackTask.cpp)
target_link_libraries(tcpip common)

#=================================================
#    executable
#=================================================
add_executable(${PROJECT_NAME}_node ${SERVER_CPP_PATH} src/main.cpp src/server/RosMappingCtl.cpp src/server/RosNavigationCtl.cpp include/neo_bridge/RosMappingCtl.h include/neo_bridge/RosNavigationCtl.h src/server/CCommandExecutor.cpp include/neo_bridge/CCommandExecutor.h src/server/RosExploreCtl.cpp include/neo_bridge/RosExploreCtl.h src/server/TaskCmdVel.cpp include/neoslam_sdk/TypePacket_CmdVel.h include/neo_bridge/TaskCmdVel.h src/server/TaskLidar.cpp include/neo_bridge/TaskLidar.h include/neoslam_sdk/TypePacket_LidarDatas.h include/neoslam_sdk/Type_Pose.h include/neoslam_sdk/Type_MutexMap.hpp src/server/TaskLoadMap.cpp include/neo_bridge/TaskLoadMap.h include/neoslam_sdk/TypePacket_MapUpload.h include/neoslam_sdk/Type_UserData.h src/server/RosLocalizationCtl.cpp include/neo_bridge/RosLocalizationCtl.h src/server/CConfig.cpp include/neo_bridge/CConfig.h src/server/TaskGlobalPath.cpp include/neoslam_sdk/TypePacket_GlobalPath.h include/neo_bridge/TaskGlobalPath.h include/neoslam_sdk/TypePacket_RobotStatus.h include/neoslam_sdk/TypePacket_RosControl.h include/neoslam_sdk/TypePacket_MapDatas.h include/neoslam_sdk/TypePacket_SendGoal.h include/neoslam_sdk/TypePacket_CancelGoal.h include/neo_bridge/CDebug.h include/neo_bridge/CBusTask.h include/neo_bridge/CUnDataPackTask.h src/server/ThreadReceiveAck.cpp include/neo_bridge/ThreadReceiveAck.h src/server/ThreadTimerTask.cpp  include/neo_bridge/ThreadTimerTask.h src/server/ThreadClientEpoll.cpp include/neo_bridge/ThreadClientEpoll.h)
target_link_libraries(${PROJECT_NAME}_node
        ${catkin_LIBRARIES}
        ${LUA_LIBRARIES}
        -lm
        common
        tcpip
        )

add_executable(example src/client_example.cpp)


#add_executable(${PROJECT_NAME}_preserver ${SERVER_CPP_PATH_LIST} src/main.cpp)
#target_link_libraries(${PROJECT_NAME}_node
#  ${catkin_LIBRARIES}
#  common
#  tcpip
#)

#add_executable(${PROJECT_NAME}_backserver ${SERVER_CPP_PATH_LIST} src/main.cpp)
#target_link_libraries(${PROJECT_NAME}_node
#  ${catkin_LIBRARIES}
#  common
#  tcpip
#)
