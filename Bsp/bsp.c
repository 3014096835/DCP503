#include  "stm32f10x.h"
#include  "stm32f10x_conf.h" 
#include  "bsp.h"
//#include 	"delay.h"
#include  "BoardVariable.h"


/*
*********************************************************************************************************
* Function    ：void BSP_Configuration(void)
* Description : 硬件初始化 
* Para        : none
* Return(s)   : none
* Example     : BSP_Configuration();
*********************************************************************************************************
*/
void BSP_Configuration(void)
{
		BSP_RCC_Configuration();
    BSP_GPIO_Configuration(); 
}



void BSP_GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*********BUZZER**************/
    GPIO_InitStructure.GPIO_Pin   = BSP_GPIOC_BUZZER ;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; // 推挽输出;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
    /*********激光笔**************/
    GPIO_InitStructure.GPIO_Pin = BSP_GPIOD_KIA|BSP_GPIOD_KIB;	 // KIB,PD0; KIA,PD1;   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     // 推挽输出;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
		
    /*********摇杆-按键**************/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	  // AN1,PC2;           
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
    /*********JA~JI **************/   

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;           
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     // 推挽输出;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	             
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     // 推挽输出;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;	             
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     // 推挽输出;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	             
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     // 推挽输出;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
			GPIO_SetBits(GPIOD, GPIO_Pin_15);
		

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;	             
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     // 推挽输出;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;	             
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     // 推挽输出;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
		
			MOTX01_SetPul_HI;	// OUT1;
			MOTX01_SetEn_LI;	MOTX01_SetDir_HI;	// EN1,DIR1;
			MOTY02_SetPul_HI;	// OUT2;
			MOTY02_SetEn_LI;	MOTY02_SetDir_HI;	// EN2,DIR2;
         
}



/*
*********************************************************************************************************
* Function    ：void BSP_RCC_Configuration(void)
* Description : 时钟配置函数,系统时钟配置为72MHZ，外设时钟配置 
* Para        : none
* Return(s)   : none
* Example     : BSP_RCC_Configuration();
*********************************************************************************************************
*/
void BSP_RCC_Configuration(void)
{
	SystemInit();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);// 引脚映射、IO中断进行读写前打开AFIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  // APB2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);		//开DMA1的时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);		//开DMA2的时钟
}

/*
*********************************************************************************************************
* Function    : void  BSP_LED_Toggle (Led_TypeDef LED)
* Description : TOGGLE any or all the LEDs on the board.
*
* Para        : LED     The ID of the LED to control:
*
*                       0    TOGGLE all LEDs on the board
*                       1    TOGGLE LED 1
*                       2    TOGGLE LED 2
* Return(s)   : none
* Example     : BSP_LED_Toggle(LED_ALL);// 翻转 all LED
              : BSP_LED_Toggle(LED1);   // 翻转 LED1
*********************************************************************************************************
*/
void  BSP_LED_Toggle (unsigned char LED)
{
		uint16_t  pins;

		pins = GPIO_ReadOutputData(GPIOC);

		switch (LED)
		{
		case 1:
				 if ((pins & BSP_GPIOC_LED1) == 0)      {		GPIO_SetBits(GPIOC, BSP_GPIOC_LED1);   } 
				 else             {		GPIO_ResetBits(GPIOC, BSP_GPIOC_LED1);  }
				break;
		case 2:
				 if ((pins & BSP_GPIOC_LED2) == 0)      {		GPIO_SetBits(GPIOC, BSP_GPIOC_LED2);   } 
				 else             {		GPIO_ResetBits(GPIOC, BSP_GPIOC_LED2);  }
				break;
		default:     break;
		}
}




