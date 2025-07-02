/*
 * @file          adjust.c
 * @brief         参数设置程序
 * @ucos-ii       V2.86 
 * @function      
 * @compiler      RealView MDK-ARM Compiler V5.14
 * @peripheral    V3.5.0
 * @author  	  	
 * @date   	      2018/04/27
 * @corporation   浙江求是科教设备有限公司
 * @website       http://www.zjqiushi.com/
*/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include  "stm32f10x_conf.h" 
#include  "bsp.h"
#include  "adjust.h"
#include  "BoardVariable.h"
/* Private variables ---------------------------------------------------------*/

long int Mess[32]={40,240,440,640, 640,440,240,40,  40,240,440,640,   640,440,240,40,
										0,0,0,0,       120,120,120,120, 240,240,240,240,  360,360,360,360};

/**********************************************************************************************************
* Function    ：void Flash_Init(unsigned char n_read)
* Description : Flash-EEPROM初始化;1为可读取状态;
* Para        : none
* Return(s)   : none
* Example     : Flash_Init();
**********************************************************************************************************/
void Flash_Init(unsigned char n_read)
{
	unsigned char i;
	if(((*(uint16_t*)(0x807f800)) == 0x01) && (n_read==1))// 查询校准点是否已存入EEPROM,未存入则取固定点;
	{
		Arg_ReadAdj.PointaX = (*(uint16_t*)(0x807f802))*256 + (*(uint16_t*)(0x807f804));
		Arg_ReadAdj.PointaY = (*(uint16_t*)(0x807f806))*256 + (*(uint16_t*)(0x807f808));
		Arg_ReadAdj.LASER_Axis = (*(uint16_t*)(0x807f80a))*256 + (*(uint16_t*)(0x807f80c));
		Arg_ReadAdj.MatchY = (*(uint16_t*)(0x807f80e))*256 + (*(uint16_t*)(0x807f810));
		Arg_ReadAdj.CBMzeroX = (*(uint16_t*)(0x807f812))*256 + (*(uint16_t*)(0x807f814));
		Arg_ReadAdj.CBMzeroY = (*(uint16_t*)(0x807f818))*256 + (*(uint16_t*)(0x807f81a));
		Arg_ReadAdj.Shape = *(uint16_t*)(0x807f81c);
		Arg_ReadAdj.Color = *(uint16_t*)(0x807f81e);
		//Arg_ReadAdj.Type = *(uint16_t*)(0x807f820);
		Arg_ReadAdj.Volume = *(uint16_t*)(0x807f822);	
		Arg_ReadAdj.LowSpeed = *(uint16_t*)(0x807f824);
		Arg_ReadAdj.HighSpeed = *(uint16_t*)(0x807f828);
		Arg_ReadAdj.RunDir = *(uint16_t*)(0x807f82a);
		
		if((*(uint16_t*)(0x807f82e)) == 0xffff)
		{
			for(i=0;i<32;i++)
			{
				Arg_ReadAdj.PlotReal[i] = Mess[i]+100;
			}
			Flash_eepromIn();//EEPROM 模拟 FLASH 存入;
		}
		else
		{
			for(i=0;i<32;i++)
			{
				Arg_ReadAdj.PlotReal[i] = *(uint16_t*)(0x807f82e+i*4);
			}
		}
		if((*(uint16_t*)(0x807f82e+32*4)) == 0xffff)
		{
			for(i=0;i<32;i++)
			{
				Arg_ReadAdj.MotorReal[i] = Mess[i]+300;
			}
			Flash_eepromIn();//EEPROM 模拟 FLASH 存入;
		}
		else
		{
			for(i=0;i<32;i++)
			{
				Arg_ReadAdj.MotorReal[i] = *(uint16_t*)(0x807f82e+32*4+i*2);
			}
		}
		
	}
	else
	{
		Arg_ReadAdj.PointaX = 352;		Arg_ReadAdj.PointaY = 0;		// 校准参数设定;校准点A(80,0)像素坐标;
		Arg_ReadAdj.LASER_Axis = 530;	Arg_ReadAdj.MatchY = 360;		// 校准参数设定;校准点B(640,360)像素坐标;
		Arg_ReadAdj.CBMzeroX = 2048;		Arg_ReadAdj.CBMzeroY = 2048;		// 校准参数设定;舵机云台零位校准;
		Arg_ReadAdj.Shape = 1;					// 校准参数设定;检测目标物体_形状;1~5;
		Arg_ReadAdj.Color = 2;					// 校准参数设定;检测目标物体_颜色;1~4;
		//Arg_ReadAdj.Type = 1;					// 校准参数设定;检测目标物体_类型;;1~4;
		Arg_ReadAdj.Volume = 1;				// 校准参数设定;语音音量设定;1~16;
		Arg_ReadAdj.LowSpeed = 8;			// 校准参数设定;运动速度;低速3~10cm/s;
		Arg_ReadAdj.HighSpeed = 20;		// 校准参数设定;运动速度;高速10~30cm/s;
		Arg_ReadAdj.RunDir = Dir_Rightup;	// 校准参数设定;视频运动方向;	//Dir_Leftdown=0;Dir_Rightup=1;
		
		for(i=0;i<32;i++)
		{
			Arg_ReadAdj.PlotReal[i] = Mess[i]+100;
			Arg_ReadAdj.MotorReal[i] = Mess[i]+300;
		}
		
		
		Flash_eepromIn();//EEPROM 模拟 FLASH 存入;
		
						if((Select_ItemMode==1) || (Select_ItemMode==2) || (Select_ItemMode==3))
						{
							Select_Shape[Select_ItemMode-1] = Arg_ReadAdj.Shape;
							Select_Color[Select_ItemMode-1] = Arg_ReadAdj.Color;
						}
						else
						{
							Select_Shape[0] = Arg_ReadAdj.Shape;
							Select_Shape[1] = Arg_ReadAdj.Shape;
							Select_Shape[2] = Arg_ReadAdj.Shape;
							Select_Color[0] = Arg_ReadAdj.Color;
							Select_Color[1] = Arg_ReadAdj.Color;
							Select_Color[2] = Arg_ReadAdj.Color;
						}
	}
}

