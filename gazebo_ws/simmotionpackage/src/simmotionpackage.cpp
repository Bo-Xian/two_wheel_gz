#include "simmotionpackage/simmotionpackage.h"

// double time_start = 0, time_end = 0;
void SimMotionPackage::motionCallback (const tku_msgs::IKinfo_message& msg)
{
    parameterinfo->ik_parameters.R_Goal[0] = -msg.IK_Point_RX;
    parameterinfo->ik_parameters.R_Goal[1] = msg.IK_Point_RY - 4.5;
    parameterinfo->ik_parameters.R_Goal[2] = 21.7 - msg.IK_Point_RZ;
    parameterinfo->ik_parameters.R_Goal[3] = msg.IK_Point_RThta;
    parameterinfo->ik_parameters.L_Goal[0] = msg.IK_Point_LX;
    parameterinfo->ik_parameters.L_Goal[1] = msg.IK_Point_LY + 4.5;
    parameterinfo->ik_parameters.L_Goal[2] = 21.7 - msg.IK_Point_LZ;
    parameterinfo->ik_parameters.L_Goal[3] = msg.IK_Point_LThta;
    parameterinfo->parameters.Period_T = msg.Period_T;
    parameterinfo->parameters.Sample_Time = msg.Sampletime;
    inversekinematics.DoIK((parameterinfo->parameters.Period_T / parameterinfo->parameters.Sample_Time));

    // motor_angle[9].data = parameterinfo->ik_parameters.Ltest_theta[5];
    // motor_angle[10].data = parameterinfo->ik_parameters.Ltest_theta[0];//+Ladd;
    // motor_angle[11].data = parameterinfo->ik_parameters.Ltest_theta[1]-0.105 + 0.25;//-0.105;
    // motor_angle[12].data = parameterinfo->ik_parameters.Ltest_theta[2]+0.104 - 0.5;
    // motor_angle[13].data = parameterinfo->ik_parameters.Ltest_theta[3]-0.045 + 0.25;
    // motor_angle[14].data = -parameterinfo->ik_parameters.Ltest_theta[4];//-Ladd;

    // motor_angle[15].data = -parameterinfo->ik_parameters.Rtest_theta[5];
    // motor_angle[16].data = parameterinfo->ik_parameters.Rtest_theta[0];//+Radd;
    // motor_angle[17].data = -(parameterinfo->ik_parameters.Rtest_theta[1]-0.105 + 0.25);//+0.105;
    // motor_angle[18].data = -(parameterinfo->ik_parameters.Rtest_theta[2]+0.104 - 0.5);
    // motor_angle[19].data = -(parameterinfo->ik_parameters.Rtest_theta[3]-0.045 + 0.25);
    // motor_angle[20].data = -(parameterinfo->ik_parameters.Rtest_theta[4]+0.1);//-Radd;

    // printf("%f\t", (parameterinfo->ik_parameters.Ltest_theta[5]+3.14159265)/(2*3.14159265)*4096);
    // printf("%f\t", (parameterinfo->ik_parameters.Ltest_theta[0]+3.14159265)/(2*3.14159265)*4096);
    // printf("%f\t", (parameterinfo->ik_parameters.Ltest_theta[1]+3.14159265)/(2*3.14159265)*4096);
    // printf("%f\t", (parameterinfo->ik_parameters.Ltest_theta[2]+3.14159265)/(2*3.14159265)*4096);
    // printf("%f\t", (parameterinfo->ik_parameters.Ltest_theta[3]+3.14159265)/(2*3.14159265)*4096);
    // printf("%f\n", (-parameterinfo->ik_parameters.Ltest_theta[4]+3.14159265)/(2*3.14159265)*4096);

    // printf("%f\t", (-parameterinfo->ik_parameters.Rtest_theta[5]+3.14159265)/(2*3.14159265)*4096);
    // printf("%f\t", (parameterinfo->ik_parameters.Rtest_theta[0]+3.14159265)/(2*3.14159265)*4096);
    // printf("%f\t", (-parameterinfo->ik_parameters.Rtest_theta[1]+3.14159265)/(2*3.14159265)*4096);
    // printf("%f\t", (-parameterinfo->ik_parameters.Rtest_theta[2]+3.14159265)/(2*3.14159265)*4096);
    // printf("%f\t", (-parameterinfo->ik_parameters.Rtest_theta[3]+3.14159265)/(2*3.14159265)*4096);
    // printf("%f\n", (-parameterinfo->ik_parameters.Rtest_theta[4]+3.14159265)/(2*3.14159265)*4096);
    // ROS_INFO("%f",motor_angle[12].data);
    motor_angle[9].data = parameterinfo->ik_parameters.Ltest_theta[5] + (stand_data.angle[(int)MotorID::left_hip_yaw] - ik_ref_data.angle[(int)MotorID::left_hip_yaw]).toPI();
    motor_angle[10].data = parameterinfo->ik_parameters.Ltest_theta[0] + (stand_data.angle[(int)MotorID::left_hip_roll] - ik_ref_data.angle[(int)MotorID::left_hip_roll]).toPI();
    motor_angle[11].data = parameterinfo->ik_parameters.Ltest_theta[1] + (stand_data.angle[(int)MotorID::left_hip_pitch] - ik_ref_data.angle[(int)MotorID::left_hip_pitch]).toPI();
    motor_angle[12].data = parameterinfo->ik_parameters.Ltest_theta[2] + (stand_data.angle[(int)MotorID::left_knee_pitch] - ik_ref_data.angle[(int)MotorID::left_knee_pitch]).toPI();
    motor_angle[13].data = parameterinfo->ik_parameters.Ltest_theta[3] + (stand_data.angle[(int)MotorID::left_ankle_pitch] - ik_ref_data.angle[(int)MotorID::left_ankle_pitch]).toPI();
    motor_angle[14].data = -parameterinfo->ik_parameters.Ltest_theta[4] + (stand_data.angle[(int)MotorID::left_ankle_roll] - ik_ref_data.angle[(int)MotorID::left_ankle_roll]).toPI();

    motor_angle[15].data = -parameterinfo->ik_parameters.Rtest_theta[5] + (stand_data.angle[(int)MotorID::right_hip_yaw] - ik_ref_data.angle[(int)MotorID::right_hip_yaw]).toPI();
    motor_angle[16].data = parameterinfo->ik_parameters.Rtest_theta[0] + (stand_data.angle[(int)MotorID::right_hip_roll] - ik_ref_data.angle[(int)MotorID::right_hip_roll]).toPI();
    motor_angle[17].data = -(parameterinfo->ik_parameters.Rtest_theta[1]) + (stand_data.angle[(int)MotorID::right_hip_pitch] - ik_ref_data.angle[(int)MotorID::right_hip_pitch]).toPI();
    motor_angle[18].data = -(parameterinfo->ik_parameters.Rtest_theta[2]) + (stand_data.angle[(int)MotorID::right_knee_pitch] - ik_ref_data.angle[(int)MotorID::right_knee_pitch]).toPI();
    motor_angle[19].data = -(parameterinfo->ik_parameters.Rtest_theta[3]) + (stand_data.angle[(int)MotorID::right_ankle_pitch] - ik_ref_data.angle[(int)MotorID::right_ankle_pitch]).toPI();
    motor_angle[20].data = -parameterinfo->ik_parameters.Rtest_theta[4] + (stand_data.angle[(int)MotorID::right_ankle_roll] - ik_ref_data.angle[(int)MotorID::right_ankle_roll]).toPI();

    for(int i = 9; i < 21; i++)motor_control[i].publish(this->motor_angle[i]);
    // time_end = ros::WallTime::now().toSec();
}

