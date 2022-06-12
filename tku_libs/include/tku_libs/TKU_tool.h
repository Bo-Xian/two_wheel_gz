#ifndef TOOL_H
#define TOOL_H

#include <ros/ros.h>
#include <ros/package.h>
#include <sys/time.h>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>

#define STANDPATH "/home/iclab/Desktop/Standmotion"

using namespace std;

class Tool
{
public:
    Tool()
    {
    	// PATH = ros::package::getPath("strategy") + "/Parameter";
    }
    float readvalue(fstream &fin, string title,int mode);
    void Delay(int timedelay);
    // string PATH;

};
extern Tool* tool;

#endif // TOOL_H
