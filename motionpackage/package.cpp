#include "package.h"
#include <ros/callback_queue.h>

static void head_callback(int id, uint8_t *buf, int length)
{

}
static void IMU_callback(int id, uint8_t *buf, int length)
{
    for(int i=0; i<length; i++)
    {
        sensor_data_buf[sensor_data_buf_cnt++] = buf[i];
        if(sensor_data_buf_cnt >= SENSOR_BUF_SIZE)
        {
            sensor_data_buf_cnt = 0;
            isBufFull = true;
            Sensor_Data_Process();
            break;
        }
    }
}
static void mcssl_callback(int id, uint8_t *buf, int length)
{
    static int dio_tmpstatus = 0;
    static bool walkdata_receive = false;
    ROS_INFO("FPGAack is :%x %x %x %x",buf[0],buf[1],buf[2],buf[3]);
    for(int i = 0; i < 4; i++)
    {
        switch (dio_tmpstatus) {
        case 0:
            if(buf[i] == 'S')
                dio_tmpstatus = 1;
            break;
        case 1:
            if(buf[i] == 'U')
                dio_tmpstatus = 2;

            if(buf[i] == 'T')
                dio_tmpstatus = 4;
            break;
        case 2:
            dio_tmpstatus = 3;
            FPGAack.data = buf[i];
            FPGAack_Publish.publish(FPGAack);
            break;
        case 3:
            if(buf[i] == 'E')
            {
                dio_tmpstatus = 0;
                if(walkdata_receive)
                {
                    walkack.data = true;
                    walkack_Publish.publish(walkack);
                }
            }
            break;
        case 4:
            if(buf[i] == 'Y')
            {
                InterfaceFlag = 1;
            }
            else if(buf[i] == 'N')
            {
                InterfaceFlag = 0;
                SendSectorPackage.clear();
            }
            else if(buf[i] == 0xF5)
            {
                walkdata_receive = true;
            }
            else
            {
                walkdata_receive = false;
            }
            dio_tmpstatus = 3;
            break;
        }
    }
}
int mcssl_init(/*const char *devs*/)
{
    char *devs;
    char *devs_head;
    char *devs_IMU;
    cssl_start();
    if (!serial){
        devs="/dev/ttyUSB0";
        serial=cssl_open(devs, mcssl_callback, 0, 115200, 8, 0, 1);
    }
    if (!serial_head){
        devs_head="/dev/ttyS1";
        serial_head=cssl_open(devs_head, head_callback, 0, 115200, 8, 0, 1);
    }
    if (!serial_IMU){
        devs_IMU="/dev/ttyS0";
        serial_IMU=cssl_open(devs_IMU, IMU_callback, 0, 115200, 8, 0, 1);
    }
    ROS_INFO("Initialize Motion with port=%s...",devs);
    ROS_INFO("Initialize Head with port=%s...",devs_head);
    ROS_INFO("Initialize IMU with port=%s...",devs_IMU);
    if (!serial){
        printf("%s\n",cssl_geterrormsg());
        printf("---> Motion RS232 OPEN FAIL <---\n");
        fflush(stdout);
        return -1;
    }
    if (!serial_head){
        printf("%s\n",cssl_geterrormsg());
        printf("---> Head RS232 OPEN FAIL <---\n");
        fflush(stdout);
        return -1;
    }
    if (!serial_IMU){
        printf("%s\n",cssl_geterrormsg());
        printf("---> Head RS232 OPEN FAIL <---\n");
        fflush(stdout);
        return -1;
    }
    cssl_setflowcontrol(serial, 0, 0);
    cssl_setflowcontrol(serial_head, 0, 0);
    cssl_setflowcontrol(serial_IMU, 0, 0);
    return 1;
}
void mcssl_finish(){

    cssl_close(serial);
    cssl_close(serial_head);
    cssl_close(serial_IMU);
    cssl_stop();
}
//------------------------
void motorinit()
{
    motorpackage[0] = 0x53;
    motorpackage[1] = 0x54;
    motorpackage[2] = 0xF5;
    motorpackage[5] = 5;
    motorpackage[26] = 0x45;
}
void mcssl_send2motor(double Rx, double Ry, double Rz, double Rthta, double Lx, double Ly, double Lz, double Lthta, int Sampletime, int Walking_State, int Sensor_Mode)
{
    int Rx_h, Rx_l, Ry_h, Ry_l, Rz_h, Rz_l, Rthta_h, Rthta_l;
    int Lx_h, Lx_l, Ly_h, Ly_l, Lz_h, Lz_l, Lthta_h, Lthta_l;
    int sampletime_h, sampletime_l;
    Rx_h = (int)(((int)(Rx * 1000) >> 8) & 0xFF);
    Rx_l = (int)((int)(Rx * 1000) & 0xFF);
    Ry_h = (int)(((int)(Ry * 1000) >> 8) & 0xFF);
    Ry_l = (int)((int)(Ry * 1000) & 0xFF);
    Rz_h = (int)(((int)(Rz * 1000) >> 8) & 0xFF);
    Rz_l = (int)((int)(Rz * 1000) & 0xFF);
    Rthta_h = (int)(((int)(Rthta * 1000) >> 8) & 0xFF);
    Rthta_l = (int)((int)(Rthta * 1000) & 0xFF);
    Lx_h = (int)(((int)(Lx * 1000) >> 8) & 0xFF);
    Lx_l = (int)((int)(Lx * 1000) & 0xFF);
    Ly_h = (int)(((int)(Ly * 1000) >> 8) & 0xFF);
    Ly_l = (int)((int)(Ly * 1000) & 0xFF);
    Lz_h = (int)(((int)(Lz * 1000) >> 8) & 0xFF);
    Lz_l = (int)((int)(Lz * 1000) & 0xFF);
    Lthta_h = (int)(((int)(Lthta * 1000) >> 8) & 0xFF);
    Lthta_l = (int)((int)(Lthta * 1000) & 0xFF);
    sampletime_h = (int)((int)(Sampletime >> 8) & 0xFF);
    sampletime_l = (int)(int)(Sampletime & 0xFF);
    motorpackage[6] = Rx_h;
    motorpackage[7] = Rx_l;
    motorpackage[8] = Ry_h;
    motorpackage[9] = Ry_l;
    motorpackage[10] = Rz_h;
    motorpackage[11] = Rz_l;
    motorpackage[12] = Rthta_h;
    motorpackage[13] = Rthta_l;
    motorpackage[14] = Lx_h;
    motorpackage[15] = Lx_l;
    motorpackage[16] = Ly_h;
    motorpackage[17] = Ly_l;
    motorpackage[18] = Lz_h;
    motorpackage[19] = Lz_l;
    motorpackage[20] = Lthta_h;
    motorpackage[21] = Lthta_l;
    motorpackage[22] = Walking_State;
    motorpackage[23] = Sensor_Mode;
    motorpackage[24] = sampletime_h;
    motorpackage[25] = sampletime_l;
    cssl_putdata(serial, motorpackage, 27);
}
/*bool motionCallback(tku_msgs::IKinfo::Request &req, tku_msgs::IKinfo::Response &res)
{
    double Rx, Ry, Rz, Rthta, Lx, Ly, Lz, Lthta;
    int sampletime;
    Rx = req.IK_Point_RX;
    Ry = req.IK_Point_RY;
    Rz = req.IK_Point_RZ;
    Rthta = req.IK_Point_RThta;
    Lx = req.IK_Point_LX;
    Ly = req.IK_Point_LY;
    Lz = req.IK_Point_LZ;
    Lthta = req.IK_Point_LThta;
    sampletime = req.Sampletime;
    mcssl_send2motor(Rx, Ry, Rz, Rthta, Lx, Ly, Lz, Lthta, sampletime);
    res.SentData = true;
    return true;
}*/
void motionCallback (const tku_msgs::IKinfo_message& msg)
{
    double Rx, Ry, Rz, Rthta, Lx, Ly, Lz, Lthta;
    int sampletime, walking_state, sensor_mode;
    Rx = msg.IK_Point_RX;
    Ry = msg.IK_Point_RY;
    Rz = msg.IK_Point_RZ;
    Rthta = msg.IK_Point_RThta;
    Lx = msg.IK_Point_LX;
    Ly = msg.IK_Point_LY;
    Lz = msg.IK_Point_LZ;
    Lthta = msg.IK_Point_LThta;
    sampletime = msg.Sampletime;
    walking_state = msg.Walking_State;
    sensor_mode = msg.Sensor_Mode;

    mcssl_send2motor(Rx, Ry, Rz, Rthta, Lx, Ly, Lz, Lthta, sampletime, walking_state, sensor_mode);
}
//---head package---//
void RobotisListini()
{
    RobotisList.clear();
    for(int i = 1; i < 3; i++)
    {
        tsRobotis motor;
        motor.ID = i;
        motor.GoalPosition = 2048;
        motor.Speed = 511;
        RobotisList.push_back(motor);
    }
}
void SYNC_WRITE()
{
    unsigned short blk_size;
    unsigned short crc_value;
///////////////////////////////////////torque///////////////////////////////////////
    // Header
    torquePackage[0] = 0xFF;
    torquePackage[1] = 0xFF;
    torquePackage[2] = 0xFD;
    // Reserved
    torquePackage[3] = 0x00;
    // ID
    torquePackage[4] = 0xFE;
    // Length      The length after the Packet Length field (Instruction, Parameter, CRC fields). Packet Length = number of Parameters + 3
    torquePackage[5] = 0x06;
    torquePackage[6] = 0;
    // Instruction
    torquePackage[7] = 0x03;      //write         // 0x83 = sync write

    // Parameter
    torquePackage[8] = 0x40;      // addressL             // Velocity: 0x70 = 112 Position: 0x74(hex) = 116(dec)
    torquePackage[9] = 0;

    torquePackage[10] = 0x01;      // data length(byte)

    blk_size = 5 + torquePackage[5];
    crc_value = update_crc(0,torquePackage,blk_size);

    torquePackage[11] = (crc_value << 8) >> 8;
    torquePackage[12] = crc_value >> 8;

    

    for(int cnt = 0; cnt < 13; cnt++)
    {
        cssl_putchar(serial_head,torquePackage[cnt]);
    }
    //ROS_INFO("torquePackage: %d",torquePackage[14]);
///////////////////////////////////////torque///////////////////////////////////////
    tool->Delay(1);
    // Header
    HeadPackage[0] = 0xFF;
    HeadPackage[1] = 0xFF;
    HeadPackage[2] = 0xFD;
    // Reserved
    HeadPackage[3] = 0x00;
    // ID
    HeadPackage[4] = 0xFE;
    // Length      The length after the Packet Length field (Instruction, Parameter, CRC fields). Packet Length = number of Parameters + 3
    HeadPackage[5] = 0x19;
    HeadPackage[6] = 0;
    // Instruction
    HeadPackage[7] = 0x83;      //write         // 0x83 = sync write
    // Parameter
    HeadPackage[8] = 0x70;      // addressL             // Velocity: 0x70 = 112 Position: 0x74(hex) = 116(dec)
    HeadPackage[9] = 0;

    HeadPackage[10] = 0x08;      // data length(byte)
    HeadPackage[11] = 0;

    HeadPackage[12] = RobotisList[0].ID;      //MotorID
    HeadPackage[13] = RobotisList[0].Speed & 0xFF;
    HeadPackage[14] = (RobotisList[0].Speed >> 8) & 0xFF;
    HeadPackage[15] = (RobotisList[0].Speed >> 16) & 0xFF;
    HeadPackage[16] = (RobotisList[0].Speed >> 24) & 0xFF;
    HeadPackage[17] = RobotisList[0].GoalPosition & 0xFF;
    HeadPackage[18] = (RobotisList[0].GoalPosition >> 8) & 0xFF;
    HeadPackage[19] = (RobotisList[0].GoalPosition >> 16) & 0xFF;
    HeadPackage[20] = (RobotisList[0].GoalPosition >> 24) & 0xFF;

    HeadPackage[21] = RobotisList[1].ID;      //MotorID
    HeadPackage[22] = RobotisList[1].Speed & 0xFF;
    HeadPackage[23] = (RobotisList[1].Speed >> 8) & 0xFF;
    HeadPackage[24] = (RobotisList[1].Speed >> 16) & 0xFF;
    HeadPackage[25] = (RobotisList[1].Speed >> 24) & 0xFF;
    HeadPackage[26] = RobotisList[1].GoalPosition & 0xFF;
    HeadPackage[27] = (RobotisList[1].GoalPosition >> 8) & 0xFF;
    HeadPackage[28] = (RobotisList[1].GoalPosition >> 16) & 0xFF;
    HeadPackage[29] = (RobotisList[1].GoalPosition >> 24) & 0xFF;

    blk_size = 5 + HeadPackage[5];
    crc_value = update_crc(0,HeadPackage,blk_size);

    HeadPackage[30] = (crc_value << 8) >> 8;
    HeadPackage[31] = crc_value >> 8;
    
    for(int cnt = 0; cnt < 32; cnt++)
    {
        cssl_putchar(serial_head,HeadPackage[cnt]);
    }
}
void HeadMotorFunction(const tku_msgs::HeadPackage &msg)
{
    double tmpTime = abs(RobotisList[msg.ID -1].GoalPosition - msg.Position) * (AX12Velocity / msg.Speed);
    do
    {
        gettimeofday(&tend, NULL);
        timeuse = (1000000*(tend.tv_sec - tstart.tv_sec) + (tend.tv_usec - tstart.tv_usec));
    }while(timeuse < tmpTime);
    RobotisList[msg.ID - 1].GoalPosition = msg.Position;
    RobotisList[msg.ID - 1].Speed = msg.Speed;
    SYNC_WRITE();
    gettimeofday(&tend, NULL);
}
//---head package---//
//---Sector package---//