void SimMotionPackage::getHeadAngle(const tku_msgs::HeadPackage &msg)
{
    if(msg.ID == (int)HeadMotorID::head_pitch)
    {
        head_angle[(int)HeadMotorID::head_pitch].data = (msg.Position-2047)/2048.0*3.14159265;
        head_control[(int)HeadMotorID::head_pitch].publish(head_angle[(int)HeadMotorID::head_pitch]);
    }
    else
    {
        head_angle[(int)HeadMotorID::neck_yaw].data = (msg.Position-2047)/2048.0*3.14159265;
        head_control[(int)HeadMotorID::neck_yaw].publish(head_angle[(int)HeadMotorID::neck_yaw]);
    }
}

void SimMotionPackage::SectorControlFuntion(unsigned int mode, SectorData &sector_data)
{
    switch(mode)
    {
        case (unsigned int)SectorMode::AbsoluteAngle:
            for(int i = 0; i < MotorSum; i++)this->stand_data.angle[i] = sector_data.angle[i];
            for(int i = 0; i < MotorSum; i++)motor_angle[i].data = this->stand_data.angle[i].toPI_G();
            for(int i = 0; i < MotorSum; i++)motor_control[i].publish(this->motor_angle[i]);
        break;
        case (unsigned int)SectorMode::RelativeAngle:
            for(int i = 0; i < MotorSum; i++)this->stand_data.angle[i] += sector_data.angle[i];
            for(int i = 0; i < MotorSum; i++)motor_angle[i].data = this->stand_data.angle[i].toPI_G();
            for(int i = 0; i < MotorSum; i++)motor_control[i].publish(this->motor_angle[i]);
            // for(int i = 0; i < MotorSum; i++)ROS_INFO("speed = %d\tangle = %d", stand_data.speed[i].toDec(), stand_data.angle[i].toDec());
        break;
        case (unsigned int)SectorMode::MotionList:
            for(int i = 0; i < MotorSum; i++)this->stand_data.angle[i] += sector_data.angle[i];
            for(int i = 0; i < MotorSum; i++)motor_angle[i].data = this->stand_data.angle[i].toPI_G();
            for(int i = 0; i < MotorSum; i++)motor_control[i].publish(this->motor_angle[i]);
            tool_gz.simDelay(sector_data.delay);
        break;
    }
}

