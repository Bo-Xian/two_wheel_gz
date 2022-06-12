#ifndef TDATAUNIT_H
#define TDATAUNIT_H
#include <fstream>
#include <string>
#include <stdlib.h>
#include <string.h>
//#include "../../../strategy/src/StrategyNameAndPath.h"
using namespace std;


struct CameraParameter
{
    int  autoexposure;
    int  auto_white_balance;
    int  brightness;
    int  contrast;
    int  saturation;
    int  white_balance;
    string ParameterName;
};


class TdataUnit
{
public:
    TdataUnit();
    ~TdataUnit();
    void SaveCameraSetFiile();
    void LoadCameraSetFiile();


    CameraParameter* CameraParameterValue;
};
extern TdataUnit* DataUnit;

#endif // TDATAUNIT_H
