#ifndef __STRATEGY_BASE_H_
#define __STRATEGY_BASE_H_
#include <ros/ros.h>
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Bool.h>
#include <tku_msgs/Interface.h>
#include <tku_msgs/HeadPackage.h>
#include <tku_msgs/SingleMotorData.h>
#include <tku_msgs/SandHandSpeed.h>
#include <tku_msgs/ObjectList.h>
#include <tku_msgs/DrawImage.h>
#include <tku_msgs/LabelModelObjectList.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>


class TKU_KidSizeStrategyBase
{
	public:
		ros::NodeHandle *nh;
    		cv_bridge::CvImagePtr cvimg;
		bool isStart = false;
//////------------------------------Publisher API------------------------------//////
		virtual void SendBodyAuto(int x, int y, int z, int thta, int mode, int sensor);
	    virtual void SendContinuousValue(int x, int y, int z, int thta, int sensor);
		virtual void DrawImageFunction(int cnt, int mode, int xmin, int xmax, int ymin, int ymax, int r, int g, int b);
		virtual void SendHeadMotor(int ID, int Position, int Speed);
		virtual void SendBodySector(int Sector);
		virtual void SendHandSpeed(int Sector, int Speed);
		virtual void SendSingleMotor(int ID, int Position, int Speed);

//////------------------------------Subscriber API------------------------------//////
		virtual void GetObject(const tku_msgs::ObjectList &msg);
		virtual void GetLabelModel(const tku_msgs::LabelModelObjectList &msg);
		virtual void Catch_image(const sensor_msgs::ImageConstPtr& img);
		virtual void StartFunction(const std_msgs::Bool& msg);
		virtual void DIOackFunction(const std_msgs::Int16 &msg);

		//virtual void strategymain();
		//virtual float readvalue(std::fstream &fin, std::string title,bool mode);
		//virtual void Delay(int timedelay);
		TKU_KidSizeStrategyBase(ros::NodeHandle &nh);
		~TKU_KidSizeStrategyBase();
	private:
	//---------------Publisher-----------------
		ros::Publisher WalkingGait_Publish;
		ros::Publisher HeadMotor_Publish;
		ros::Publisher Sector_Publish;
		ros::Publisher HandSpeed_Publish;
		ros::Publisher DrawImage_Publish;
		ros::Publisher ContinuousValue_Publish;
		ros::Publisher SingleMotorData_Publish;
	//--------------Subscriber------------------
		ros::Subscriber Start_subscribe;
		ros::Subscriber DIOack_subscribe;
		ros::Subscriber LabelModel_subscribe;
		ros::Subscriber ObjectList_subscribe;
		ros::Subscriber compressimage_subscribe;
	//-------------------------------
};
#endif
