#include <ros/ros.h>
#include <ros/callback_queue.h>
#include <ros/package.h>
#include "walkinggait/InverseKinematics.hpp"
#include "walkinggait/parameterinfo.hpp"
#include "tku_libs/TKU_tool.h"
#include "tku_libs/modelinfo.h"

#include <std_msgs/Float64.h>
#include <std_msgs/Int16.h>
#include "std_msgs/Bool.h"
#include "std_msgs/String.h"
#include "tku_msgs/IKinfo_message.h"
#include "tku_msgs/HeadPackage.h"
#include "tku_msgs/SaveMotionVector.h"
#include "tku_msgs/SaveMotion.h"
#include "tku_msgs/ReadMotion.h"
#include "tku_msgs/InterfaceSend2Sector.h"

enum class HeadMotorID {neck_yaw = 1, head_pitch};
enum class MotorID {left_shoulder_pitch, left_shoulder_roll, left_middle_yaw, left_elbow_pitch,
					right_shoulder_pitch, right_shoulder_roll, right_middle_yaw, right_elbow_pitch, waist_yaw,
					left_hip_yaw, left_hip_roll, left_hip_pitch, left_knee_pitch, left_ankle_pitch, left_ankle_roll,
					right_hip_yaw, right_hip_roll, right_hip_pitch, right_knee_pitch, right_ankle_pitch, right_ankle_roll};

enum class SectorMode {AbsoluteAngle = 242, RelativeAngle, MotionList};

#define MotorSum 21
// #define SPEED_CONTROL_MAX_CNTS 1000

class SectorDataBase
{
public:
	int H_D;
	int L_D;
	float remainder;
	SectorDataBase()
	{
		H_D = 0;
		L_D = 0;
		remainder = 0;
	}
	int toDec() const
	{
		if(this->H_D < 128)
		{
			return this->L_D+256*this->H_D;
		}
		else
		{
			return -(this->L_D+256*(this->H_D-128));
		}
	}
	double toPI_G()
	{
		int temp = this->toDec();
		// ROS_INFO("temp = %d", temp);
		if(temp >= 4096)
		{
			this->L_D = 255;
			this->H_D = 15;
			ROS_ERROR("AbsoluteAngle over limit angle");
			return 4095;
		}
		else if(temp >= 0)
		{
			return ((double)temp-2047)/2048.0*3.14159265;
		}
		else
		{
			this->L_D = 0;
			this->H_D = 0;
			ROS_ERROR("AbsoluteAngle is not negative");
			return 0;
		}
	}
	double toPI() const
	{
		return (double)this->toDec()/2048.0*3.14159265;
	}
	SectorDataBase *operator=(int const value)
	{
		this->remainder = 0;
		if(value >= 0)
		{
			this->L_D = value%256;
			this->H_D = value/256;
		}
		else
		{
			this->L_D = -value%256;
			this->H_D = -value/256+128;
		}
		return this;
	}
	SectorDataBase *operator/(int const value)
	{
		int temp = this->toDec()/value;
		if(value == 0)ROS_ERROR("SectorDataBase *operator/(int const value), value = 0");
		this->remainder /= (float)value;
		this->remainder += (float)this->toDec()/value - temp;
		if(this->remainder >= 1 || this->remainder <= -1)temp += (int)this->remainder;
		this->remainder = this->remainder - (int)this->remainder;
		if(temp >= 0)
		{
			this->L_D = temp%256;
			this->H_D = temp/256;
		}
		else
		{
			this->L_D = -value%256;
			this->H_D = -value/256+128;
		}
		return this;
	}
	friend SectorDataBase operator-(SectorDataBase const &c1, SectorDataBase const &c2)
	{
		SectorDataBase _temp_sector_data_base;
		int temp = c1.toDec() - c2.toDec();
		float remainder = c1.remainder - c2.remainder;
		if(remainder >= 1 || remainder <= -1)temp += (int)remainder;
		remainder = remainder - (int)remainder;
		_temp_sector_data_base.remainder = remainder;
		if(temp < 0)
		{
			_temp_sector_data_base.L_D = -temp%256;
			_temp_sector_data_base.H_D = -temp/256+128;
		}
		else
		{
			_temp_sector_data_base.L_D = temp%256;
			_temp_sector_data_base.H_D = temp/256;
		}
		return _temp_sector_data_base;
	}
	friend SectorDataBase operator+(SectorDataBase const &c1, SectorDataBase const &c2)
	{
		SectorDataBase _temp_sector_data_base;
		int temp = c1.toDec() + c2.toDec();
		float remainder = c1.remainder + c2.remainder;
		if(remainder >= 1 || remainder <= -1)temp += (int)remainder;
		remainder = remainder - (int)remainder;
		_temp_sector_data_base.remainder = remainder;
		if(temp < 0)
		{
			_temp_sector_data_base.L_D = -temp%256;
			_temp_sector_data_base.H_D = -temp/256+128;
		}
		else
		{
			_temp_sector_data_base.L_D = temp%256;
			_temp_sector_data_base.H_D = temp/256;
		}
		return _temp_sector_data_base;
	}
	SectorDataBase *operator+=(SectorDataBase const &c1)
	{
		int temp = this->toDec() + c1.toDec();
		float remainder = this->remainder + c1.remainder;
		ROS_INFO("remainder = %f", remainder);
		if(remainder >= 1 || remainder <= -1)temp += (int)remainder;
		remainder = remainder - (int)remainder;
		this->remainder = remainder;
		if(temp < 0)
		{
			this->L_D = -temp%256;
			this->H_D = -temp/256+128;
		}
		else
		{
			this->L_D = temp%256;
			this->H_D = temp/256;
		}
		return this;
	}
	SectorDataBase *operator+=(int const value)
	{
		int temp = this->toDec() + value;
		if(this->remainder >= 1 || this->remainder <= -1)temp += (int)this->remainder;
		this->remainder = this->remainder - (int)this->remainder;
		if(temp < 0)
		{
			this->L_D = -temp%256;
			this->H_D = -temp/256+128;
		}
		else
		{
			this->L_D = temp%256;
			this->H_D = temp/256;
		}
		return this;
	}
};