void Standini()
{
    for(int i =0; i < 87; i++)
    {
        packageMotorData[i] = 0;                            
    } 
    SendSectorPackage.clear();
    ROS_INFO("Standini");
    char pathend[20] = "/sector/";
    char pathend2[20] = "29.ini";
    char path[200];
    int packagecnt;
    int cnt = 3;
    strcpy(path, STANDPATH);
    strcat(path, pathend);
    strcat(path, pathend2);
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
            for(int i = 1; i < packagecnt; i++)
            {
                SendSectorPackage.push_back(tool->readvalue(fin, "|", 3));
                //ROS_INFO("%d",SendSectorPackage[i]);
            }
        }
        catch(exception e)
        {
        }
        packageMotorData[0] = 0x53;
        packageMotorData[1] = 0x54;
        packageMotorData[2] = 0xF2;
        for(int i =1; i < packagecnt; i++)
        {
            packageMotorData[cnt++] = SendSectorPackage[i];                            
        }      
        /*for(int j = 0; j < cnt; j++)
        {
            ROS_INFO("packageMotorData: %x",packageMotorData[j]);
        }*/
        cssl_putdata(serial, packageMotorData, cnt);
    }
    ROS_INFO("End_LoadSector");
    SendSectorPackage.clear();
}

void SectorSend2FPGAFunction(const std_msgs::Int16 &msg)
{
    for(int i =0; i < 87; i++)
    {
        packageMotorData[i] = 0;                            
    } 
    SendSectorPackage.clear();
    ROS_INFO("SendSectorPackage");
    char filename[10];
    sprintf(filename,"%d",msg.data);
    char pathend[20] = "/sector/";
    char pathend2[20] = ".ini";
    char path[200];
    int packagecnt;
    int cnt = 3;
    if(msg.data == 29)
    {
        strcpy(path, STANDPATH);
        strcat(pathend, filename);
        strcat(path, pathend);
        strcat(path, pathend2);
    }
    else
    {
        std::string PATH = ros::package::getPath("strategy") + "/Parameter";
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
        }
        catch(exception e)
        {
        }
        switch(SendSectorPackage[0])
        {
            case 242:
                packageMotorData[0] = 0x53;
                packageMotorData[1] = 0x54;
                packageMotorData[2] = 0xF2;
                for(int i =1; i < packagecnt; i++)
                {
                    packageMotorData[cnt++] = SendSectorPackage[i];                            
                }      
                cssl_putdata(serial, packageMotorData, cnt);
                execute_ack.data = true;
                ExecuteCallBack_Publish.publish(execute_ack);
                ROS_INFO("Execute is finsih");
                break;
            case 243:
                packageMotorData[0] = 0x53;
                packageMotorData[1] = 0x54;
                packageMotorData[2] = 0xF3;
                for(int i =1; i < packagecnt; i++)
                {
                    packageMotorData[cnt++] = SendSectorPackage[i];                       
                } 
                cssl_putdata(serial, packageMotorData, cnt);
                execute_ack.data = true;
                ExecuteCallBack_Publish.publish(execute_ack);
                ROS_INFO("Execute is finsih");
                break;
            case 244:
                bool motionlist_flag = true; 
                int cnt_tmp = 1;
                while(motionlist_flag)
                {
                    packageMotorData[0] = 0x53;
                    packageMotorData[1] = 0x54;
                    packageMotorData[2] = 0xF3;
                    for(int i = cnt_tmp; i < packagecnt; i++)
                    {
                        packageMotorData[cnt++] = SendSectorPackage[i]; 
                        if(SendSectorPackage[i+1] == 68 && SendSectorPackage[i+2] == 89)
                        {
                            cnt_tmp = i + 4;
                            ROS_INFO("Delay: %d",SendSectorPackage[i + 3]);
                            if(SendSectorPackage[cnt_tmp] == 69 && SendSectorPackage[cnt_tmp + 1] == 78)
                            {
                                motionlist_flag = false;
                            }
                            /*for(int j = 0; j < cnt; j++)
                            {
                                ROS_INFO("packageMotorData: %x",packageMotorData[j]);
                            }*/
                            cssl_putdata(serial, packageMotorData, cnt);
                            tool->Delay(SendSectorPackage[i + 3]);
                            cnt = 3;
                            break;  
                        }                    
                    } 
                }
                execute_ack.data = true;
                ExecuteCallBack_Publish.publish(execute_ack);
                ROS_INFO("Execute is finsih");
                break;
        }
    }
    ROS_INFO("End_LoadSector");
    SendSectorPackage.clear();
    /*sectorpackage[3] = msg.data;
    cssl_putdata(serial, sectorpackage, 5);
    ROS_INFO("SendSector: %d",sectorpackage[3]);*/
}
//---Sector package---//
//---------Interface Save---------//
unsigned short update_crc(unsigned short crc_accum, unsigned char *data_blk_ptr, unsigned short data_blk_size)
{ 
    unsigned short i, j;
    unsigned short crc_table[256] = {
        0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
        0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
        0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
        0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
        0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
        0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
        0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
        0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
        0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
        0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
        0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
        0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
        0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
        0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
        0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
        0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
        0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
        0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
        0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
        0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
        0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
        0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
        0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
        0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
        0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
        0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
        0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
        0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
        0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
        0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
        0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
        0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202
    };

    for(j = 0; j < data_blk_size; j++)
    {
        i = ((unsigned short)(crc_accum >> 8) ^ data_blk_ptr[j]) & 0xFF;
        crc_accum = (crc_accum << 8) ^ crc_table[i];
    }

    return crc_accum;
}
void InterfaceSaveDataFunction(const tku_msgs::SaveMotion &msg)
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
//---------Interface Save---------//
//---------Interface Read---------//
bool InterfaceReadDataFunction(tku_msgs::ReadMotion::Request &Motion_req, tku_msgs::ReadMotion::Response &Motion_res)
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
//---------Interface Read---------//
//---interfaceSendSector package---//
void InterfaceSend2SectorFunction(const tku_msgs::InterfaceSend2Sector &msg)
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
        if(filename == "29")
        {
            strcpy(path, STANDPATH);
            strcat(pathend, filename.c_str());
            strcat(path, pathend);
            strcat(path, pathend2);
        }
        else
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
            InterfaceCallBack_Publish.publish(interface_ack);
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
                        InterfaceCallBack_Publish.publish(interface_ack);
                    }
                }
                else
                {
                    if(i == SaveSectorPackage[len-3] + 1)
                    {
                        ROS_INFO("Send sector is successful!!");
                        InterfaceCallBack_Publish.publish(interface_ack);
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

void MotorSpeedFunction(const tku_msgs::SandHandSpeed &msg)
{
    uint8_t H,L;

    handspeedpackage.clear();

    ROS_INFO("ReadHandMotionStart");
    char filename[10];
    char path[200];
    sprintf(filename,"%d",msg.sector);
    char pathend[20] = "/sector/";
    char pathend2[20] = ".ini";
    std::string PATH = ros::package::getPath("strategy") + "/Parameter";
    strcpy(path, PATH.c_str());
    strcat(pathend, filename);
    strcat(path, pathend);
    strcat(path, pathend2);
    int packagecnt;
    int cnt = 3;
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
            handspeedpackage.push_back(packagecnt);
            handspeedpackage.push_back(tool->readvalue(fin, "Package", 2));
            ROS_INFO("mode = %d",handspeedpackage[1]);
            for(int i = 1; i < packagecnt; i++)
            {
                handspeedpackage.push_back(tool->readvalue(fin, "|", 3));
            }
        }
        catch(exception e)
        {
        }
    }
    ROS_INFO("End_LoadSector");

    L = msg.speed & 0xFF;
    H = msg.speed>>8 & 0xFF;

    handspeedpackage[18] = L;
    handspeedpackage[19] = H;
    handspeedpackage[30] = L;
    handspeedpackage[31] = H;
    handspeedpackage[46] = L;
    handspeedpackage[47] = H;
    handspeedpackage[70] = L;
    handspeedpackage[71] = H;

    handspeedpackage[133] = L;
    handspeedpackage[134] = H;
    handspeedpackage[137] = L;
    handspeedpackage[138] = H;
    handspeedpackage[141] = L;
    handspeedpackage[142] = H;
    handspeedpackage[157] = L;
    handspeedpackage[158] = H;
    handspeedpackage[161] = L;
    handspeedpackage[162] = H;
    handspeedpackage[165] = L;
    handspeedpackage[166] = H;

    handspeedpackage[220] = L;
    handspeedpackage[221] = L;
    handspeedpackage[244] = L;
    handspeedpackage[245] = H;

    ofstream OutFile(path);
    ROS_INFO("SendhandspeedBegin");
    OutFile << "PackageCnt = ";
    OutFile << handspeedpackage[0];
    OutFile << "\n";
    OutFile << "Package = ";
    for(int i = 1; i < handspeedpackage[0] + 1; i++)   //[0]&[1] is headpackage so +2 to save last package 
    {
        OutFile << handspeedpackage[i];
        ROS_INFO("%d",handspeedpackage[i]);
        OutFile <<"|| ";
    }
    ROS_INFO("SendhandspeedEnd");
    OutFile.close();

    handspeedpackage.clear();
}

