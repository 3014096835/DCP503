#ifndef __operate_H
#define __operate_H

#include "stm32f10x.h"

// operateA,
extern unsigned char 	ValIN_Setwei;							// ������ֵ����;���λѡ;
extern unsigned int		ValIN_Value[15];						// ������ֵ����;�趨��ֵ����;
extern unsigned int 	ValIN_OneBUF[40];					// ������ֵ����;�趨��ֵ����ֵ����;
extern unsigned char 	ValIN_count;							// ������ֵ����;�趨��ֵ��������; -> ��2���������趨;
extern unsigned int 	ValIN_eachlen[15];					// ������ֵ����;�趨��ֵ�������ȷֲ�; -> ��{3,3}�����ֱ�Ϊ��λ����λ����;
extern unsigned int 	ValIN_eachmax[15];					// ������ֵ����;�趨��ֵ���������ֵ�ֲ�; -> ��{3,3}�������ֵ�ֱ�Ϊ999��999;


// operateA,
extern void StartUp_Interface(void);
extern void Set0_Home(void);
extern void Set1_TimeSel(void);
extern void ManualControl_Onepoint(void);
extern void ManualControl_Yaogan(void);
extern void Keyset_ValueInit0(unsigned int type);
extern unsigned int Keyset_ValueInput(void);
extern void Show_Wave(void);// ʾ������ʾ;////20241020����;

// operateB,
extern void AutoPick_ModeSel(void);
extern void Keyset_AutoPick01(void);
extern void AutoPick_ModeRun(void);


#endif

