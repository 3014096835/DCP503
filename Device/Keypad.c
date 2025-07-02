/*
 * @file          Communicate.c
 * @brief         程序: 键盘:矩阵键盘-独立键盘-部分函数;
 * @ucos-ii       V2.86 
 * @function      QSS-ICBS1 
 * @compiler      RealView MDK-ARM Compiler  V5.14
 * @peripheral    V3.5.0
 * @author  	  	
 * @date   	      2024/05/01
 * @corporation   浙江求是科教设备有限公司
 * @website       http://www.zjqiushi.com/
*/
/* Includes ------------------------------------------------------------------*/
#include  "stm32f10x.h"
#include  "stm32f10x_conf.h" 
#include  "Keypad.h"
#include 	"delay.h"


// 独立按键部分;
#define BSP_GPIOG_SW1         GPIO_Pin_12                       //sw1=PG12 
#define BSP_GPIOG_SW2         GPIO_Pin_13                       //sw1=PG13
#define BSP_GPIOG_SW3         GPIO_Pin_14                       //sw1=PG14
#define BSP_GPIOG_SW4         GPIO_Pin_15                       //sw1=PG15
//
unsigned char press_flag1=0;
unsigned char press_flag2=0;
unsigned char press_flag3=0;
unsigned char press_flag4=0;
// 矩阵按键部分;
unsigned char	key_Pressed;
unsigned char	key_val=0;
unsigned char	key_flag;
//
unsigned char	key_Map[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}; //16个按键的键值数组


void Keypad_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
	
		/////////////////////// Keypad To (JE) ///////////////////////////
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;	             
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     // 推挽输出;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOG, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	             
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入;
		GPIO_Init(GPIOG, &GPIO_InitStructure);
		/////////////////////// To (JE) ///////////////////////////

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	    // SW1~SW4;         
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入;
		GPIO_Init(GPIOG, &GPIO_InitStructure);
}

//////////////////////// Keypad To (JE) ////////////////////////////////////
void Check_Key(void)
{
		unsigned char row, col; 
		unsigned int KEY_DOUT,tmp1, tmp2;
		tmp1 = 0x0008;
		for(row=0; row<4; row++)		//行扫描
		{	
			KEY_DOUT = 0X000f;		//输出全为1
			KEY_DOUT-= tmp1;		//依次输出一个为0
			GPIOG->ODR=((GPIOG->ODR&0xfff0)|KEY_DOUT);
			tmp1 >>=1;
			if((GPIO_ReadInputData(GPIOG)&0x00f0)<0x00f0)
			//if((KEY_DIN & 0xF0) < 0xF0) //P2输入是否有一位为0
			{
				tmp2 = 0x0010;			//用于检测出哪一位为0
				for(col=0; col<4; col++)	//列扫描
				{
					if(0x00 == (GPIO_ReadInputData(GPIOG) & tmp2))	//找到等于0的列
					{
						key_val = key_Map[row*4 + col];//获取键值
						return;		//退出循环
					}
					tmp2 <<= 1;		//右移1位
				}
			}

		}
}

void Key_Event(void)
{
		unsigned int tmp;
		GPIOG->ODR=((GPIOG->ODR&0xfff0)|0x0000);
		tmp = GPIO_ReadInputData(GPIOG);			
		if ((0x00 == key_Pressed) && ((tmp & 0x00F0) < 0x00F0))		//如果有键按下
		{
			key_Pressed = 1;		//按键按下标识位置位
			delay_ms(10);				//延时去抖
			Check_Key();			//获取键
			//		key_flag    = 1;		//按键标识置位
		}
		else if ((key_Pressed == 1)&&((tmp & 0x00f0) == 0x00F0))	//如果按键释放
		{
			key_Pressed = 0;		//清除标识位
			key_flag    = 1;		//按键标识位置位
		}
		else
		{
			delay_ms(1);
		}
}
//////////////////////// To (JE) ////////////////////////////////////



void key_scan(void)
{
//	if(GPIO_ReadInputDataBit(GPIOC, BSP_GPIOC_BTN1 )==0)
//	{
//	} 
//	if(GPIO_ReadInputDataBit(GPIOC, BSP_GPIOC_BTN2 )==0)
//	{
//	} 
		if(GPIO_ReadInputDataBit(GPIOG, BSP_GPIOG_SW1 )==0)
		{
			while(GPIO_ReadInputDataBit(GPIOG, BSP_GPIOG_SW1 )==0);
			press_flag1=1;
		}
		else	{press_flag1=0;}
		if(GPIO_ReadInputDataBit(GPIOG, BSP_GPIOG_SW2 )==0)
		{
			while(GPIO_ReadInputDataBit(GPIOG, BSP_GPIOG_SW2 )==0);
			press_flag2=1;
		}
		else	{press_flag2=0;}
		if(GPIO_ReadInputDataBit(GPIOG, BSP_GPIOG_SW3 )==0)
		{
			while(GPIO_ReadInputDataBit(GPIOG, BSP_GPIOG_SW3 )==0);
			press_flag3=1;
		}
		else	{press_flag3=0;}
		if(GPIO_ReadInputDataBit(GPIOG, BSP_GPIOG_SW4 )==0)
		{
			while(GPIO_ReadInputDataBit(GPIOG, BSP_GPIOG_SW4 )==0);
			press_flag4=1;
		}
		else	{press_flag4=0;}
}

