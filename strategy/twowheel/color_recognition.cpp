#include <ros/ros.h>
#include <stdio.h>
#include <geometry_msgs/Twist.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <std_msgs/Float64.h>
using namespace std;
using namespace cv;

cv::Mat orign;
sensor_msgs::ImagePtr msg_orign;

void Sub_image(const sensor_msgs::ImageConstPtr& msg){
	cv_bridge::CvImagePtr cv_ptr;
    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    orign = cv_ptr->image;
}

void Pub_image(Mat data, image_transport::Publisher topic){
	topic.publish(cv_bridge::CvImage(std_msgs::Header(), "bgr8", data).toImageMsg());
}

void pub_circle_x(double data, ros::Publisher topic){
	std_msgs::Float64 temp;
	temp.data = data;
	topic.publish(temp);
}

void pub_circle_y(double data, ros::Publisher topic){
	std_msgs::Float64 temp;
	temp.data = data;
	topic.publish(temp);
}
void pub_circle_r(double data, ros::Publisher topic){
	std_msgs::Float64 temp;
	temp.data = data;
	topic.publish(temp);
}

void Hough_Circle_transform(Mat src, Mat& dst, double& x, double& y, double& r){
	resize(src, src, cv::Size(320, 240));
	//中值滤波
	Mat mOut;
	medianBlur(src, mOut, 9);
	cvtColor(mOut, mOut, CV_BGR2GRAY);

	//找霍夫圆
	vector<Vec3f> cir;
	HoughCircles(mOut, cir, CV_HOUGH_GRADIENT, 1, 20, 100, 30, 5, 100);
	src.copyTo(dst);
	if(cir.size()){
		for (size_t i = 0; i < cir.size(); i++) {
			Vec3f cc = cir[i];
			circle(dst, Point(cc[0], cc[1]), cc[2], Scalar(255, 255, 255), 2, LINE_AA);
			circle(dst, Point(cc[0], cc[1]),2, Scalar(255, 255, 255), 2, LINE_AA);
			// cout<<cir[i]<<" ";
			x = cir[i][0];
			y = cir[i][1];
			r = cir[i][2];
		}
	}else{
		x = 0;
		y = 0;
		r = 0;
	}
	// cout<<endl;
}


int main(int argc, char** argv){
	ros::init(argc, argv, "image_process");
	ros::NodeHandle nh;
	ros::Rate loop_rate(30);

	image_transport::ImageTransport it(nh);
	image_transport::Subscriber sub = it.subscribe("/twowheel/camera/image_raw", 1, Sub_image);
	image_transport::Publisher img_pub = it.advertise("/twowheel/camera/mask", 1);

	ros::Publisher cir_x_pub = nh.advertise<std_msgs::Float64>("/twowheel/camera/circle_x", 1);
	ros::Publisher cir_y_pub = nh.advertise<std_msgs::Float64>("/twowheel/camera/circle_y", 1);
	ros::Publisher cir_r_pub = nh.advertise<std_msgs::Float64>("/twowheel/camera/circle_r", 1);


	cv::Mat processed_img, Hough_img;
	double cir_x, cir_y, cir_r;

	while(ros::ok()){
		if(!orign.empty()){
			Hough_Circle_transform(orign, Hough_img, cir_x, cir_y, cir_r);
			Pub_image(Hough_img, img_pub);
			pub_circle_x(cir_x,cir_x_pub);
			pub_circle_y(cir_y,cir_y_pub);
			pub_circle_r(cir_r,cir_r_pub);
		}
		ros::spinOnce();
		loop_rate.sleep();
	}
}