void SingleMotorFunction(const tku_msgs::SingleMotorData &msg)
{
    uint8_t Angle_H,Angle_L,Speed_H,Speed_L,checksum = 0;
    int position;
    int cnt = 0;
    position = abs(msg.Position);
    if(msg.Position < 0)
    {
        Angle_L = position & 0xFF;
        Angle_H = (position >>8 & 0xFF)+128;
    }
    else
    {
        Angle_L = position & 0xFF;
        Angle_H = position >>8 & 0xFF;
    }
    Speed_L = msg.Speed & 0xFF;
    Speed_H = msg.Speed>>8 & 0xFF;

    onemotorpackage[0] = 0x53;
    onemotorpackage[1] = 0x54;
    onemotorpackage[2] = 0xF3;

    for(cnt = 3; cnt < (msg.ID - 1) * 4 + 3; cnt++)
    {
        onemotorpackage[cnt] = 0;
    }

    onemotorpackage[cnt++] = Speed_L;
    onemotorpackage[cnt++] = Speed_H;
    onemotorpackage[cnt++] = Angle_L;
    onemotorpackage[cnt++] = Angle_H;

    for(int i = cnt; i < 87; i++)
    {
        onemotorpackage[i] = 0;
    }

    for(int i = 0; i < 87; i++)
    {
        ROS_INFO("onemotorpackage = %d",onemotorpackage[i]);
    }
    cssl_putdata(serial, onemotorpackage, 87);
}
//---HandSpeed package---//

