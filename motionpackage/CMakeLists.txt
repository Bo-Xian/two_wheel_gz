cmake_minimum_required(VERSION 2.8.3)
project(motionpackage)

find_package(catkin REQUIRED COMPONENTS
# qt_build
 message_generation
 std_msgs
 roscpp
 roslib
 tku_libs
 tku_msgs
)

set(TKU_LIBRARIES ${CATKIN_DEVEL_PREFIX}/lib/libtku_libs.so)
include_directories(${catkin_INCLUDE_DIRS})

#add_message_files(DIRECTORY msg FILES 
#
#)
#add_service_files(DIRECTORY srv FILES 
#
#)
#generate_messages(DEPENDENCIES std_msgs)

catkin_package(CATKIN_DEPENDS
 message_runtime
 std_msgs
 roscpp
)



add_library(csslLib cssl/cssl.h cssl/cssl.c cssl/uty.c)

#add_executable(motionpackage motion.cpp MotorControl.h)
add_executable(motionpackage package.cpp package.h)
target_link_libraries(motionpackage ${catkin_LIBRARIES} ${TKU_LIBRARIES})
#target_link_libraries(motion_test qnode)
target_link_libraries(motionpackage csslLib)