void SimMotionPackage::SectorSend2GazeboFunction(const std_msgs::Int16 &msg)
{
    ROS_INFO("SendSectorPackage");
    char filename[10];
    sprintf(filename,"%d",msg.data);
    char pathend[20] = "/sector/";
    char pathend2[20] = ".ini";
    char path[200];
    int packagecnt;
    this->sector_data.init();
    // if(msg.data == 29)
    // {
    //     strcpy(path, STANDPATH);
    //     strcat(pathend, filename);
    //     strcat(path, pathend);
    //     strcat(path, pathend2);
    // }
    // else
    {
        string PATH = ros::package::getPath("strategy") + "/Parameter";
        strcpy(path, PATH.c_str());
        strcat(pathend, filename);
        strcat(path, pathend);
        strcat(path, pathend2);
    }
    fstream fin;
    fin.open(path, ios::in);
    if(!fin)
    {
        ROS_INFO("Filename Error!!");
    }
    else
    {
        try
        {
            packagecnt = tool->readvalue(fin, "PackageCnt", 0);
            SendSectorPackage.push_back(tool->readvalue(fin, "Package", 2));
            ROS_INFO("mode = %d",SendSectorPackage[0]);
            for(int i = 1; i < packagecnt; i++)
            {
                SendSectorPackage.push_back(tool->readvalue(fin, "|", 3));
                //ROS_INFO("%d",SendSectorPackage[i]);
            }
            for(int i = 1, j = 0; i < packagecnt; i+=4)
            {
                if(SendSectorPackage[i] == 68 && SendSectorPackage[i+1] == 89)
                {
                    this->sector_data.delay = SendSectorPackage[i+2];
                    ROS_INFO("delay_time = %d", this->sector_data.delay);
                    SectorControlFuntion(SendSectorPackage[0], this->sector_data);
                    this->sector_data.init();
                    i += 3;
                    j = 0;
                    if(SendSectorPackage[i] == 69 && SendSectorPackage[i+1] == 78)
                    {
                        execute_ack.data = true;
                        ExecuteCallBack_pub.publish(execute_ack);
                        ROS_INFO("Execute is finsih");
                        SendSectorPackage.clear();
                        ROS_INFO("End_LoadSector");
                        return;
                    }
                }
                if(j > 20)ROS_ERROR("MotorID over 21");
                this->sector_data.speed[j].L_D = SendSectorPackage[i];
                this->sector_data.speed[j].H_D = SendSectorPackage[i+1];

                this->sector_data.angle[j].L_D = SendSectorPackage[i+2];
                this->sector_data.angle[j].H_D = SendSectorPackage[i+3];
                // ROS_INFO("speed.L_D = %d, speed.H_D = %d", sector_data.speed[j].L_D, sector_data.speed[j].H_D);
                // ROS_INFO("angle.L_D = %d, angle.H_D = %d", sector_data.angle[j].L_D, sector_data.angle[j].H_D);
                ROS_INFO("speed = %d\tangle = %d", this->sector_data.speed[j].toDec(), this->sector_data.angle[j].toDec());
                j++;
            }
            SectorControlFuntion(SendSectorPackage[0], this->sector_data);
            execute_ack.data = true;
            ExecuteCallBack_pub.publish(execute_ack);
            ROS_INFO("Execute is finsih");
        }
        catch(exception e)
        {
        }
    }
    SendSectorPackage.clear();
    ROS_INFO("End_LoadSector");
}