class SectorData
{
public:
	SectorData()
	{
		angle = new SectorDataBase[21];
		speed = new SectorDataBase[21];
	}
	~SectorData()
	{
		delete[] angle;
		delete[] speed;
	}
	void init()
	{
		delay = 0;
		delete[] angle;
		delete[] speed;
		angle = new SectorDataBase[21];
		speed = new SectorDataBase[21];
	}
	int delay;
	SectorDataBase *angle;
	SectorDataBase *speed;
};

class SimMotionPackage
{
public:
	SimMotionPackage(ros::NodeHandle &nh, ros::NodeHandle &nhPrivate)
	{
		head_control[(int)HeadMotorID::neck_yaw] 			= nh.advertise<std_msgs::Float64>("/kidsize/neck_yaw_position_controller/command", 1);
		head_control[(int)HeadMotorID::head_pitch] 			= nh.advertise<std_msgs::Float64>("/kidsize/head_pitch_position_controller/command", 1);

		motor_control[(int)MotorID::left_shoulder_pitch]	= nh.advertise<std_msgs::Float64>("/kidsize/left_shoulder_pitch_position_controller/command", 1);
		motor_control[(int)MotorID::left_shoulder_roll]		= nh.advertise<std_msgs::Float64>("/kidsize/left_shoulder_roll_position_controller/command", 1);
		motor_control[(int)MotorID::left_middle_yaw] 		= nh.advertise<std_msgs::Float64>("/kidsize/left_middle_yaw_position_controller/command", 1);
		motor_control[(int)MotorID::left_elbow_pitch] 		= nh.advertise<std_msgs::Float64>("/kidsize/left_elbow_pitch_position_controller/command", 1);

		motor_control[(int)MotorID::right_shoulder_pitch] 	= nh.advertise<std_msgs::Float64>("/kidsize/right_shoulder_pitch_position_controller/command", 1);
		motor_control[(int)MotorID::right_shoulder_roll] 	= nh.advertise<std_msgs::Float64>("/kidsize/right_shoulder_roll_position_controller/command", 1);
		motor_control[(int)MotorID::right_middle_yaw] 		= nh.advertise<std_msgs::Float64>("/kidsize/right_middle_yaw_position_controller/command", 1);
		motor_control[(int)MotorID::right_elbow_pitch] 		= nh.advertise<std_msgs::Float64>("/kidsize/right_elbow_pitch_position_controller/command", 1);
		motor_control[(int)MotorID::waist_yaw] 				= nh.advertise<std_msgs::Float64>("/kidsize/waist_yaw_position_controller/command", 1);

		motor_control[(int)MotorID::left_hip_yaw] 			= nhPrivate.advertise<std_msgs::Float64>("/kidsize/left_hip_yaw_position_controller/command", 1);
		motor_control[(int)MotorID::left_hip_roll] 			= nhPrivate.advertise<std_msgs::Float64>("/kidsize/left_hip_roll_position_controller/command", 1);
		motor_control[(int)MotorID::left_hip_pitch] 		= nhPrivate.advertise<std_msgs::Float64>("/kidsize/left_hip_pitch_position_controller/command", 1);
		motor_control[(int)MotorID::left_knee_pitch] 		= nhPrivate.advertise<std_msgs::Float64>("/kidsize/left_knee_pitch_position_controller/command", 1);
		motor_control[(int)MotorID::left_ankle_pitch] 		= nhPrivate.advertise<std_msgs::Float64>("/kidsize/left_ankle_pitch_position_controller/command", 1);
		motor_control[(int)MotorID::left_ankle_roll] 		= nhPrivate.advertise<std_msgs::Float64>("/kidsize/left_ankle_roll_position_controller/command", 1);

		motor_control[(int)MotorID::right_hip_yaw] 			= nhPrivate.advertise<std_msgs::Float64>("/kidsize/right_hip_yaw_position_controller/command", 1);
		motor_control[(int)MotorID::right_hip_roll] 		= nhPrivate.advertise<std_msgs::Float64>("/kidsize/right_hip_roll_position_controller/command", 1);
		motor_control[(int)MotorID::right_hip_pitch] 		= nhPrivate.advertise<std_msgs::Float64>("/kidsize/right_hip_pitch_position_controller/command", 1);
		motor_control[(int)MotorID::right_knee_pitch] 		= nhPrivate.advertise<std_msgs::Float64>("/kidsize/right_knee_pitch_position_controller/command", 1);
		motor_control[(int)MotorID::right_ankle_pitch] 		= nhPrivate.advertise<std_msgs::Float64>("/kidsize/right_ankle_pitch_position_controller/command", 1);
		motor_control[(int)MotorID::right_ankle_roll] 		= nhPrivate.advertise<std_msgs::Float64>("/kidsize/right_ankle_roll_position_controller/command", 1);

		walkdata_sub = nh.subscribe("/package/walkingdata", 10, &SimMotionPackage::motionCallback, this);
		headdata_sub = nh.subscribe("/package/HeadMotor", 10, &SimMotionPackage::getHeadAngle, this);
		SectorSend2FPGA_sub = nh.subscribe("/package/Sector", 1000, &SimMotionPackage::SectorSend2GazeboFunction, this);
	    InterfaceSaveData_sub = nh.subscribe("/package/InterfaceSaveMotion", 1000, &SimMotionPackage::InterfaceSaveDataFunction, this);
        InterfaceSend2Sector_sub = nh.subscribe("/package/InterfaceSend2Sector", 1000, &SimMotionPackage::InterfaceSend2SectorFunction, this);
		ExecuteCallBack_pub = nh.advertise<std_msgs::Bool>("/package/executecallback", 1000);
	    InterfaceCallBack_pub = nh.advertise<std_msgs::Bool>("/package/motioncallback", 1000);

        InterfaceReadData_ser = nh.advertiseService("/package/InterfaceReadSaveMotion",&SimMotionPackage::InterfaceReadDataFunction, this);
        // speed_control_timer = nhPrivate.createTimer(ros::Duration(0.01), &SimMotionPackage::speedControlTimer, this, false, false);

        stand_data.angle[(int)MotorID::left_shoulder_pitch] 	= 2047;
        stand_data.angle[(int)MotorID::left_shoulder_roll]		= 1100;
		stand_data.angle[(int)MotorID::left_middle_yaw]			= 3072;
		stand_data.angle[(int)MotorID::left_elbow_pitch]		= 1980;

		stand_data.angle[(int)MotorID::right_shoulder_pitch]	= 2047;
		stand_data.angle[(int)MotorID::right_shoulder_roll]		= 2994;
		stand_data.angle[(int)MotorID::right_middle_yaw]		= 1023;
		stand_data.angle[(int)MotorID::right_elbow_pitch]		= 2100;
		stand_data.angle[(int)MotorID::waist_yaw]				= 2047;

		stand_data.angle[(int)MotorID::left_hip_yaw]			= 2047;
		stand_data.angle[(int)MotorID::left_hip_roll]			= 2065;
		stand_data.angle[(int)MotorID::left_hip_pitch]			= 1650;
		stand_data.angle[(int)MotorID::left_knee_pitch]			= 2774;
		stand_data.angle[(int)MotorID::left_ankle_pitch]		= 1689;
		stand_data.angle[(int)MotorID::left_ankle_roll]			= 2030;

		stand_data.angle[(int)MotorID::right_hip_yaw]			= 2048;
		stand_data.angle[(int)MotorID::right_hip_roll]			= 2030;
		stand_data.angle[(int)MotorID::right_hip_pitch]			= 2445;
		stand_data.angle[(int)MotorID::right_knee_pitch]		= 1322;
		stand_data.angle[(int)MotorID::right_ankle_pitch]		= 2406;
		stand_data.angle[(int)MotorID::right_ankle_roll]		= 2065;
		//-------------------------------------------------------------
		ik_ref_data.angle[(int)MotorID::left_hip_yaw]			= 2048;
		ik_ref_data.angle[(int)MotorID::left_hip_roll]			= 2065;
		ik_ref_data.angle[(int)MotorID::left_hip_pitch]			= 1556;
		ik_ref_data.angle[(int)MotorID::left_knee_pitch]		= 3032;
		ik_ref_data.angle[(int)MotorID::left_ankle_pitch]		= 1556;
		ik_ref_data.angle[(int)MotorID::left_ankle_roll]		= 2030;

		ik_ref_data.angle[(int)MotorID::right_hip_yaw]			= 2048;
		ik_ref_data.angle[(int)MotorID::right_hip_roll]			= 2030;
		ik_ref_data.angle[(int)MotorID::right_hip_pitch]		= 2540;
		ik_ref_data.angle[(int)MotorID::right_knee_pitch]		= 1063;
		ik_ref_data.angle[(int)MotorID::right_ankle_pitch]		= 2540;
		ik_ref_data.angle[(int)MotorID::right_ankle_roll]		= 2130;
		// ROS_INFO("%f", (ik_ref_data.angle[9]-stand_data.angle[9]).toPI());
		readStandFunction();
		// ROS_INFO("%d", speed_control_timer.isValid());
		tool_gz.set_Client(nh);
	};
	~SimMotionPackage(){};

