#include "tdataunit/tdataunit.hpp"
#include "tku_libs/TKU_tool.h"

// TdataUnit* DataUnit = new TdataUnit();
TdataUnit::TdataUnit()
{
    CameraParameterValue = new CameraParameter;
    CameraParameterValue->autoexposure = 0;
    CameraParameterValue->auto_white_balance = 0;
    CameraParameterValue->brightness = 0;
    CameraParameterValue->contrast = 0;
    CameraParameterValue->saturation = 0;
    CameraParameterValue->white_balance = 0;
    CameraParameterValue->ParameterName = "[Camera Set Parameter]";   
}

TdataUnit::~TdataUnit()
{
    delete CameraParameterValue;
}

void TdataUnit::SaveCameraSetFiile()
{
    char path[200];
    printf("%s",path);
    std::string PATH = ros::package::getPath("strategy") + "/Parameter";
    strcpy(path, PATH.c_str());
    strcat(path, "/CameraSet.ini");
    try
    {
//        ofstream OutFile(sFileName.c_str());
        ofstream OutFile(path);
        OutFile << CameraParameterValue->ParameterName;
        OutFile << "\n";
        OutFile << "autoexposure = ";
        OutFile << CameraParameterValue->autoexposure;
        OutFile << "\n";
        OutFile << "auto_white_balance = ";
        OutFile << CameraParameterValue->auto_white_balance;
        OutFile << "\n";
        OutFile << "brightness = ";
        OutFile << CameraParameterValue->brightness;
        OutFile << "\n";
        OutFile << "contrast = ";
        OutFile << CameraParameterValue->contrast;
        OutFile << "\n";
        OutFile << "saturation = ";
        OutFile << CameraParameterValue->saturation;
        OutFile << "\n";
        OutFile << "white_balance = ";
        OutFile << CameraParameterValue->white_balance;
        OutFile << "\n";

        OutFile.close();
    }
    catch( exception e )
    {
    }
}

void TdataUnit::LoadCameraSetFiile()
{

    fstream fin;
    char line[100]; 
    char path[200];
    std::string PATH = ros::package::getPath("strategy") + "/Parameter";
    strcpy(path, PATH.c_str());
    strcat(path, "/CameraSet.ini");

    fin.open(path, ios::in);
    //fin.open(("../../Parameter/Color_Model_Data/ColorModelData.ini"), ios::in);
    try
    {
        fin.getline(line,sizeof(line),'\n');
        CameraParameterValue->autoexposure = tool->readvalue(fin, "autoexposure", 0);
        CameraParameterValue->auto_white_balance = tool->readvalue(fin, "auto_white_balance", 0);
        CameraParameterValue->brightness = tool->readvalue(fin, "brightness", 0);
        CameraParameterValue->contrast = tool->readvalue(fin, "contrast", 0);
        CameraParameterValue->saturation = tool->readvalue(fin, "saturation", 0);
        CameraParameterValue->white_balance = tool->readvalue(fin, "white_balance", 0);
        fin.close();
    }
    catch(exception e)
    {
    }
}