//---Receive & Process Sensor Data 
void Sensor_Data_Process()
{
    uint16_t Sensor_Data_tmp[11];
    double IMU_Value[3];
    uint16_t ForceSensor_Value[8];
    int Sensor_Data_Count = 0;
    int ForceSensor_Count = 0;
    bool isDataOk = false;
    tku_msgs::SensorPackage sensorpackage;

    isBufFull = false;

    for(int i=0; i<SENSOR_BUF_SIZE; i++)
    {
        if(sensor_data_buf[i] == 0x53 && sensor_data_buf[i+1] == 0x54 && sensor_data_buf[i+2] == 0xF7 && sensor_data_buf[i+27] == 0x45)
        {
            Sensor_Data_Count = i+3;
            isDataOk = true;
            break;
        }
    }
    if(isDataOk)
    {
        for(int i=0; i<3; i++)
        {
            Sensor_Data_tmp[i] = ((sensor_data_buf[Sensor_Data_Count++] << 8) | (sensor_data_buf[Sensor_Data_Count++]));
            if(Sensor_Data_tmp[i] & 0x8000) //negative
            {
                IMU_Value[i] = (double)((Sensor_Data_tmp[i] & 0x7FFF) * (-1)) / 100.0;
            }
            else                            //positive
            {
                IMU_Value[i] = (double)(Sensor_Data_tmp[i]) / 100.0;
            }
            sensorpackage.IMUData.push_back(IMU_Value[i]);
        }
        Sensor_Data_Count+=2;
        for(int i=0; i<8; i++)
        {
            ForceSensor_Value[i] = ((double)((sensor_data_buf[Sensor_Data_Count++] << 8) | (sensor_data_buf[Sensor_Data_Count++])));
            sensorpackage.ForceSensorData.push_back(ForceSensor_Value[i]);
        }
        Sensorpackage_Publish.publish(sensorpackage);
        isDataOk = false;
        //ROS_INFO("Roll = %2.2f\tPitch = %2.2f", IMU_Value[0], IMU_Value[1]);
        //ROS_INFO("L1 = %d\tL2 = %d", ForceSensor_Value[0], ForceSensor_Value[1]);
    }
    sensorpackage.IMUData.clear();
    sensorpackage.ForceSensorData.clear();
}

