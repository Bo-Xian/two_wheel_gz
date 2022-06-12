#include "tku_libs/strategy_info.h"
StrategyInfo* strategyinfo = new StrategyInfo();

StrategyInfo::StrategyInfo()
{
    getLabelModelFlag = false;
    getimage = false;
    this->Label_Model = new unsigned char [320 * 240];
    for(int i = 0; i < 8; i++)
    {
        this->ColorMaskSubjectCnt[i] = 0;
        for(int j = 0; j < this->ColorMaskSubjectCnt[i]; j++)
        {
            this->ColorMaskSubject[i][j].X = 0;
            this->ColorMaskSubject[i][j].Y = 0;
            this->ColorMaskSubject[i][j].XMin = 0;
            this->ColorMaskSubject[i][j].YMin = 0;
            this->ColorMaskSubject[i][j].XMax = 0;
            this->ColorMaskSubject[i][j].YMax = 0;
            this->ColorMaskSubject[i][j].Width = 0;
            this->ColorMaskSubject[i][j].Height = 0;
            this->ColorMaskSubject[i][j].size = 0;
        }
    }
}
