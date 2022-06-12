#include <ros/ros.h>
#include <stdio.h>
#include <std_msgs/Float64.h>

ros::Publisher lwheel_front_pub;
ros::Publisher rwheel_front_pub;
ros::Publisher lwheel_back_pub;
ros::Publisher rwheel_back_pub;

void set_velocity(float left_wheel, float right_wheel)
{
	std_msgs::Float64 msg;
	msg.data = left_wheel;
	lwheel_front_pub.publish(msg);
	rwheel_front_pub.publish(msg);
	msg.data = right_wheel;
	lwheel_back_pub.publish(msg);
	rwheel_back_pub.publish(msg);
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "twowheel");
	ros::NodeHandle nh;

	lwheel_front_pub = nh.advertise<std_msgs::Float64>("/twowheel/lwheel_front_joint_velocity_controller/command", 10);
	rwheel_front_pub = nh.advertise<std_msgs::Float64>("/twowheel/rwheel_front_joint_velocity_controller/command", 10);
	lwheel_back_pub = nh.advertise<std_msgs::Float64>("/twowheel/lwheel_back_joint_velocity_controller/command", 10);
	rwheel_back_pub = nh.advertise<std_msgs::Float64>("/twowheel/rwheel_back_joint_velocity_controller/command", 10);

	float left_wheel = 0,
          right_wheel = 0;

	std::printf("Set left_wheel velocity: ");
	std::scanf("%f", &left_wheel);
	std::printf("Set right_wheel velocity: ");
	std::scanf("%f", &right_wheel);
	set_velocity( left_wheel, right_wheel);

	return 0;
}
