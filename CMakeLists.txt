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

add_library(common ${COMMON_CPP_PATH})
target_link_libraries(common pthread)

add_library(tcpip ${TCPIP_CPP_PATH})
target_link_libraries(tcpip common)

#=================================================
#    executable
#=================================================
add_executable(${PROJECT_NAME}_node ${SERVER_CPP_PATH} src/main.cpp src/server/RosRobotCtl.cpp include/neo_bridge/RosRobotCtl.h src/server/RosLidarCtl.cpp src/server/RosMappingCtl.cpp src/server/RosNavigationCtl.cpp include/neo_bridge/RosLidarCtl.h include/neo_bridge/RosMappingCtl.h include/neo_bridge/RosNavigationCtl.h include/neoslam_sdk/mode.h src/server/CCommandExecutor.cpp include/neo_bridge/CCommandExecutor.h src/server/RosExploreCtl.cpp include/neo_bridge/RosExploreCtl.h src/server/TaskCmdVel.cpp include/neoslam_sdk/Type_CmdVel.h include/neo_bridge/TaskCmdVel.h src/server/TaskLidar.cpp include/neo_bridge/TaskLidar.h include/neoslam_sdk/Type_Lidar.h include/neo_bridge/Debug.h include/neoslam_sdk/Type_Pose.h include/neoslam_sdk/neo_map.hpp src/server/TaskLoadMap.cpp include/neo_bridge/TaskLoadMap.h include/neoslam_sdk/Type_LoadMap.h include/neoslam_sdk/Type_UserData.h src/server/RosLocationCtl.cpp include/neo_bridge/RosLocationCtl.h src/server/CConfig.cpp include/neo_bridge/CConfig.h src/server/TaskGlobalPath.cpp include/neoslam_sdk/Type_GlobalPath.hpp include/neo_bridge/TaskGlobalPath.h)
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
