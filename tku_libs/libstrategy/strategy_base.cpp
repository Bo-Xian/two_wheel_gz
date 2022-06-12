#include "tku_libs/strategy_base.h"
#include "tku_libs/strategy_info.h"
TKU_KidSizeStrategyBase::TKU_KidSizeStrategyBase(ros::NodeHandle &nh)
{
    this->nh = &nh;
    ContinuousValue_Publish = nh.advertise<tku_msgs::Interface>("/ChangeContinuousValue_Topic", 1000);
    WalkingGait_Publish = nh.advertise<tku_msgs::Interface>("SendBodyAuto_Topic", 1000);
    DrawImage_Publish = nh.advertise<tku_msgs::DrawImage>("/strategy/drawimage", 100);
    HeadMotor_Publish = nh.advertise<tku_msgs::HeadPackage>("/package/HeadMotor", 1000);
    Sector_Publish = nh.advertise<std_msgs::Int16>("/package/Sector", 1000);
    HandSpeed_Publish = nh.advertise<tku_msgs::SandHandSpeed>("/package/motorspeed", 1000);
    SingleMotorData_Publish = nh.advertise<tku_msgs::SingleMotorData>("/package/SingleMotorData", 1000);
      
    ObjectList_subscribe = nh.subscribe("/Object/List", 1, &TKU_KidSizeStrategyBase::GetObject,this);
    LabelModel_subscribe = nh.subscribe("/LabelModel/List", 1, &TKU_KidSizeStrategyBase::GetLabelModel,this);
    compressimage_subscribe = nh.subscribe("compress_image", 1, &TKU_KidSizeStrategyBase::Catch_image,this);
    Start_subscribe = nh.subscribe("/web/start", 1, &TKU_KidSizeStrategyBase::StartFunction,this);
    DIOack_subscribe = nh.subscribe("/package/FPGAack", 1, &TKU_KidSizeStrategyBase::DIOackFunction,this);
}

TKU_KidSizeStrategyBase::~TKU_KidSizeStrategyBase()
{

}

/*void TKU_KidSizeStrategyBase::strategymain()
{

}*/

void TKU_KidSizeStrategyBase::SendBodyAuto(int x, int y, int z, int thta, int mode, int sensor)
{
    tku_msgs::Interface walkdata;
    walkdata.x = x;
    walkdata.y = y;
    walkdata.z = z;
    walkdata.thta = thta;
    walkdata.walking_mode = mode;
    walkdata.sensor_mode = sensor;
    // if(x >= 0)
    //     walkdata.walking_state = 0;
    // else
    //     walkdata.walking_state = 1;
    
    WalkingGait_Publish.publish(walkdata);
}

void TKU_KidSizeStrategyBase::SendContinuousValue(int x, int y,  int z, int thta, int sensor)
{
    tku_msgs::Interface walkdata;
    walkdata.x = x;
    walkdata.y = y;
    walkdata.z = z;
    walkdata.thta = thta;
    walkdata.sensor_mode = sensor;
    // if(x >= 0)
    //     walkdata.walking_state = 0;
    // else
    //     walkdata.walking_state = 1;

    ContinuousValue_Publish.publish(walkdata);
}

void TKU_KidSizeStrategyBase::GetObject(const tku_msgs::ObjectList &msg)
{
    if(strategyinfo->getimage)
    {
        for(int i = 0; i < 8; i++)
        {
            strategyinfo->ColorMaskSubjectCnt[i] = msg.Objectlist[i].cnt;
            for(int j = 0; j < strategyinfo->ColorMaskSubjectCnt[i]; j++)
            {
                strategyinfo->ColorMaskSubject[i][j].X = msg.Objectlist[i].Colorarray[j].X;
                strategyinfo->ColorMaskSubject[i][j].Y = msg.Objectlist[i].Colorarray[j].Y;
                strategyinfo->ColorMaskSubject[i][j].XMin = msg.Objectlist[i].Colorarray[j].XMin;
                strategyinfo->ColorMaskSubject[i][j].YMin = msg.Objectlist[i].Colorarray[j].YMin;
                strategyinfo->ColorMaskSubject[i][j].XMax = msg.Objectlist[i].Colorarray[j].XMax;
                strategyinfo->ColorMaskSubject[i][j].YMax = msg.Objectlist[i].Colorarray[j].YMax;
                strategyinfo->ColorMaskSubject[i][j].Width = msg.Objectlist[i].Colorarray[j].Width;
                strategyinfo->ColorMaskSubject[i][j].Height = msg.Objectlist[i].Colorarray[j].Height;
                strategyinfo->ColorMaskSubject[i][j].size = msg.Objectlist[i].Colorarray[j].size;
            }
        }
    }
    strategyinfo->getimage = false;
}

void TKU_KidSizeStrategyBase::GetLabelModel(const tku_msgs::LabelModelObjectList &msg)
{
    for(int i = 0; i < 320*240; i++)
    {
        strategyinfo->Label_Model[i] = msg.LabelModel[i];
    }
    strategyinfo->getLabelModelFlag = false;
}
void TKU_KidSizeStrategyBase::DrawImageFunction(int cnt, int mode, int xmin, int xmax, int ymin, int ymax, int r, int g, int b)
{
    tku_msgs::DrawImage ImageData;
    ImageData.cnt = cnt;
    ImageData.XMax = xmax;
    ImageData.XMin = xmin;
    ImageData.YMax = ymax;
    ImageData.YMin = ymin;
    ImageData.rValue = r;
    ImageData.gValue = g;
    ImageData.bValue = b;
    ImageData.Mode = mode;
    DrawImage_Publish.publish(ImageData);
}

void TKU_KidSizeStrategyBase::Catch_image(const sensor_msgs::ImageConstPtr& img)
{
    cvimg = cv_bridge::toCvCopy(img, sensor_msgs::image_encodings::BGR8);
}

void TKU_KidSizeStrategyBase::SendHeadMotor(int ID, int Position, int Speed)
{
    tku_msgs::HeadPackage HeadData;
    HeadData.ID = ID;
    HeadData.Position = Position;
    HeadData.Speed = Speed;
    HeadMotor_Publish.publish(HeadData);
}

void TKU_KidSizeStrategyBase::SendBodySector(int Sector)
{
    std_msgs::Int16 SectorData;
    SectorData.data = Sector;
    Sector_Publish.publish(SectorData);
}

void TKU_KidSizeStrategyBase::SendHandSpeed(int Sector, int Speed)
{
    tku_msgs::SandHandSpeed HandSpeedData;
    HandSpeedData.sector = Sector;
    HandSpeedData.speed = Speed;
    HandSpeed_Publish.publish(HandSpeedData);
}

void TKU_KidSizeStrategyBase::SendSingleMotor(int ID, int Position, int Speed)
{
    tku_msgs::SingleMotorData MotorData;
    MotorData.ID = ID;
    MotorData.Position = Position;
    MotorData.Speed = Speed;
    SingleMotorData_Publish.publish(MotorData);
}

void TKU_KidSizeStrategyBase::StartFunction(const std_msgs::Bool &msg)
{
    ROS_INFO("start");
    if(msg.data)
        isStart = true;
    else
        isStart = false;
}

void TKU_KidSizeStrategyBase::DIOackFunction(const std_msgs::Int16 &msg)
{
    if(msg.data & 0x10)
        isStart = true;
    else
        isStart = false;
    strategyinfo->DIOValue.DInput = msg.data;
}
