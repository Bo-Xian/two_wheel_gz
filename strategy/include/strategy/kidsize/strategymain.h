#include <ros/ros.h>
#include <ros/package.h>
#include <vector>
#include <stdio.h>
#include <std_msgs/String.h>
#include "kidsize_libs/strategyinfo.h"
#include "kidsize_libs/tool.h"
#include "kidsize_libs/modelinfo.h"
#include "kidsize_libs/MotionTable.h"
#include "kidsize_libs/strategybase.h"

using namespace std;

class KidsizeGazebo : public StrategyBase
{
public:
	KidsizeGazebo(ros::NodeHandle &nh) : StrategyBase(nh)
	{

	};
	~KidsizeGazebo(){};
	void strategymain();
};