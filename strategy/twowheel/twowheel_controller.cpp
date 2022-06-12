#include <ros/ros.h>
#include <stdio.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Twist.h>
using namespace std;

ros::Publisher lwheel_front_pub;
ros::Publisher rwheel_front_pub;
ros::Publisher lwheel_back_pub;
ros::Publisher rwheel_back_pub;
ros::Subscriber cmd_sub;
float cmd_x = 0;
float cmd_z = 0;

double with_of_wheel = 6;

void cmd_sub_callback(const geometry_msgs::Twist& msg){
	// cout<< msg.linear.x<<" "<<msg.angular.z<<" ";
	cmd_x = msg.linear.x;
	cmd_z = msg.angular.z;
}

void set_velocity(float x, float z){
	// cout<< x<<" "<<z<<endl;
	std_msgs::Float64 left,right;
	left.data = x-z*with_of_wheel/2.0;
	right.data = x+z*with_of_wheel/2.0;
	lwheel_front_pub.publish(left);
	lwheel_back_pub.publish(left);
	rwheel_front_pub.publish(right);
	rwheel_back_pub.publish(right);
	// cout<<"left = "<<left.data<<" right = "<<right.data<<endl;
}

int main(int argc, char** argv){
	ros::init(argc, argv, "twowheel_controller");
	ros::NodeHandle nh;
	ros::Rate loop_rate(20);

	lwheel_front_pub = nh.advertise<std_msgs::Float64>("/twowheel/lwheel_front_joint_velocity_controller/command", 10);
	rwheel_front_pub = nh.advertise<std_msgs::Float64>("/twowheel/rwheel_front_joint_velocity_controller/command", 10);
	lwheel_back_pub = nh.advertise<std_msgs::Float64>("/twowheel/lwheel_back_joint_velocity_controller/command", 10);
	rwheel_back_pub = nh.advertise<std_msgs::Float64>("/twowheel/rwheel_back_joint_velocity_controller/command", 10);
	cmd_sub = nh.subscribe("/twowheel/cmd_vel",1000,&cmd_sub_callback);

	while(ros::ok()){

		set_velocity(cmd_x, cmd_z);
		// cout<<"running"<<endl;
		ros::spinOnce();
		loop_rate.sleep();

	}
}

