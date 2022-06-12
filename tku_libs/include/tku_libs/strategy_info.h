#ifndef PARTICLEINFO_H
#define PARTICLEINFO_H

#include <vector>
using namespace std;
enum DrawMode
{
	etDrawLine, etDrawObject
};

enum etLabelModel
{
    etOrange, etYellow, etBlue, etGreen, etBlack, etRed, etWhite, etOther
};
struct Objectdata
{
    int XMax, XMin, YMax, YMin;
    int X;
    int Y;
    int Width;
    int Height;
    int size;
};
typedef enum
{
    OrangeLabel = 0x01,
    YellowLabel = 0x02,
    BlueLabel = 0x04,  
    GreenLabel = 0x08,
    BlackLabel = 0x10,
    RedLabel = 0x20,
    WhiteLabel = 0x40,
    OthersLabel = 0x80
}etLabelMark;

enum etWalkingMode
{
    etSingleStep,
    etContinuousStep,
    etLC_Step,
    etLC_Down,
    etLongJump,
    etSingle_wood,
    etSingle_third,
    etContinuousStep_second,
    etContinuousStep_third
};

enum HeadMotorID
{
    etHorizontalID = 1, etVerticalID = 2
};

enum etSensorMode   // pitch fix X-direction error, roll fix Y-direction error
{
    etNone,         //without imu
    etRoll,         //only roll
    etPitch,        //only pitch
    etRollPitch     //roll & pitch
};

typedef union
{
    unsigned char DInput;
    struct
    {
        unsigned D0 :1;
        unsigned D1 :1;
        unsigned D2 :1;
        unsigned D3 :1;
        unsigned NONE0 :1;
        unsigned NONE2 :1;
        unsigned NONE3 :1;
        unsigned NONE4 :1;
    }Switch;
}DIOInput;
class StrategyInfo
{
public:
    StrategyInfo();
    bool getLabelModelFlag;
    bool getimage;
    int ColorMaskSubjectCnt[8];
    Objectdata ColorMaskSubject[8][320];
    DIOInput DIOValue;
    unsigned char *Label_Model;
};
extern StrategyInfo *strategyinfo;

#endif // PARTICLEINFO_H