bool SimMotionPackage::InterfaceReadDataFunction(tku_msgs::ReadMotion::Request &Motion_req, tku_msgs::ReadMotion::Response &Motion_res)
{
    ROS_INFO("LoadParameter");
    string filename = Motion_req.name;
    char pathend[20] = "/";
    char path[200];
    char str[10];
    int datacnt;
    int num;
    if(Motion_req.readstate == 1)
    {
        strcpy(path, STANDPATH);
        strcat(pathend, filename.c_str());
        strcat(path, pathend);
    }
    else
    {
        std::string PATH = ros::package::getPath("strategy") + "/Parameter";
        strcpy(path, PATH.c_str());
        strcat(pathend, filename.c_str());
        strcat(path, pathend);
    }
    fstream fin;

    fin.open(path, ios::in);
    if(!fin)
    {
        ROS_INFO("Filename Error!!");
    }
    else
    {
        try
        {
            ROS_INFO("Start_Load");
            Motion_res.VectorCnt = tool->readvalue(fin, "VectorCnt", 0);
            for(int i = 0; i < Motion_res.VectorCnt; i++)
            {
                Motion_res.motionstate.push_back(tool->readvalue(fin, "State", 0));
                Motion_res.ID.push_back(tool->readvalue(fin, "ID", 0));
                switch(Motion_res.motionstate[i])
                {
                    case 0:
                        for(int j = 0; j < 40; j++)
                        {
                            if((j+1)%2 == 1)
                            {
                                char Motion[20] = "A";
                                sprintf(str,"%d",(j/2)+1);
                                strcat(Motion, str);
                                num = tool->readvalue(fin, Motion, 2);
                            }
                            else
                            {
                                char Delay[20] = "D";
                                sprintf(str,"%d",(j/2)+1);
                                strcat(Delay, str);
                                if(j == 39)
                                {
                                    num = tool->readvalue(fin, Delay, 0);
                                }
                                else
                                {
                                    num = tool->readvalue(fin, Delay, 2);
                                }
                            }
                            Motion_res.MotionList.push_back(num);
                        }
                        break;
                    case 1:
                        for(int j = 0; j < 21; j++)
                        {
                            char Motor[20] = "M";
                            sprintf(str,"%d",j+1);
                            strcat(Motor, str);
                            if(j == 20)
                            {
                              num = tool->readvalue(fin, Motor, 0);
                            }
                            else
                            {
                                num = tool->readvalue(fin, Motor, 2);
                            }
                            Motion_res.RelativeData.push_back(num);
                        }
                        break;
                    case 2:
                        for(int j = 0; j < 21; j++)
                        {
                            char Motor[20] = "M";
                            sprintf(str,"%d",j+1);
                            strcat(Motor, str);
                            if(j == 20)
                            {
                                num = tool->readvalue(fin, Motor, 0);
                            }
                            else
                            {
                                num = tool->readvalue(fin, Motor, 2);
                            }
                            Motion_res.RelativeData.push_back(num);
                        }
                        break;
                    case 3:
                        for(int j = 0; j < 21; j++)
                        {
                            char Motor[20] = "M";
                            sprintf(str,"%d",j+1);
                            strcat(Motor, str);
                            if(j == 20)
                            {
                                num = tool->readvalue(fin, Motor, 0);
                            }
                            else
                            {
                                num = tool->readvalue(fin, Motor, 2);
                            }
                            Motion_res.AbsoluteData.push_back(num);
                        }
                        break;
                    case 4:
                        for(int j = 0; j < 21; j++)
                        {
                            char Motor[20] = "M";
                            sprintf(str,"%d",j+1);
                            strcat(Motor, str);
                            if(j == 20)
                            {
                                num = tool->readvalue(fin, Motor, 0);
                            }
                            else
                            {
                                num = tool->readvalue(fin, Motor, 2);
                            }
                            Motion_res.AbsoluteData.push_back(num);
                        }
                        break;
                }               

            }
        }
        catch(exception e)
        {
        }
    }
    ROS_INFO("End_Load");
    return true;
}