void SensorSetFunction(const tku_msgs::SensorSet &msg)
{
    //for test variable     // 18.32 * 100 = 1832
    int Desire_Roll = msg.Roll;
    int Desire_Pitch = msg.Pitch;
    int Desire_Yaw = msg.Yaw;
    bool Sensor_Request = msg.SensorReq;
    bool ForceSensor_Offset_Reset = msg.ForceOffsetReset;
    bool ForceState = msg.ForceState;
    //

    sensorsetpackage[0] = 0x53;
    sensorsetpackage[1] = 0x54;
    sensorsetpackage[2] = 0xF6;

    if(Desire_Roll < 0)
    {
        Desire_Roll = ~(Desire_Roll) + 1;
        sensorsetpackage[3] = ((Desire_Roll >> 8) & 0xFF) | 0x80;
        sensorsetpackage[4] = Desire_Roll & 0xFF;
    }
    else
    {
        sensorsetpackage[3] = (Desire_Roll >> 8) & 0xFF;
        sensorsetpackage[4] = Desire_Roll & 0xFF;
    }
    if(Desire_Pitch < 0)
    {
        Desire_Pitch = ~(Desire_Pitch) + 1;
        sensorsetpackage[5] = ((Desire_Pitch >> 8) & 0xFF) | 0x80;
        sensorsetpackage[6] = Desire_Pitch & 0xFF;
    }
    else
    {
        sensorsetpackage[5] = (Desire_Pitch >> 8) & 0xFF;
        sensorsetpackage[6] = Desire_Pitch & 0xFF;
    }
    if(Desire_Yaw < 0)
    {
        Desire_Yaw = ~(Desire_Yaw) + 1;
        sensorsetpackage[7] = ((Desire_Yaw >> 8) & 0xFF) | 0x80;
        sensorsetpackage[8] = Desire_Yaw & 0xFF;
    }
    else
    {
        sensorsetpackage[7] = (Desire_Yaw >> 8) & 0xFF;
        sensorsetpackage[8] = Desire_Yaw & 0xFF;
    }
    ROS_INFO("%x",sensorsetpackage[3]);
    ROS_INFO("%x",sensorsetpackage[4]);
    sensorsetpackage[9] = (ForceState << 2) | (ForceSensor_Offset_Reset << 1) | Sensor_Request; //Sensor_Request
    
    sensorsetpackage[10] = 0;    //Reserve
    sensorsetpackage[11] = 0x45;


    cssl_putdata(serial, sensorsetpackage, SENSOR_SET_PACKAGE_SIZE);
}

