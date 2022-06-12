#include "strategy/kidsize/strategymain.h"

int main(int argc, char** argv)
{
	ros::init(argc, argv, "kidsize");
	ros::NodeHandle nh;
	tool_gz->set_Client(nh);
	KidsizeGazebo kidsize_gazebo(nh);

	ros::Rate loop_rate(30);

	while (nh.ok()) 
	{
		kidsize_gazebo.strategymain();
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}

void KidsizeGazebo::strategymain()
{
	if(isStart)
	{	
		
	}
	else
	{

	}
}