void SimMotionPackage::InterfaceSaveDataFunction(const tku_msgs::SaveMotion &msg)
{
    MotionSaveData.SaveMotionVector.push_back(msg);
    if(msg.saveflag == true)
    {
        ROS_INFO("VectorSize = %d",MotionSaveData.SaveMotionVector.size()-1);
        string filename = msg.name;
        char pathend[20] = "/";
        char path[200];
        if(msg.savestate == 1)
        {
            strcpy(path, STANDPATH);
            strcat(pathend, filename.c_str());
            strcat(path, pathend);
        }
        else
        {
            std::string PATH = ros::package::getPath("strategy") + "/Parameter";
            strcpy(path, PATH.c_str());
            strcat(pathend, filename.c_str());
            strcat(path, pathend);
        }
        ofstream OutFile(path);
        ROS_INFO("SaveBegin");
        OutFile << "VectorCnt = ";
        OutFile << MotionSaveData.SaveMotionVector.size()-1;
        OutFile << "\n";
        for(int i = 0; i < MotionSaveData.SaveMotionVector.size()-1; i++)
        {
            switch(MotionSaveData.SaveMotionVector[i].motionstate)
            {
                case 0:
                    OutFile << "State = ";
                    OutFile << MotionSaveData.SaveMotionVector[i].motionstate;
                    OutFile << "\n";
                    OutFile << "ID = ";
                    OutFile << MotionSaveData.SaveMotionVector[i].ID;
                    OutFile << "\n";
                    for(int j = 0; j < MotionSaveData.SaveMotionVector[i].MotionList.size(); j++)
                    {
                        if(j%2 == 0)
                        {
                            OutFile << "A";
                            OutFile << (j/2)+1;
                            OutFile << " = ";
                            OutFile << MotionSaveData.SaveMotionVector[i].MotionList[j];
                            OutFile << "|";
                        }
                        else
                        {
                            OutFile << "D";
                            OutFile << (j/2)+1;
                            OutFile << " = ";
                            OutFile << MotionSaveData.SaveMotionVector[i].MotionList[j];
                            if(j == MotionSaveData.SaveMotionVector[i].MotionList.size()-1)
                            {
                                break;
                            }
                            else
                            {
                                OutFile << "|";
                            }
                        }
                    }
                    OutFile << "\n";
                    break;
                case 1:
                    OutFile << "State = ";
                    OutFile << MotionSaveData.SaveMotionVector[i].motionstate;
                    OutFile << "\n";
                    OutFile << "ID = ";
                    OutFile << MotionSaveData.SaveMotionVector[i].ID;
                    OutFile << "\n";
                    for(int j = 0; j < MotionSaveData.SaveMotionVector[i].MotorData.size(); j++)
                    {
                        OutFile << "M";
                        OutFile << j+1;
                        OutFile << " = ";
                        OutFile << MotionSaveData.SaveMotionVector[i].MotorData[j];
                        if(j == MotionSaveData.SaveMotionVector[i].MotorData.size()-1)
                        {
                            break;
                        }
                        else
                        {
                            OutFile << "|";
                        }
                    }
                    OutFile << "\n";
                    break;
                case 2:
                    OutFile << "State = ";
                    OutFile << MotionSaveData.SaveMotionVector[i].motionstate;
                    OutFile << "\n";
                    OutFile << "ID = ";
                    OutFile << MotionSaveData.SaveMotionVector[i].ID;
                    OutFile << "\n";
                    for(int j = 0; j < MotionSaveData.SaveMotionVector[i].MotorData.size(); j++)
                    {
                        OutFile << "M";
                        OutFile << j+1;
                        OutFile << " = ";
                        OutFile << MotionSaveData.SaveMotionVector[i].MotorData[j];
                        if(j == MotionSaveData.SaveMotionVector[i].MotorData.size()-1)
                        {
                            break;
                        }
                        else
                        {
                            OutFile << "|";
                        }
                    }
                    OutFile << "\n";
                    break;
                case 3:
                    OutFile << "State = ";
                    OutFile << MotionSaveData.SaveMotionVector[i].motionstate;
                    OutFile << "\n";
                    OutFile << "ID = ";
                    OutFile << MotionSaveData.SaveMotionVector[i].ID;
                    OutFile << "\n";
                    for(int j = 0; j < MotionSaveData.SaveMotionVector[i].MotorData.size(); j++)
                    {
                        OutFile << "M";
                        OutFile << j+1;
                        OutFile << " = ";
                        OutFile << MotionSaveData.SaveMotionVector[i].MotorData[j];
                        if(j == MotionSaveData.SaveMotionVector[i].MotorData.size()-1)
                        {
                            break;
                        }
                        else
                        {
                            OutFile << "|";
                        }
                    }
                    OutFile << "\n";
                    break;
                case 4:
                    OutFile << "State = ";
                    OutFile << MotionSaveData.SaveMotionVector[i].motionstate;
                    OutFile << "\n";
                    OutFile << "ID = ";
                    OutFile << MotionSaveData.SaveMotionVector[i].ID;
                    OutFile << "\n";
                    for(int j = 0; j < MotionSaveData.SaveMotionVector[i].MotorData.size(); j++)
                    {
                        OutFile << "M";
                        OutFile << j+1;
                        OutFile << " = ";
                        OutFile << MotionSaveData.SaveMotionVector[i].MotorData[j];
                        if(j == MotionSaveData.SaveMotionVector[i].MotorData.size()-1)
                        {
                            break;
                        }
                        else
                        {
                            OutFile << "|";
                        }
                    }
                    OutFile << "\n";
                    break;
            }               
        }
        ROS_INFO("SaveEnd");
        MotionSaveData.SaveMotionVector.clear();
        OutFile.close();
    }
}

