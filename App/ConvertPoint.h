#ifndef __ConvertPoint_H
#define __ConvertPoint_H

#include "stm32f10x.h"


// 物品检测数据分析;
extern const unsigned char Res_PosInit[5000];	//端口接收数据缓冲区 
extern int Res_PosTrack[60][3];				// 物品检测中,物品追踪点缓存.信息;
extern unsigned int Res_Trackcnt;				// 物品检测中,物品追踪点缓存.数量;
extern unsigned char Res_TrackACK[60];	// 物品检测中,物品追踪点缓存.状态;此点已完成检测操作,则为1;否则为0,可实行检测操作;
extern int Res_PosFollow[20][2];				// 物品检测中,当前检测到的可做检测操作的物品,坐标;
extern unsigned int Res_FollowSum;				// 物品检测中,当前检测到的可做检测操作的物品,总数量;
extern unsigned int Res_FollowNum;				// 物品检测中,当前检测到的可做检测操作的物品,当前已操作数量;

extern unsigned char Test_SpeedEn;			// 测视频速度;使能测速功能;
extern unsigned int Res_Speedcnt;			// 测视频速度;速度值缓存个数;
extern int Res_SpeedBuf[10];					// 测视频速度;速度值缓存值;

extern int Tmp_CheckSource[20];						// 缓存当前可监测坐标点;Y坐标;
extern int Tmp_PicSource[20];							// 缓存当前可指示坐标点;Y坐标;
extern int Res_FollowSource[20];							// 物品检测中,当前检测到的可做检测操作的物品,坐标;
////////// 跟随 /////////////////
extern int Run_FollowData[3];
extern unsigned char Run_FollowAddr;
extern unsigned char Run_FollowUpdate;

// 目标位置;
extern long	Plot_Xset[16],Plot_Xreal[16];// Y轴校准点;
extern long	Plot_Yset[16],Plot_Yreal[16];// Y轴校准点;
extern long	Plot_XsetA[4],Plot_XrealA[4];//
extern long	Plot_YsetA[4],Plot_YrealA[4];//
extern long	Plot_XrealReturn,Plot_YrealReturn;// Y轴校准点;
extern long int XReal_Code,YReal_Code;
extern long int XReal_Motor,YReal_Motor;
// 实时坐标位置;
extern long int XRealAll;
extern long int YRealAll;// 由当前角度计算出当前屏幕坐标位置(叠加上校准点,相对准确表达当前实际坐标值);


// 
extern void Data_init(void);
extern void Data_InitToAuto(void);
extern unsigned int Data_AsciiToDec(unsigned char DataTmp);
extern void Auto_DataProcess(uint8_t M_VideoDir,uint8_t M_VideoMode,uint8_t M_SpeedView);
// 
void  Position( long    x, long    y, long *  p, long *  q,
	              long *  XX, long *  YY);
extern void Deal_EnLaserData(long int nDataX, long int nDataY);
extern void Deal_TranLaserDataCode(void);// 计算出当前实际坐标(绝对位置编码器定位);
extern void Deal_TranLaserDataMotor(void);// 计算出当前实际坐标(步进电机步距定位);



#endif

