cmake_minimum_required(VERSION 2.8.3)
project(neo_bridge)

#=================================================
#    find package
#=================================================
option(UseRos "UseRos" ON)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
)
find_package(Threads REQUIRED)

catkin_package(

)

include_directories(
        include
        ${catkin_INCLUDE_DIRS}
        include/neo_bridge
)
#link_directories("/home/server/third/lib")

#=================================================
#    GLOB
#=================================================
file(GLOB INCLUDE_PATH_LIST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/neo_bridge/*.h)

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
add_executable(${PROJECT_NAME}_node ${SERVER_CPP_PATH} src/main.cpp)
target_link_libraries(${PROJECT_NAME}_node
        ${catkin_LIBRARIES}
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