void SimMotionPackage::InterfaceSend2SectorFunction(const tku_msgs::InterfaceSend2Sector &msg)
{
    SaveSectorPackage.push_back(msg.Package);
    int stamp = 0;
    int checksum_int = 0;
    int checksum_Lhand_int = 0;
    int checksum_Rhand_int = 0;
    int checksum_Lfoot_int = 0;
    int checksum_Rfoot_int = 0;
    uint8_t checksum_Lhand;
    uint8_t checksum_Rhand;
    uint8_t checksum_Lfoot;
    uint8_t checksum_Rfoot;
    int count = 0;
    int len = SaveSectorPackage.size();
    if (SaveSectorPackage[0] == 0x53 && SaveSectorPackage[1] == 0x54 && SaveSectorPackage[len-2] == 0x4E && SaveSectorPackage[len-1] == 0x45)
    {
        char pathend[20] = "/sector/";
        char pathend2[20] = ".ini";
        char path[200];
        string filename = msg.sectorname;
        // if(filename == "29")
        // {
        //     strcpy(path, STANDPATH);
        //     strcat(pathend, filename.c_str());
        //     strcat(path, pathend);
        //     strcat(path, pathend2);
        // }
        // else
        {
            std::string PATH = ros::package::getPath("strategy") + "/Parameter";
            strcpy(path, PATH.c_str());
            strcat(pathend, filename.c_str());
            strcat(path, pathend);
            strcat(path, pathend2);
        }
        ofstream OutFile(path);
        ROS_INFO("SaveSectorBegin");
        OutFile << "PackageCnt = ";
        OutFile << SaveSectorPackage[len-3];
        OutFile << "\n";
        OutFile << "Package = ";
        OutFile << SaveSectorPackage[2];
        OutFile <<"|| ";
        int pkgsum = 1;
        interface_ack.data = true;
        if(SaveSectorPackage[2] == 242 || SaveSectorPackage[2] == 243)
        {
            if(SaveSectorPackage[len-3] == 85)
            {
                for(int i = 3; i < SaveSectorPackage[len-3] + 2; i++)
                {
                    checksum_int += SaveSectorPackage[i];
                    if(i < 19)
                    {
                        checksum_Lhand_int = checksum_int;
                    }
                    else if(i < 35)
                    {
                        checksum_Rhand_int = checksum_int - checksum_Lhand_int;
                    }
                    else if(i < 63)
                    {
                        checksum_Lfoot_int = checksum_int - checksum_Lhand_int - checksum_Rhand_int;
                    }
                    else
                    {
                        checksum_Rfoot_int = checksum_int - checksum_Lhand_int - checksum_Rhand_int - checksum_Lfoot_int;
                    }
                }
                checksum_Lhand = checksum_Lhand_int & 0xff;
                checksum_Rhand = checksum_Rhand_int & 0xff;
                checksum_Lfoot = checksum_Lfoot_int & 0xff;
                checksum_Rfoot = checksum_Rfoot_int & 0xff;
                ROS_INFO("checksum_Lhand = %d",checksum_Lhand);
                ROS_INFO("checksum_Rhand = %d",checksum_Rhand);
                ROS_INFO("checksum_Lfoot = %d",checksum_Lfoot);
                ROS_INFO("checksum_Rfoot = %d",checksum_Rfoot);
                if(checksum_Lhand == SaveSectorPackage[len-7] && checksum_Rhand == SaveSectorPackage[len-6] && checksum_Lfoot == SaveSectorPackage[len-5] && checksum_Rfoot == SaveSectorPackage[len-4])
                {
                    ROS_INFO("Send sector is successful!!");
                    interface_ack.data = true;
                }
                else
                {
                    ROS_INFO("Send sector is fail!!");
                    interface_ack.data = false;
                }
            }
            else
            {
                ROS_INFO("Send sector is fail!!");
                interface_ack.data = false;
            }
            InterfaceCallBack_pub.publish(interface_ack);
        }
        for(int i = 3; i < SaveSectorPackage[len-3] + 2; i++)   //[0]&[1] is headpackage so +2 to save last package 
        {
            if(SaveSectorPackage[2] == 244)
            {
                if(SaveSectorPackage[i+1] == 68 && SaveSectorPackage[i+2] == 89)
                {
                    ROS_INFO("pkgsum = %d",pkgsum);
                    if(pkgsum == 84 || pkgsum == 87)
                    {
                        pkgsum = 1;
                            interface_ack.data = true;
                    }
                    else
                    {
                        ROS_INFO("Send sector is fail!!");
                        interface_ack.data = false;
                        InterfaceCallBack_pub.publish(interface_ack);
                    }
                }
                else
                {
                    if(i == SaveSectorPackage[len-3] + 1)
                    {
                        ROS_INFO("Send sector is successful!!");
                        InterfaceCallBack_pub.publish(interface_ack);
                    }
                    pkgsum++;
                }
            }
            if(!interface_ack.data)
            {
                break;
            }
            OutFile << SaveSectorPackage[i];
            //ROS_INFO("%d",SaveSectorPackage[i]);
            OutFile <<"|| ";
        }
        ROS_INFO("SaveSectorEnd");
        SaveSectorPackage.clear();
        OutFile.close();
    }
}

