cmake_minimum_required(VERSION 2.8.3)
project(imageprocess)

find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  image_transport
  genmsg
  roscpp
  sensor_msgs
  std_msgs
  message_generation
  tku_msgs
)

find_package(OpenCV REQUIRED)

# add the resized image message
#add_message_files(DIRECTORY msg
#   FILES ButtonColorForm.msg HSVValue.msg ColorData.msg ColorArray.msg ObjectList.msg #LabelModelObjectList.msg DrawImage.msg
#   CameraId.msg
#)


#add_service_files(DIRECTORY srv FILES HSVInfo.srv BuildModel.srv SaveHSV.srv)

#generate_messages(DEPENDENCIES sensor_msgs std_msgs)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES imageprocess
   CATKIN_DEPENDS cv_bridge image_transport roscpp sensor_msgs std_msgs
#  DEPENDS system_lib
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

# add the publisher example
add_executable(imageprocess
    src/main/imagemain.cpp
    #src/main/imagemain.h
    src/ImageBase/imagebase.cpp
    #src/ImageBase/imagebase.hpp
    src/Coordinate/tcoordinate.cpp
    #src/Coordinate/tcoordinate.hpp
    src/ModelBase/tmodelunit.cpp
    #src/ModelBase/tmodelunit.hpp
    src/ModelBase/TColorFunctionUnit.cpp
    #src/ModelBase/TColorFunctionUnit.hpp
    src/ModelBase/tdataunit.cpp
    #src/ModelBase/tdataunit.hpp
    #src/ModelBase/modelparameter.h

    #src/DrawImage/DrawImage.hpp
    src/DrawImage/DrawImage.cpp
)
target_link_libraries(imageprocess ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

install(FILES
  launch/image.launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
)

