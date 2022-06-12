catkin_make -DCATKIN_WHITELIST_PACKAGES="tku_libs;tku_msgs"
catkin_make -DCATKIN_WHITELIST_PACKAGES_=""
. src/gazebo_ws/scripts/env.sh
sudo apt-get install ros-noetic-effort-controllers


open gazebo
    $ roslaunch fira_gz twowheel_world.launch
run strategy
    $ rosrun strategy twowheel


in lanuch
    $ rosrun strategy color_recognition
    $ rosrun strategy twowheel_controller