/*==============================================================================*/
//Main
/*==============================================================================*/
int main(int argc, char **argv)
{
    RobotisListini();
    motorinit();
    //MotorSpeedini();
    //Initial
    ros::init(argc, argv, "motionpackage");
    ros::NodeHandle n("~");
	ros::NodeHandle nh;
	ros::CallbackQueue my_queue;
	nh.setCallbackQueue(&my_queue);
	ros::AsyncSpinner spinner(1,&my_queue);
	spinner.start();
    ros::AsyncSpinner s(4);
	s.start();
    //ros::ServiceServer motion_service = n.advertiseService("/package/walkingdata", motionCallback);
    ros::Subscriber motion_sub = nh.subscribe("/package/walkingdata", 1000, motionCallback);
    ros::ServiceServer InterfaceReadData_service = n.advertiseService("/package/InterfaceReadSaveMotion",InterfaceReadDataFunction);
    
    ros::Subscriber headmotor_subscribe = n.subscribe("/package/HeadMotor", 1000, HeadMotorFunction);
    ros::Subscriber SectorSend2FPGA_subscribe = n.subscribe("/package/Sector", 1000, SectorSend2FPGAFunction);
    ros::Subscriber motorspeed_subscribe = n.subscribe("/package/motorspeed", 1000, MotorSpeedFunction);
    ros::Subscriber SingleMotorData_subscribe = n.subscribe("/package/SingleMotorData", 1000, SingleMotorFunction);
    ros::Subscriber InterfaceSend2Sector = n.subscribe("/package/InterfaceSend2Sector", 1000, InterfaceSend2SectorFunction);
    ros::Subscriber InterfaceSaveData_Subscribe = n.subscribe("/package/InterfaceSaveMotion", 1000, InterfaceSaveDataFunction);
    ros::Subscriber SensorSet_Subscribe = n.subscribe("/web/sensorset", 1000, SensorSetFunction);
    
    FPGAack_Publish = n.advertise<std_msgs::Int16>("/package/FPGAack", 1000);
    walkack_Publish = n.advertise<std_msgs::Bool>("/package/walkack", 1000);
    InterfaceCallBack_Publish = n.advertise<std_msgs::Bool>("/package/motioncallback", 1000);
    ExecuteCallBack_Publish = n.advertise<std_msgs::Bool>("/package/executecallback", 1000);
    Sensorpackage_Publish = n.advertise<tku_msgs::SensorPackage>("/package/sensorpackage", 1000);
    
    gettimeofday(&tstart, NULL);
    do{

        if(mcssl_init() > 0)
        {
            Standini();
            break;
        }else
        {
            usleep(1000000);//1s = 1,000,000 us
        }

    }while(ros::ok());
    ROS_INFO("Motion is running\n");
    usleep(1000000);
    ros::Rate loop_rate(50);
    bool test = true;
    while(ros::ok())
    {
        if(test)
        {

            test = false;
        }
        //ros::spinOnce();
        loop_rate.sleep();
    }
    mcssl_finish();

    return 0;
}
