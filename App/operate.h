#ifndef __operate_H
#define __operate_H

#include "stm32f10x.h"

// operateA,
extern unsigned char 	ValIN_Setwei;							// 键盘数值输入;光标位选;
extern unsigned int		ValIN_Value[15];						// 键盘数值输入;设定数值缓存;
extern unsigned int 	ValIN_OneBUF[40];					// 键盘数值输入;设定数值单个值缓存;
extern unsigned char 	ValIN_count;							// 键盘数值输入;设定数值变量个数; -> 如2个变量待设定;
extern unsigned int 	ValIN_eachlen[15];					// 键盘数值输入;设定数值变量长度分布; -> 如{3,3}变量分别为百位、百位长度;
extern unsigned int 	ValIN_eachmax[15];					// 键盘数值输入;设定数值变量最大限值分布; -> 如{3,3}变量最大值分别为999、999;


// operateA,
extern void StartUp_Interface(void);
extern void Set0_Home(void);
extern void Set1_TimeSel(void);
extern void ManualControl_Onepoint(void);
extern void ManualControl_Yaogan(void);
extern void Keyset_ValueInit0(unsigned int type);
extern unsigned int Keyset_ValueInput(void);
extern void Show_Wave(void);// 示波器显示;////20241020新增;

// operateB,
extern void AutoPick_ModeSel(void);
extern void Keyset_AutoPick01(void);
extern void AutoPick_ModeRun(void);


#endif

