#ifndef __TIMER_H
#define __TIMER_H

#include  "stm32f10x.h"
								
#define	StepMotor_StartRun		{TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);}	 				//使能TIM2中断开启;

extern unsigned char TC1_SNumX;
extern unsigned char TC1_SNumY;
extern unsigned int TC1_flagX,TC1_flagY;

extern void TIM2_Configuration(void);
extern void TIM2_IRQHandler(void);
extern void TIM3_Configuration(void);
extern void TIM3_IRQHandler(void);

extern void TIM7_Configuration(void);
extern void  TIM7_IRQHandler(void);

#endif
