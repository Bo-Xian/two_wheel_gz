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

    motor_angle[9].data = parameterinfo->ik_parameters.Ltest_theta[5];
    motor_angle[10].data = parameterinfo->ik_parameters.Ltest_theta[0];//+Ladd;
    motor_angle[11].data = parameterinfo->ik_parameters.Ltest_theta[1]-0.105 + 0.25;//-0.105;
    motor_angle[12].data = parameterinfo->ik_parameters.Ltest_theta[2]+0.104 - 0.5;
    motor_angle[13].data = parameterinfo->ik_parameters.Ltest_theta[3]-0.045 + 0.25;
    motor_angle[14].data = -parameterinfo->ik_parameters.Ltest_theta[4];//-Ladd;

    motor_angle[15].data = -parameterinfo->ik_parameters.Rtest_theta[5];
    motor_angle[16].data = parameterinfo->ik_parameters.Rtest_theta[0];//+Radd;
    motor_angle[17].data = -(parameterinfo->ik_parameters.Rtest_theta[1]-0.105 + 0.25);//+0.105;
    motor_angle[18].data = -(parameterinfo->ik_parameters.Rtest_theta[2]+0.104 - 0.5);
    motor_angle[19].data = -(parameterinfo->ik_parameters.Rtest_theta[3]-0.045 + 0.25);
    motor_angle[20].data = -parameterinfo->ik_parameters.Rtest_theta[4]-0.1;//-Radd;

    // ROS_INFO("%f",motor_angle[12].data);
    // motor_angle[9].data = parameterinfo->ik_parameters.Ltest_theta[5];
    // motor_angle[10].data = parameterinfo->ik_parameters.Ltest_theta[0];//+Ladd;
    // motor_angle[11].data = parameterinfo->ik_parameters.Ltest_theta[1]-0.105 + 0.38;//-0.105;
    // motor_angle[12].data = parameterinfo->ik_parameters.Ltest_theta[2]+0.104 - 0.45*2;
    // motor_angle[13].data = parameterinfo->ik_parameters.Ltest_theta[3]-0.045 + 0.45;
    // motor_angle[14].data = -parameterinfo->ik_parameters.Ltest_theta[4];//-Ladd;

    // motor_angle[15].data = -parameterinfo->ik_parameters.Rtest_theta[5];
    // motor_angle[16].data = parameterinfo->ik_parameters.Rtest_theta[0];//+Radd;
    // motor_angle[17].data = -(parameterinfo->ik_parameters.Rtest_theta[1]-0.105 + 0.38);//+0.105;
    // motor_angle[18].data = -(parameterinfo->ik_parameters.Rtest_theta[2]+0.104 - 0.45*2);
    // motor_angle[19].data = -(parameterinfo->ik_parameters.Rtest_theta[3]-0.045 + 0.45);
    // motor_angle[20].data = -parameterinfo->ik_parameters.Rtest_theta[4]-0.1;//-Radd;

    for(int i = 9; i < 21; i++)motor_control[i].publish(this->motor_angle[i]);
    // time_end = ros::WallTime::now().toSec();
}

void SimMotionPackage::getHeadAngle(const tku_msgs::HeadPackage &msg)
{
    if(msg.ID == (int)HeadMotorID::pitch)
    {
        head_angle[(int)HeadMotorID::pitch].data = (msg.Position-2047)/4096.0*3.14159265;
        head_control[(int)HeadMotorID::pitch].publish(head_angle[(int)HeadMotorID::pitch]);
    }
    else
    {
        head_angle[(int)HeadMotorID::yaw].data = (msg.Position-2047)/4096.0*3.14159265;
        head_control[(int)HeadMotorID::yaw].publish(head_angle[(int)HeadMotorID::yaw]);
    }
}

void SimMotionPackage::SectorControlFuntion()
{

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
    SectorData sector_data;
    if(msg.data == 29)
    {
        strcpy(path, STANDPATH);
        strcat(pathend, filename);
        strcat(path, pathend);
        strcat(path, pathend2);
    }
    else
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
                    sector_data.delay = SendSectorPackage[i+2];
                    ROS_INFO("delay_time = %d", sector_data.delay);
                    sector_data.init();
                    i += 3;
                    j = 0;
                    if(SendSectorPackage[i] == 69 && SendSectorPackage[i+1] == 78)
                    {
                        break;
                    }
                }
                SectorDataBase speed_data_base, angle_data_base;
                speed_data_base.L_D = SendSectorPackage[i];
                speed_data_base.H_D = SendSectorPackage[i+1];
                angle_data_base.L_D = SendSectorPackage[i+2];
                angle_data_base.H_D = SendSectorPackage[i+3];
                sector_data.speed.push_back(speed_data_base);
                sector_data.angle.push_back(angle_data_base);
                // ROS_INFO("speed.L_D = %d, speed.H_D = %d", sector_data.speed[j].L_D, sector_data.speed[j].H_D);
                // ROS_INFO("angle.L_D = %d, angle.H_D = %d", sector_data.angle[j].L_D, sector_data.angle[j].H_D);
                ROS_INFO("speed = %d\tangle = %d", sector_data.speed[j].D(), sector_data.angle[j].D());
                j++;
            }
            execute_ack.data = true;
            ExecuteCallBack_Publish.publish(execute_ack);
            ROS_INFO("Execute is finsih");
        }
        catch(exception e)
        {
        }
    }
    SendSectorPackage.clear();
    ROS_INFO("End_LoadSector");
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
	ros::Rate loop_rate(50);
	while(nh.ok())
	{
		loop_rate.sleep();
	}

	return 0;
}