/**********************************************************************************************************
* Function    ：void Flash_eepromIn(void)
* Description : EEPROM 模拟 FLASH 存入 
* Para        : none
* Return(s)   : none
* Example     : Flash_eepromIn();
**********************************************************************************************************/
void Flash_eepromIn(void)
{
	unsigned char i;
	FLASH_Unlock();	//FLASH解锁
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);// Clear All pending flags//清标志位
	FLASH_ErasePage(0x0807f800);// 擦除页   512k(2Kbye):245页:0x0807 f000--0x0807 f7ff;255页:0x0807 f800--0x0807 ffff;
	FLASH_ProgramHalfWord(0x807f800, 0x01);
	FLASH_ProgramHalfWord(0x807f802, (unsigned int)Arg_ReadAdj.PointaX/256);				FLASH_ProgramHalfWord(0x807f804, (unsigned int)Arg_ReadAdj.PointaX%256);
	FLASH_ProgramHalfWord(0x807f806, (unsigned int)Arg_ReadAdj.PointaY/256);				FLASH_ProgramHalfWord(0x807f808, (unsigned int)Arg_ReadAdj.PointaY%256);
	FLASH_ProgramHalfWord(0x807f80a, (unsigned int)Arg_ReadAdj.LASER_Axis/256);			FLASH_ProgramHalfWord(0x807f80c, (unsigned int)Arg_ReadAdj.LASER_Axis%256);
	FLASH_ProgramHalfWord(0x807f80e, (unsigned int)Arg_ReadAdj.MatchY/256);					FLASH_ProgramHalfWord(0x807f810, (unsigned int)Arg_ReadAdj.MatchY%256);
	FLASH_ProgramHalfWord(0x807f812, (unsigned int)Arg_ReadAdj.CBMzeroX/256);				FLASH_ProgramHalfWord(0x807f814, (unsigned int)Arg_ReadAdj.CBMzeroX%256);
	FLASH_ProgramHalfWord(0x807f818, (unsigned int)Arg_ReadAdj.CBMzeroY/256);				FLASH_ProgramHalfWord(0x807f81a, (unsigned int)Arg_ReadAdj.CBMzeroY%256);
	FLASH_ProgramHalfWord(0x807f81c, Arg_ReadAdj.Shape);
	FLASH_ProgramHalfWord(0x807f81e, Arg_ReadAdj.Color);
	FLASH_ProgramHalfWord(0x807f820, 1);//Arg_ReadAdj.Type
	FLASH_ProgramHalfWord(0x807f822, Arg_ReadAdj.Volume);
	FLASH_ProgramHalfWord(0x807f824, Arg_ReadAdj.LowSpeed);
	FLASH_ProgramHalfWord(0x807f828, Arg_ReadAdj.HighSpeed);
	FLASH_ProgramHalfWord(0x807f82a, Arg_ReadAdj.RunDir);
	for(i=0;i<32;i++)
	{
		FLASH_ProgramHalfWord(0x807f82e+i*4, Arg_ReadAdj.PlotReal[i]);
	}
	for(i=0;i<32;i++)
	{
		FLASH_ProgramHalfWord(0x807f82e+32*4+i*2, Arg_ReadAdj.MotorReal[i]);
	}
	
	
	FLASH_Lock();//FLASH锁定 
}








