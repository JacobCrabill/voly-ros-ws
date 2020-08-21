cmake_minimum_required(VERSION 2.8.3)
project(gazebo_livox_lidar_plugin)

find_package(catkin REQUIRED
gazebo_plugins
gazebo_ros
roscpp)

find_package (gazebo REQUIRED)

catkin_package(
 INCLUDE_DIRS include
 LIBRARIES ${PROJECT_NAME}
 CATKIN_DEPENDS gazebo_plugins gazebo_ros roscpp
#  DEPENDS 
)

link_directories(${GAZEBO_LIBRARY_DIRS})

include_directories( 
  include
  ${catkin_INCLUDE_DIRS} 
  ${Boost_INCLUDE_DIR} 
  ${GAZEBO_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME} src/livox_lidar_plugin.cpp)
target_link_libraries( 
  ${PROJECT_NAME} 
  ${catkin_LIBRARIES} 
  ${GAZEBO_LIBRARIES} 
  RayPlugin)

install(TARGETS ${PROJECT_NAME} 
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})