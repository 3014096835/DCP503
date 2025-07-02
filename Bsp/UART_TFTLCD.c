/*
 * @file          UART_TFTLCD.c
 * @brief         程序: 串口通讯-204A(TFTLCD显示屏与键盘)-部分函数;
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
#include	"USART.h"
#include  "BoardVariable.h"
#include "delay.h"


int Rx2_cnt=0;	// 	接收数据缓存区,地址索引;
unsigned int Rx2_buf[15]={0};	// 	接收数据缓存区;

/*******************************************************************************
* Function Name  : void USART2_IRQHandler(void)
* Description    : USART2 Interrupt.// 从204A接收数据,获得矩阵键盘与独立键盘状态值; Matrixkey_val,Presskey_val;
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_IRQHandler(void)
{ 
		unsigned int Tmp_Rxd2;
	
		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
		{
			Tmp_Rxd2 = USART_ReceiveData(USART2);   //将读寄存器的数据缓存到接收缓冲区里
			
			if(Tmp_Rxd2==0XC1)	Rx2_cnt=1;// 帧头;
			switch(Rx2_cnt-1)
			{
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
					Rx2_buf[Rx2_cnt-1] = Tmp_Rxd2;		Rx2_cnt++;			break;
				case 9:
					Rx2_buf[Rx2_cnt-1] = Tmp_Rxd2;
					if((Rx2_buf[Rx2_cnt-2]==0xFD) && (Rx2_buf[Rx2_cnt-1]==0xFC))// 帧尾;
					{
						Matrixkey_val = Rx2_buf[2];	// 矩阵键盘值更新;
						Presskey_val = Rx2_buf[3];	// 独立按键值更新;
					}
					Rx2_cnt=0;
					break;
				default:	break;
			}
		}
		
		if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)                   //这段是为了避免STM32 USART 第一个字节发不出去的BUG 
		{ 
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);					     //禁止发缓冲器空中断， 
		}	
}



/**********************************************************************************************************
* Function    ：void LCD_DealData(void)
* Description : 界面数据更新处理 ;
* Para        : none
* Return(s)   : none
* Example     : LCD_DealData();
**********************************************************************************************************/
void LCD_DealData(void)
{
	if(LCD_ReTime == 1)
	{
		LCD_ReTime=0;
			LCD_Data[2] = 9;// 功能项;
			LCD_Data[3] = SysTime_Hour;
			LCD_Data[4] = SysTime_Minute;//
			LCD_Data[5] = SysTime_Second;//
			LCD_Data[6] = 0x00;
			LCD_Data[7] = 0x00;
			LCD_Data[8] = 0x00;
			LCD_Data[9] = 0x00;
			LCD_Data[10] = 0x00;
			LCD_Data[11] = 0x00;
			LCD_Data[12] = 0x00;
			LCD_Data[13] = 0x00;
			LCD_Data[14] = 0x00;
			LCD_Data[15] = 0x00;
			LCD_Data[16] = 0x00;
			LCD_Data[17] = 0x00;
			LCD_Data[18] = 0x00;
			LCD_Data[19] = 0x00;
	}
	else
		switch(LCD_flag)
		{
			case 0:	
			case 1:	
				LCD_Data[2] = 1;// 功能项;
				LCD_Data[3] = 5;// 工位号;5;
				LCD_Data[4] = 1;// 闪烁时间间隔;(s);1S;
				LCD_Data[5] = 0x00;
				LCD_Data[6] = 0x00;
				LCD_Data[7] = 0x00;
				LCD_Data[8] = 0x00;
				LCD_Data[9] = 0x00;
				LCD_Data[10] = 0x00;
				LCD_Data[11] = 0x00;
				LCD_Data[12] = 0x00;
				LCD_Data[13] = 0x00;
				LCD_Data[14] = 0x00;
				LCD_Data[15] = 0x00;
				LCD_Data[16] = 0x00;
				LCD_Data[17] = 0x00;
				LCD_Data[18] = 0x00;
				LCD_Data[19] = 0x00;
				break;
			case 2:	
			case 9:	
				LCD_Data[2] = 2;// 功能项;
				LCD_Data[3] = 0x00;
				LCD_Data[4] = 0x00;
				LCD_Data[5] = 0x00;
				LCD_Data[6] = 0x00;
				LCD_Data[7] = 0x00;
				LCD_Data[8] = 0x00;
				LCD_Data[9] = 0x00;
				LCD_Data[10] = 0x00;
				LCD_Data[11] = 0x00;
				LCD_Data[12] = 0x00;
				LCD_Data[13] = 0x00;
				LCD_Data[14] = 0x00;
				LCD_Data[15] = 0x00;
				LCD_Data[16] = 0x00;
				LCD_Data[17] = 0x00;
				LCD_Data[18] = 0x00;
				LCD_Data[19] = 0x00;
				break;
			case 3:
				LCD_Data[2] = 10;// 功能项;
				LCD_Data[3] = ValIN_Value[0];
				LCD_Data[4] = ValIN_Value[1];//
				LCD_Data[5] = ValIN_Value[2];//
				LCD_Data[6] = ValIN_Setwei;
				LCD_Data[7] = 0x00;
				LCD_Data[8] = 0x00;
				LCD_Data[9] = 0x00;
				LCD_Data[10] = 0x00;
				LCD_Data[11] = 0x00;
				LCD_Data[12] = 0x00;
				LCD_Data[13] = 0x00;
				LCD_Data[14] = 0x00;
				LCD_Data[15] = 0x00;
				LCD_Data[16] = 0x00;
				LCD_Data[17] = 0x00;
				LCD_Data[18] = 0x00;
				LCD_Data[19] = 0x00;
				break;
			case 4:
				LCD_Data[2] = 3;// 功能项;
				LCD_Data[3] = Laser_State;// 激光笔状态;0,close;1,open;
					LCD_Data[4] = ValIN_Value[0]/256;
					LCD_Data[5] = ValIN_Value[0]%256;
					LCD_Data[6] = ValIN_Value[1]/256;
					LCD_Data[7] = ValIN_Value[1]%256;
				LCD_Data[8] = ValIN_Setwei;
				LCD_Data[9] = Loop_RunHoldtime;
				LCD_Data[10] = 0x00;
				LCD_Data[11] = 0x00;
				LCD_Data[12] = 0x00;
				LCD_Data[13] = 0x00;
				LCD_Data[14] = 0x00;
				LCD_Data[15] = 0x00;
				LCD_Data[16] = 0x00;
				LCD_Data[17] = 0x00;
				LCD_Data[18] = 0x00;
				LCD_Data[19] = 0x00;
				break;
			case 5:	
				LCD_Data[2] = 4;// 功能项;
				LCD_Data[3] = Video_Mode+1;// 视频难度状态;0;1;2;
				LCD_Data[4] = 0x00;
				LCD_Data[5] = 0x00;
				LCD_Data[6] = 0x00;
				LCD_Data[7] = 0x00;
				LCD_Data[8] = 0x00;
				LCD_Data[9] = 0x00;
				LCD_Data[10] = 0x00;
				LCD_Data[11] = 0x00;
				LCD_Data[12] = 0x00;
				LCD_Data[13] = 0x00;
				LCD_Data[14] = 0x00;
				LCD_Data[15] = 0x00;
				LCD_Data[16] = 0x00;
				LCD_Data[17] = 0x00;
				LCD_Data[18] = 0x00;
				LCD_Data[19] = 0x00;
				break;
			case 6:
			if(Page_Up==1)// "Up";向上翻页;
			{
				Page_Up=0;
				LCD_Data[2] = 7;// 功能项;
				LCD_Data[3] = 0x00;
				LCD_Data[4] = 0x00;
				LCD_Data[5] = 0x00;
				LCD_Data[6] = 0x00;
				LCD_Data[7] = 0x00;
				LCD_Data[8] = 0x00;
				LCD_Data[9] = 0x00;
				LCD_Data[10] = 0x00;
				LCD_Data[11] = 0x00;
				LCD_Data[12] = 0x00;
				LCD_Data[13] = 0x00;
				LCD_Data[14] = 0x00;
				LCD_Data[15] = 0x00;
				LCD_Data[16] = 0x00;
				LCD_Data[17] = 0x00;
				LCD_Data[18] = 0x00;
				LCD_Data[19] = 0x00;
			}
			else if(Page_Down==1)// "Down";向下翻页;
			{
				Page_Down=0;
				LCD_Data[2] = 8;// 功能项;
				LCD_Data[3] = 0x00;
				LCD_Data[4] = 0x00;
				LCD_Data[5] = 0x00;
				LCD_Data[6] = 0x00;
				LCD_Data[7] = 0x00;
				LCD_Data[8] = 0x00;
				LCD_Data[9] = 0x00;
				LCD_Data[10] = 0x00;
				LCD_Data[11] = 0x00;
				LCD_Data[12] = 0x00;
				LCD_Data[13] = 0x00;
				LCD_Data[14] = 0x00;
				LCD_Data[15] = 0x00;
				LCD_Data[16] = 0x00;
				LCD_Data[17] = 0x00;
				LCD_Data[18] = 0x00;
				LCD_Data[19] = 0x00;
			}
			else if(LCD_AutoEnter==1)
			{
				LCD_AutoEnter=0;
				LCD_Data[2] = 5;// 功能项;
				LCD_Data[3] = 0x00;
				LCD_Data[4] = 0x00;
				LCD_Data[5] = 0x00;
				LCD_Data[6] = 0x00;
				LCD_Data[7] = 0x00;
				LCD_Data[8] = 0x00;
				LCD_Data[9] = 0x00;
				LCD_Data[10] = 0x00;
				LCD_Data[11] = 0x00;
				LCD_Data[12] = 0x00;
				LCD_Data[13] = 0x00;
				LCD_Data[14] = 0x00;
				LCD_Data[15] = 0x00;
				LCD_Data[16] = 0x00;
				LCD_Data[17] = 0x00;
				LCD_Data[18] = 0x00;
				LCD_Data[19] = 0x00;
			}
			else if(LCD_PicSuccess==1)
			{
				LCD_PicSuccess=0;
				LCD_Data[2] = 5;// 功能项;
				LCD_Data[3] = Material_Xinxi;//
				LCD_Data[4] = Material_TimeOut/256;//
				LCD_Data[5] = Material_TimeOut%256;//
				LCD_Data[6] = Material_Shape;//
				LCD_Data[7] = Material_Color;//
				LCD_Data[8] = Material_Number;//
				LCD_Data[9] = Video_Mode+1;//
				LCD_Data[10] = 1;//Arg_ReadAdj.Type
				LCD_Data[11] = Laser_ScreenX/256;//
				LCD_Data[12] = Laser_ScreenX%256;//
				LCD_Data[13] = Laser_ScreenY/256;//
				LCD_Data[14] = Laser_ScreenY%256;//
				LCD_Data[15] = 0x00;
				LCD_Data[16] = 0x00;
				LCD_Data[17] = 0x00;
				LCD_Data[18] = 0x00;
				LCD_Data[19] = 0x00;
			}
			else
			{
				LCD_Data[2] = 11;// 功能项;
				LCD_Data[3] = Auto_Running;//
					LCD_Data[4] = Video_Mode+1;
				LCD_Data[5] = 0;
				LCD_Data[6] = 0;
				LCD_Data[7] = 0;
				LCD_Data[8] = 0;
				LCD_Data[9] = 0;
				LCD_Data[10] = 0;
				LCD_Data[11] = 0;
				LCD_Data[12] = 0;
				LCD_Data[13] = 0;
				LCD_Data[14] = 0;
				LCD_Data[15] = 0x00;
				LCD_Data[16] = 0x00;
				LCD_Data[17] = 0;
				LCD_Data[18] = 0;
				LCD_Data[19] = 0;
			}
				break;
			case 7:
				LCD_Data[2] = 6;// 功能项;
				LCD_Data[3] = Arg_ReadAdj.Shape;//
				LCD_Data[4] = Arg_ReadAdj.Color;//
				LCD_Data[5] = Select_Background;
				LCD_Data[6] = 0x00;
				LCD_Data[7] = 0x00;
				LCD_Data[8] = 0x00;
				LCD_Data[9] = 0x00;
				LCD_Data[10] = 0x00;
				LCD_Data[11] = 0x00;
				LCD_Data[12] = 0x00;
				LCD_Data[13] = 0x00;
				LCD_Data[14] = 0x00;
				LCD_Data[15] = 0x00;
				LCD_Data[16] = 0x00;
				LCD_Data[17] = 0;
				LCD_Data[18] = 0;
				LCD_Data[19] = 0;
				break;
			case 8:
				LCD_Data[2] = 12;// 功能项;
				LCD_Data[3] = ValIN_Value[0]/256;//
				LCD_Data[4] = ValIN_Value[0]%256;//
				LCD_Data[5] = ValIN_Value[1]/256;
				LCD_Data[6] = ValIN_Value[1]%256;
				LCD_Data[7] = ValIN_Value[2]/256;
				LCD_Data[8] = ValIN_Value[2]%256;
				LCD_Data[9] = ValIN_Value[3]/256;
				LCD_Data[10] = ValIN_Value[3]%256;
				LCD_Data[11] = ValIN_Value[4]/256;
				LCD_Data[12] = ValIN_Value[4]%256;
				LCD_Data[13] = ValIN_Value[5]/256;
				LCD_Data[14] = ValIN_Value[5]%256;
				LCD_Data[15] = ValIN_Value[6];
				LCD_Data[16] = ValIN_Value[7];
				LCD_Data[17] = ValIN_Value[8];
				LCD_Data[18] = ValIN_Value[9];
				LCD_Data[19] = ValIN_Setwei;
				break;
			case 10:
				LCD_Data[2] = 14;// 功能项;
				LCD_Data[3] = 0;
					LCD_Data[4] = Obj_DataX/256;//
					LCD_Data[5] = Obj_DataX%256;//
					LCD_Data[6] = Obj_DataY/256;//
					LCD_Data[7] = Obj_DataY%256;//
				LCD_Data[8] = ValIN_Setwei;
				LCD_Data[9] = CodeX.RealRead/256;
				LCD_Data[10] = CodeX.RealRead%256;
				LCD_Data[11] = CodeY.RealRead/256;
				LCD_Data[12] = CodeY.RealRead%256;
				LCD_Data[13] = 0x00;
				LCD_Data[14] = 0x00;
				LCD_Data[15] = 0x00;
				LCD_Data[16] = 0x00;
				LCD_Data[17] = 0x00;
				LCD_Data[18] = 0x00;
				LCD_Data[19] = 0x00;
				break;
			case 11:
				LCD_Data[2] = 3;// 功能项;
				LCD_Data[3] = Laser_State;// 激光笔状态;0,close;1,open;
					LCD_Data[4] = XRealAll/256;
					LCD_Data[5] = XRealAll%256;
					LCD_Data[6] = YRealAll/256;
					LCD_Data[7] = YRealAll%256;
				LCD_Data[8] = 6;
				LCD_Data[9] = Loop_RunHoldtime;
				LCD_Data[10] = 0x00;
				LCD_Data[11] = 0x00;
				LCD_Data[12] = 0x00;
				LCD_Data[13] = 0x00;
				LCD_Data[14] = 0x00;
				LCD_Data[15] = 0x00;
				LCD_Data[16] = 0x00;
				LCD_Data[17] = 0x00;
				LCD_Data[18] = 0x00;
				LCD_Data[19] = 0x00;
				break;
			
			case 20:// 示波器显示;////20241020新增;
				LCD_Data[2] = 0x00;// 功能项;
				LCD_Data[3] = 0x00;// 
				LCD_Data[4] = 0x00;
				LCD_Data[5] = 0x00;
				LCD_Data[6] = 0x00;
				LCD_Data[7] = 0x00;
				LCD_Data[8] = 0x00;
				LCD_Data[9] = 0x00;
				LCD_Data[10] = 0x00;
				LCD_Data[11] = 0x00;
				LCD_Data[12] = 0x00;
				LCD_Data[13] = 0x00;
				LCD_Data[14] = 0x00;
				LCD_Data[15] = 0x00;
				LCD_Data[16] = 0x00;
				LCD_Data[17] = 0x00;
				LCD_Data[18] = 0x00;
				LCD_Data[19] = 0x00;
				break;
			default:break;
		}
		        LCD_Data[0] = 0xA1;// 帧头;
				if(LCD_flag==20)// 示波器显示;////20241020新增;
				{
					LCD_Data[0] = 0xB1;// 帧头;
				}
				LCD_Data[1] = 0x11;// 数据字节数;
				LCD_Data[20] = 0xFD;// 尾校验;
				LCD_Data[21] = 0xFC;// 尾校验;
	
				USART_Send_Data02(1,&LCD_Data[0],22);
				delay_ms(50);
	
}



