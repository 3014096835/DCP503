#ifndef __adc_H
#define __adc_H

#include "stm32f10x.h"

void ADC_Configuration(void);
extern void TIM8_UP_IRQHandler(void);
extern void DMA1_Channel1_IRQHandler(void);

#endif

