cmake_minimum_required(VERSION 2.8.3)
project(strategy)

find_package(catkin REQUIRED COMPONENTS
 cv_bridge 
 genmsg 
 image_transport
 message_generation
 std_msgs
 roscpp
 sensor_msgs
 tku_libs
 tku_msgs
)

find_package(OpenCV)
#set(kidsize_LIBRARIES ${CATKIN_DEVEL_PREFIX}/lib/libtool.so ${CATKIN_DEVEL_PREFIX}/lib/libstrategyinfo.so)
set(TKU_LIBRARIES ${CATKIN_DEVEL_PREFIX}/lib/libtku_libs.so)
set(STRATEGYBASE_LIBRARIES ${CATKIN_DEVEL_PREFIX}/lib/libstrategybase.so)
set(TKU_GZ_LIBRARIES ${CATKIN_DEVEL_PREFIX}/lib/libtku_gz_libs.so)
# add the resized image message
add_message_files(DIRECTORY msg

)
#add_service_files(DIRECTORY srv FILES HSVInfo.srv BuildModel.srv SaveHSV.srv)
generate_messages(DEPENDENCIES std_msgs)

catkin_package(CATKIN_DEPENDS
# std_msgs
# roscpp
)

include_directories(include ${catkin_INCLUDE_DIRS})

## Declare a C++ library
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/test.cpp
# )

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
# add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Declare a C++ executable
## With catkin_make all packages are built within a single CMake context
## The recommended prefix ensures that target names across packages don't collide
# add_executable(${PROJECT_NAME}_node src/test_node.cpp)

add_executable(twowheel twowheel/twowheel.cpp)
target_link_libraries(twowheel ${catkin_LIBRARIES})
add_executable(twowheel_backup twowheel/twowheel_backup.cpp)
target_link_libraries(twowheel_backup ${catkin_LIBRARIES})

add_executable(twowheel_controller twowheel/twowheel_controller.cpp)
target_link_libraries(twowheel_controller ${catkin_LIBRARIES})

add_executable(color_recognition twowheel/color_recognition.cpp)
target_link_libraries(color_recognition ${catkin_LIBRARIES})

## Rename C++ executable without prefix
## The above recommended prefix causes long target names, the following renames the
## target back to the shorter version for ease of user use
## e.g. "rosrun someones_pkg node" instead of "rosrun someones_pkg someones_pkg_node"
# set_target_properties(${PROJECT_NAME}_node PROPERTIES OUTPUT_NAME node PREFIX "")

## Add cmake target dependencies of the executable
## same as for the library above
# add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against
# target_link_libraries(${PROJECT_NAME}_node
#   ${catkin_LIBRARIES}
# )

