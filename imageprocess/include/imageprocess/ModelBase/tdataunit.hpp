#ifndef TDATAUNIT_H
#define TDATAUNIT_H
#include <fstream>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <ros/package.h>
using namespace std;


struct ColorRange
{
    float HueMin;
    float HueMax;
    float SaturationMin;
    float SaturationMax;
    float BrightnessMin;
    float BrightnessMax;

    string LabelName;
};


class TdataUnit
{
public:
    TdataUnit();
    ~TdataUnit();
    int strategyname;
    void SaveColorRangeFile();
    void LoadColorRangeFile();
    float readvalue(fstream &fin, string title,bool mode);


    ColorRange** HSVColorRange;
};
extern TdataUnit* DataUnit;

#endif // TDATAUNIT_H