void SimMotionPackage::readStandFunction()
{
    ROS_INFO("read_stand_data");
    char pathend[20] = "/sector/";
    char pathend2[20] = ".ini";
    char path[200];
    int packagecnt;
    string PATH = ros::package::getPath("strategy") + "/Parameter";
    strcpy(path, PATH.c_str());
    strcat(pathend, "29");
    strcat(path, pathend);
    strcat(path, pathend2);
    fstream fin;
    fin.open(path, ios::in);
    if(!fin)
    {
        ROS_INFO("Sector 29 no exist Error!!");
    }
    else
    {
        try
        {
            packagecnt = tool->readvalue(fin, "PackageCnt", 0);
            SendSectorPackage.push_back(tool->readvalue(fin, "Package", 2));
            ROS_INFO("mode = %d",SendSectorPackage[0]);
            for(int i = 1; i < packagecnt; i++)
            {
                SendSectorPackage.push_back(tool->readvalue(fin, "|", 3));
            }
            for(int i = 1, j = 0; i < packagecnt; i+=4)
            {
                if(SendSectorPackage[i] == 68 && SendSectorPackage[i+1] == 89)
                {
                    stand_data.delay = SendSectorPackage[i+2];
                    ROS_INFO("delay_time = %d", stand_data.delay);
                    SectorControlFuntion(SendSectorPackage[0], stand_data);
                    stand_data.init();
                    i += 3;
                    j = 0;
                    if(SendSectorPackage[i] == 69 && SendSectorPackage[i+1] == 78)
                    {
                        execute_ack.data = true;
                        ExecuteCallBack_pub.publish(execute_ack);
                        SendSectorPackage.clear();
                        ROS_INFO("end_read_stand_data");
                        return;
                    }
                }
                if(j > 20)ROS_ERROR("MotorID over 21");
                stand_data.speed[j].L_D = SendSectorPackage[i];
                stand_data.speed[j].H_D = SendSectorPackage[i+1];

                stand_data.angle[j].L_D = SendSectorPackage[i+2];
                stand_data.angle[j].H_D = SendSectorPackage[i+3];
                ROS_INFO("stand_speed = %d\tstand_angle = %d", stand_data.speed[j].toDec(), stand_data.angle[j].toDec());
                j++;
            }
            // SectorControlFuntion((unsigned int)SectorMode::AbsoluteAngle, stand_data);
            execute_ack.data = true;
            ExecuteCallBack_pub.publish(execute_ack);
        }
        catch(exception e)
        {
        }
    }
    ROS_INFO("end_read_stand_data");
    SendSectorPackage.clear();
}

