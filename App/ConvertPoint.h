#ifndef __ConvertPoint_H
#define __ConvertPoint_H

#include "stm32f10x.h"


// ��Ʒ������ݷ���;
extern const unsigned char Res_PosInit[5000];	//�˿ڽ������ݻ����� 
extern int Res_PosTrack[60][3];				// ��Ʒ�����,��Ʒ׷�ٵ㻺��.��Ϣ;
extern unsigned int Res_Trackcnt;				// ��Ʒ�����,��Ʒ׷�ٵ㻺��.����;
extern unsigned char Res_TrackACK[60];	// ��Ʒ�����,��Ʒ׷�ٵ㻺��.״̬;�˵�����ɼ�����,��Ϊ1;����Ϊ0,��ʵ�м�����;
extern int Res_PosFollow[20][2];				// ��Ʒ�����,��ǰ��⵽�Ŀ�������������Ʒ,����;
extern unsigned int Res_FollowSum;				// ��Ʒ�����,��ǰ��⵽�Ŀ�������������Ʒ,������;
extern unsigned int Res_FollowNum;				// ��Ʒ�����,��ǰ��⵽�Ŀ�������������Ʒ,��ǰ�Ѳ�������;

extern unsigned char Test_SpeedEn;			// ����Ƶ�ٶ�;ʹ�ܲ��ٹ���;
extern unsigned int Res_Speedcnt;			// ����Ƶ�ٶ�;�ٶ�ֵ�������;
extern int Res_SpeedBuf[10];					// ����Ƶ�ٶ�;�ٶ�ֵ����ֵ;

extern int Tmp_CheckSource[20];						// ���浱ǰ�ɼ�������;Y����;
extern int Tmp_PicSource[20];							// ���浱ǰ��ָʾ�����;Y����;
extern int Res_FollowSource[20];							// ��Ʒ�����,��ǰ��⵽�Ŀ�������������Ʒ,����;
////////// ���� /////////////////
extern int Run_FollowData[3];
extern unsigned char Run_FollowAddr;
extern unsigned char Run_FollowUpdate;

// Ŀ��λ��;
extern long	Plot_Xset[16],Plot_Xreal[16];// Y��У׼��;
extern long	Plot_Yset[16],Plot_Yreal[16];// Y��У׼��;
extern long	Plot_XsetA[4],Plot_XrealA[4];//
extern long	Plot_YsetA[4],Plot_YrealA[4];//
extern long	Plot_XrealReturn,Plot_YrealReturn;// Y��У׼��;
extern long int XReal_Code,YReal_Code;
extern long int XReal_Motor,YReal_Motor;
// ʵʱ����λ��;
extern long int XRealAll;
extern long int YRealAll;// �ɵ�ǰ�Ƕȼ������ǰ��Ļ����λ��(������У׼��,���׼ȷ��ﵱǰʵ������ֵ);


// 
extern void Data_init(void);
extern void Data_InitToAuto(void);
extern unsigned int Data_AsciiToDec(unsigned char DataTmp);
extern void Auto_DataProcess(uint8_t M_VideoDir,uint8_t M_VideoMode,uint8_t M_SpeedView);
// 
void  Position( long    x, long    y, long *  p, long *  q,
	              long *  XX, long *  YY);
extern void Deal_EnLaserData(long int nDataX, long int nDataY);
extern void Deal_TranLaserDataCode(void);// �������ǰʵ������(����λ�ñ�������λ);
extern void Deal_TranLaserDataMotor(void);// �������ǰʵ������(����������ඨλ);



#endif

