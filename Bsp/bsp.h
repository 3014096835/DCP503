#ifndef __BSP_H__
#define __BSP_H__

#include  "stm32f10x.h"
#include  "stm32f10x_conf.h" 

/* Private typedef -----------------------------------------------------------*/
// USART 
typedef enum 
{   FAILED = 0,
    PASSED = !FAILED
}   TestStatus;
// LED 
typedef enum 
{
  LED_ALL = 0,
  LED1 = 1,
  LED2 = 2
} Led_TypeDef;
/* Private define ------------------------------------------------------------*/
// BUZZER
#define       BSP_GPIOC_BUZZER          GPIO_Pin_13  // PC13(IO7) 
// LED
#define       BSP_GPIOC_LED1            GPIO_Pin_7   //  PC7(IO97)
#define       BSP_GPIOC_LED2            GPIO_Pin_9   //  PC9(IO99)
//Button
#define       BSP_GPIOC_BTN1            GPIO_Pin_6   //  PC6(IO96)
#define       BSP_GPIOC_BTN2            GPIO_Pin_8   //  PC8(IO98)



/* Function  -----------------------------------------------------------*/
void BSP_Configuration(void);
void BSP_GPIO_Configuration(void);
//void Alarm_Ctrl(void);
//void BSP_EXTI_Configuration(void);
void BSP_RCC_Configuration(void);
//void key_scan(void);
void BSP_LED_Toggle (unsigned char LED);



#endif 
                               