void SimMotionPackage::speedControl(SectorData &sector_data)
{
    // this->speed_control_cnts_max_count = 1;
    // for(int i = 0; i < MotorSum; i++)
    // {
    //     if(sector_data.speed[i].toDec() > 0)
    //     {
    //         if(sector_data.speed[i].toDec() > abs(sector_data.angle[i].toDec()))
    //         {
    //             this->speed_control_cnts[i] = 1;
    //         }
    //         else
    //         {
    //             this->speed_control_cnts[i] = sector_data.angle[i].toDec()/sector_data.speed[i].toDec();
    //             if(speed_control_cnts[i] > this->speed_control_cnts_max_count)this->speed_control_cnts_max_count = this->speed_control_cnts[i];
    //         }
    //     }
    //     else
    //     {
    //         this->speed_control_cnts[i] = 1;
    //     }
    // }

    // if(!this->speed_control_timer.isValid())
    // {
    //     this->speed_control_timer.stop();
    //     this->speed_control_cnts_count = 0;

    // }
    // this->speed_control_timer.start();
}

void SimMotionPackage::speedControlTimer(const ros::TimerEvent& e)
{
    // SectorData _temp_sector_data;
    // this->speed_control_cnts_count++;
    // for(int i = 0; i < MotorSum; i++)
    // {
    //     // _temp_sector_data.angle[i] = this->sector_data.angle[i]/speed_control_cnts[i];
    //     // this->sector_data.angle[i].remainder = (float)this->sector_data.angle[i].toDec()/speed_control_cnts[i] - (int)this->sector_data.angle[i].toDec()/speed_control_cnts[i];
    //     this->stand_data.angle[i] += _temp_sector_data.angle[i];
    // }
    // for(int i = 0; i < MotorSum; i++)this->motor_angle[i].data = this->stand_data.angle[i].toPI_G();
    // if(this->speed_control_cnts_count >= this->speed_control_cnts_max_count)this->speed_control_timer.stop();
}

int main(int argc, char *argv[])
{
	ros::init(argc, argv, "simmotionpackage");
	ros::NodeHandle nh;
	ros::NodeHandle nhPrivate("~");
	ros::CallbackQueue my_queue;
    nhPrivate.setCallbackQueue(&my_queue);
    ros::AsyncSpinner spinner(0,&my_queue);
    spinner.start();
    ros::AsyncSpinner s(1);
    s.start();
	SimMotionPackage simmotionpackage(nh, nhPrivate);
    // tool_gz->set_Client(nh);
    // tool_gz->get_modelproperties("kidsize");

	ros::Rate loop_rate(5);
	while(nh.ok())
	{
        // if(simmotionpackage.init_stand_times == 2)
        // {
        //     simmotionpackage.init_stand_times++;
        //     ROS_WARN("success to stand!");
        // }
        // else if(simmotionpackage.init_stand_times < 2)
        // {
        //     ROS_WARN("wait to stand...");
        //     simmotionpackage.init_stand_times++;
        //     simmotionpackage.SectorControlFuntion((unsigned int)SectorMode::AbsoluteAngle, simmotionpackage.stand_data);
        // }
		loop_rate.sleep();
	}

	return 0;
}