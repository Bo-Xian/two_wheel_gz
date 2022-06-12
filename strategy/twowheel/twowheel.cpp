#include <ros/ros.h>
#include <stdio.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Float64.h>

using namespace std;

double cir_x = 0;
double cir_y = 0;
double cir_r = 0;

int status = 0;

double x_param = 5;
double z_param = 2;

void Sub_cir_x(const std_msgs::Float64 msg){
	cir_x = msg.data;
}

void Sub_cir_y(const std_msgs::Float64 msg){
	cir_y = msg.data;
}

void Sub_cir_r(const std_msgs::Float64 msg){
	cir_r = msg.data;
}

void Pub_cmd(float xx, float zz, ros::Publisher topic){
	geometry_msgs::Twist msg;
	msg.linear.x = xx;
	msg.angular.z = zz;
	topic.publish(msg);
}

void walk(float c_x, float c_y, float c_r, float& cmd_xx, float& cmd_zz){

	float temp_x = 0;
	float temp_z = 0;

	if(status==0){
		// turn to find ball
		if(c_r){
			temp_x = 10;
			temp_z = 0;
			status = 1;
		}else{
			temp_x = 2;
			temp_z = 10;
		}

	}else if(status==1){
		// tracking ball
		if(320.0>c_x && c_x>=240.0){
			temp_x = 10;
			temp_z = -2;
		}else if(240>c_x && c_x>170.0){
			temp_x = 10;
			temp_z = -1;
		}else if(170>=c_x && c_x>=150.0){
			temp_x = 10;
			temp_z = 0;
		}else if(150>c_x && c_x>=80.0){
			temp_x = 10;
			temp_z = 1;
		}else if(80>c_x && c_x>=0.0){
			temp_x = 10;
			temp_z = 2;
		}
		if(c_r>=90 || c_r==0.0){
			status = 0;
		}
	}
	cmd_xx = temp_x * x_param;
	cmd_zz = temp_z * z_param;
	// cout<<cmd_xx<<" "<<cmd_zz<<endl;
	cout<<" status = "<<status<<endl;
}


int main(int argc, char** argv){
	ros::init(argc, argv, "twowheel_strategy");
	ros::NodeHandle nh;
	ros::Rate loop_rate(20);

	ros::Subscriber cir_x_sub = nh.subscribe("/twowheel/camera/circle_x",1,Sub_cir_x);
	ros::Subscriber cir_y_sub = nh.subscribe("/twowheel/camera/circle_y",1,Sub_cir_y);
	ros::Subscriber cir_r_sub = nh.subscribe("/twowheel/camera/circle_r",1,Sub_cir_r);
	ros::Publisher cmd_pub = nh.advertise<geometry_msgs::Twist>("/twowheel/cmd_vel", 1);


	while(ros::ok()){

		float cmd_x, cmd_z;
		walk(cir_x, cir_y, cir_r,cmd_x,cmd_z);
		Pub_cmd(cmd_x, cmd_z, cmd_pub);
		// cout<<cmd_x<<", "<<cmd_z<<", "<<cir_r<<",";
		ros::spinOnce();
		loop_rate.sleep();
	}
}