	void motionCallback(const tku_msgs::IKinfo_message& msg);
	void getHeadAngle(const tku_msgs::HeadPackage &msg);
	void SectorSend2GazeboFunction(const std_msgs::Int16 &msg);
	void InterfaceSaveDataFunction(const tku_msgs::SaveMotion &msg);
	void InterfaceSend2SectorFunction(const tku_msgs::InterfaceSend2Sector &msg);
	void SectorControlFuntion(unsigned int mode, SectorData &sector_data);
	void readStandFunction();
	void speedControl(SectorData &sector_data);
	void speedControlTimer(const ros::TimerEvent& e);

	bool InterfaceReadDataFunction(tku_msgs::ReadMotion::Request &Motion_req, tku_msgs::ReadMotion::Response &Motion_res);

	ros::Publisher head_control[3];
	ros::Publisher motor_control[21];
	// ---
	ros::Publisher ExecuteCallBack_pub;
	ros::Publisher InterfaceCallBack_pub;

	ros::Subscriber walkdata_sub;
	ros::Subscriber headdata_sub;
	ros::Subscriber SectorSend2FPGA_sub;
	ros::Subscriber InterfaceSaveData_sub;
	ros::Subscriber InterfaceSend2Sector_sub;

	ros::ServiceServer InterfaceReadData_ser;
	// ros::Timer speed_control_timer;

	std_msgs::Float64 motor_angle[21];
	std_msgs::Float64 head_angle[3];
	std_msgs::Bool execute_ack;
	std_msgs::Bool interface_ack;
	tku_msgs::SaveMotionVector MotionSaveData;

	gazebo_tool tool_gz;
	SectorData sector_data;
	SectorData stand_data;
	SectorData ik_ref_data;
	vector<unsigned int> SaveSectorPackage;
	vector<unsigned int> SendSectorPackage;
	vector<unsigned int> handspeedpackage;
	// vector<SectorData> vscetor_data;

	uint8_t motorpackage[27] = {0};
	uint8_t torquePackage[13] = {0};
	uint8_t packageMotorData[87] = {0}; 	// address||R MotorSum (id angleL angleH speedL speedH)*MotorSum DelayL DelayH checksum2
	int InterfaceFlag = 0;
	// int init_stand_times = 0;
	// float speed_control_cnts[MotorSum];
	// int speed_control_cnts_count;
	// int speed_control_cnts_max_count;

	InverseKinematics inversekinematics;
};
