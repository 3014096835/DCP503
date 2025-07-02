/*
 * @file          UART_485.c
 * @brief         程序: 串口通讯-触摸屏RS485-部分函数;
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


/**********************************************************************************************************
* Function    ：void Massage_Post(void)
* Description : 信息更新;液晶屏, 触摸屏;
* Para        : none
* Return(s)   : none
* Example     : Massage_Post();
**********************************************************************************************************/
void Massage_Post(void)
{
		LCD_PicSuccess=1;
		LCD_DealData();
		/////////////////////////////////////////////////////////////////
		if(Video_Mode==0)	{		MaterialF1_Num++;	}
		else if(Video_Mode==1)	{		MaterialF2_Num++;	}
		else if(Video_Mode==2)	{		MaterialF3_Num++;	}
		
		MCGS_Sendform=10;// 触摸屏;更新当前识别到物品信息;
		////////////////////////////////////////////////////////////////
}


/**********************************************************************************************************
* Function    ：void Massage_PostEnd(void)
* Description : 结果发送;触摸屏;
* Para        : none
* Return(s)   : none
* Example     : Massage_PostEnd();
**********************************************************************************************************/
void Massage_PostEnd(void)
{
		MCGS_Sendform=20;// 触摸屏;更新结束汇总信息;
}


/**********************************************************************************************************
* Function    ：void MCGS_Send(uint8_t M_Cnt,uint8_t M_State,uint8_t M_Mode,long M_Time,uint8_t M_Num,uint8_t M_Message)
* Description : 触摸屏运维系统报文更新;//2023年国赛配套触摸屏程序使用;
* 							M_Cnt,此信息是否更新;
* 							M_State,信息,工作状态; M_Mode,信息,工作模式; M_Time,信息,工作时间;M_Num,信息,物品数量;
* Para        : none
* Return(s)   : none
* Example     : MCGS_Send();
**********************************************************************************************************/
void MCGS_Send(uint8_t M_Cnt,uint8_t M_State,uint8_t M_Mode,long M_Time,uint8_t M_Num,uint8_t M_Message
								,uint8_t M_Shape,uint8_t M_Color)// MCGS触摸屏发送函数新修改;//20241020修改;
{
	if(M_Cnt==1)
	{
		MCGS_TxBuffer[0] = 0x01;  // 本机地址
		MCGS_TxBuffer[1] = 0x04;  // 功能码
		MCGS_TxBuffer[2] = 26;
		
		MCGS_TxBuffer[3] = 0x00;		MCGS_TxBuffer[4] = M_State;							// 工作状态; 1,工作; 2,汇总;
		MCGS_TxBuffer[5] = 0x00;		MCGS_TxBuffer[6] = M_Mode+1;						// 工作模式;
		MCGS_TxBuffer[7] = M_Time>>8;		// 工作时间;(s)
		MCGS_TxBuffer[8] = M_Time&0xff;
			MCGS_TxBuffer[9] = 0x00;		MCGS_TxBuffer[10] = M_Shape;	// 物体形状;//+5*(Arg_ReadAdj.Type-1)	
			MCGS_TxBuffer[11] = 0x00;		MCGS_TxBuffer[12] = M_Color;	// 物体颜色;
		MCGS_TxBuffer[13] = 0x00;		MCGS_TxBuffer[14] = M_Num;							// 物体数量;
		MCGS_TxBuffer[15] = 0x00;		MCGS_TxBuffer[16] = M_Message;					// 序号;
		if(Auto_Running==0)	//停止检测时;
		{
			MCGS_TxBuffer[17] = 0;	MCGS_TxBuffer[18] = 0;	// X轴坐标;
			MCGS_TxBuffer[19] = 0;	MCGS_TxBuffer[20] = 0;	// Y轴坐标;
		}
		else
		{
			MCGS_TxBuffer[17] = Laser_ScreenX/256;	MCGS_TxBuffer[18] = Laser_ScreenX%256;	// X轴坐标;
			MCGS_TxBuffer[19] = Laser_ScreenY/256;	MCGS_TxBuffer[20] = Laser_ScreenY%256;	// Y轴坐标;
		}
		MCGS_TxBuffer[21] = 0x00;		MCGS_TxBuffer[22] = MaterialF1_Num;			// 模式1数量(F1);
		MCGS_TxBuffer[23] = 0x00;		MCGS_TxBuffer[24] = MaterialF2_Num;			// 模式2数量(F2);
		MCGS_TxBuffer[25] = 0x00;		MCGS_TxBuffer[26] = MaterialF3_Num;			// 模式3数量(F3);
		MCGS_TxBuffer[27] = 0x00;		MCGS_TxBuffer[28] = Select_Background;	// 背景色;1~2;// 1,白色;2,黑色;
			
			Send_CRC = usMBCRC16( MCGS_TxBuffer, 29 );   
		MCGS_TxBuffer[29] = Send_CRC &0xff;
		MCGS_TxBuffer[30] = Send_CRC >> 8;		// 将校验位加入发送缓冲

    USART_Send_Data02(0, MCGS_TxBuffer,31);
	}
	else
	{
		MCGS_TxBuffer[0] = 0x01;  // 本机地址
		MCGS_TxBuffer[1] = 0x04;  // 功能码
		MCGS_TxBuffer[2] = 26;
		
		MCGS_TxBuffer[3] = 0x00;		MCGS_TxBuffer[4] = M_State;						// 工作状态; 1,工作; 2,汇总;
		MCGS_TxBuffer[5] = 0x00;		MCGS_TxBuffer[6] = M_Mode+1;					// 工作模式;
		MCGS_TxBuffer[15] = 0x00;		MCGS_TxBuffer[16] = 0;								// 序号;0
		
		MCGS_TxBuffer[17] = 0;			MCGS_TxBuffer[18] = 0;	// X轴坐标;
		MCGS_TxBuffer[19] = 0;			MCGS_TxBuffer[20] = 0;	// Y轴坐标;
		MCGS_TxBuffer[21] = 0x00;		MCGS_TxBuffer[22] = MaterialF1_Num;		// 模式1数量(F1);
		MCGS_TxBuffer[23] = 0x00;		MCGS_TxBuffer[24] = MaterialF2_Num;		// 模式2数量(F2);
		MCGS_TxBuffer[25] = 0x00;		MCGS_TxBuffer[26] = MaterialF3_Num;		// 模式3数量(F3);
		
			Send_CRC = usMBCRC16( MCGS_TxBuffer, 29 );   
		MCGS_TxBuffer[29] = Send_CRC &0xff;
		MCGS_TxBuffer[30] = Send_CRC >> 8;		// 将校验位加入发送缓冲

    USART_Send_Data02(0, MCGS_TxBuffer,31);
	}
}


// MCGS触摸屏发送函数新修改;//20241020修改;
unsigned int Cnt_i=0;
unsigned int Cnt_j=0;
unsigned int Cnt_k=0;
unsigned int Cnt_n=0;

/*
*********************************************************************************************************
* Function    ：void Function_Modbus(void)
* Description : 读取接收到的数据
* Para        : none
* Return(s)   : none
* Example     : Function_Modbus();
*********************************************************************************************************
*/
void Function_Modbus(void)// 触摸屏通讯;
{
		uint8_t bt=0;
		unsigned int Value_Temp[10];
	
		if(Rece_TimeOut==0) //接收到的数据大于3,且离上次接收到数据的最大间隔时限已等于0
		{
				if(Rece_RxCounter > 3)
				{
						bt = !REC_CRC_CHECK(&Rece_Buffer[0],Rece_RxCounter);//CRC校验正确返回零
						///* 判断信号：握手还是地址*/	
						if(bt == 1 )  // CRC校验正确
						{
							USART_ITConfig(USART1, USART_IT_RXNE, DISABLE); // 禁止接收中断;
							
							switch(MCGS_Sendform)
							{
								case 5:
									if(Auto_Running==0)		{MCGS_Message=0;	MCGS_Send(0,2,Video_Mode,0,0,0,0,0);}// 计数,汇总;
									else				MCGS_Send(0,1,Video_Mode,0,0,0,0,0);// 计数,汇总;
									MCGS_Sendform=0;
								break;
								case 10:
									MCGS_Message++;
									MCGS_Send(1,1,Video_Mode,Material_TimeOut,Material_Number,MCGS_Message,Material_Shape,Material_Color);// 计数,工作;
									MCGS_Sendform++;		//MCGS_A1=0;		
								break;
								case 11:
									//MCGS_Message++;
									MCGS_Send(1,1,Video_Mode,Material_TimeOut,Material_Number,MCGS_Message,Material_Shape,Material_Color);// 计数,工作;
									MCGS_Sendform=0;
								break;
								case 20:// MCGS触摸屏发送函数新修改;//20241020修改;
									MCGS_Sendform++;
											////////////////////////////////////////////////////////////////
									Cnt_i=0;Cnt_j=0;Cnt_k=1;Cnt_n=0;
									if(Cnt_i==0)
									{
											MCGS_Message=Cnt_k;
											Value_Temp[0] = 0;//Video_Mode
											Value_Temp[1] = Auto_RunTimesec;//Material_TimeOut
										if(Cnt_n < Collect_Static.SUM)
										{
											Value_Temp[2] = Collect_Static.Listcnt[Cnt_n];
											Value_Temp[3] = Collect_Static.Shape[Cnt_n];
											Value_Temp[4] = Collect_Static.Color[Cnt_n];
										}
										else
										{
											Value_Temp[2] = 0;
											Value_Temp[3] = Arg_ReadAdj.Shape;
											Value_Temp[4] = Arg_ReadAdj.Color;
										}
										if(Cnt_j==0)	Cnt_j=1;
										else
										{
											Cnt_j=0;	Cnt_k++;	Cnt_n++;
											if(Cnt_n < Collect_Static.SUM);
											else
											{
												Cnt_i++;
												Cnt_n=0;
											}
										}
									}
									MCGS_Send(1,2,Value_Temp[0],Value_Temp[1],Value_Temp[2],MCGS_Message,Value_Temp[3],Value_Temp[4]);// 计数,汇总;
											////////////////////////////////////////////////////////////////
									break;
								case 21:// MCGS触摸屏发送函数新修改;//20241020修改;
									if(Cnt_i==0)
									{
											MCGS_Message=Cnt_k;
											Value_Temp[0] = 0;//Video_Mode
											Value_Temp[1] = Auto_RunTimesec;//Material_TimeOut
										if(Cnt_n < Collect_Static.SUM)
										{
											Value_Temp[2] = Collect_Static.Listcnt[Cnt_n];
											Value_Temp[3] = Collect_Static.Shape[Cnt_n];
											Value_Temp[4] = Collect_Static.Color[Cnt_n];
										}
										else
										{
											Value_Temp[2] = 0;
											Value_Temp[3] = Arg_ReadAdj.Shape;
											Value_Temp[4] = Arg_ReadAdj.Color;
										}
										if(Cnt_j==0)	Cnt_j=1;
										else
										{
											Cnt_j=0;	Cnt_k++;	Cnt_n++;
											if(Cnt_n < Collect_Static.SUM);
											else
											{
												Cnt_i++;
												Cnt_n=0;
											}
										}
									}
									else if(Cnt_i==1)
									{
											MCGS_Message=Cnt_k;
											Value_Temp[0] = 1;//Video_Mode
											Value_Temp[1] = Auto_RunTimesec;//Material_TimeOut
										if(Cnt_n < Collect_Low.SUM)
										{
											Value_Temp[2] = Collect_Low.Listcnt[Cnt_n];
											Value_Temp[3] = Collect_Low.Shape[Cnt_n];
											Value_Temp[4] = Collect_Low.Color[Cnt_n];
										}
										else
										{
											Value_Temp[2] = 0;
											Value_Temp[3] = Arg_ReadAdj.Shape;
											Value_Temp[4] = Arg_ReadAdj.Color;
										}
										if(Cnt_j==0)	Cnt_j=1;
										else
										{
											Cnt_j=0;	Cnt_k++;	Cnt_n++;
											if(Cnt_n < Collect_Low.SUM);
											else
											{
												Cnt_i++;
												Cnt_n=0;
											}
										}
									}
									else
									{
											MCGS_Message=Cnt_k;
											Value_Temp[0] = 2;//Video_Mode
											Value_Temp[1] = Auto_RunTimesec;//Material_TimeOut
										if(Cnt_n < Collect_High.SUM)
										{
											Value_Temp[2] = Collect_High.Listcnt[Cnt_n];
											Value_Temp[3] = Collect_High.Shape[Cnt_n];
											Value_Temp[4] = Collect_High.Color[Cnt_n];
										}
										else
										{
											Value_Temp[2] = 0;
											Value_Temp[3] = Arg_ReadAdj.Shape;
											Value_Temp[4] = Arg_ReadAdj.Color;
										}
										if(Cnt_j==0)	Cnt_j=1;
										else
										{
											Cnt_j=0;	Cnt_k++;	Cnt_n++;
											if(Cnt_n < Collect_High.SUM);
											else
											{
												Cnt_i++;MCGS_Sendform=0;
												Cnt_n=0;
											}
										}
									}
											////////////////////////////////////////////////////////////////
									MCGS_Send(1,2,Value_Temp[0],Value_Temp[1],Value_Temp[2],MCGS_Message,Value_Temp[3],Value_Temp[4]);// 计数,汇总;
											////////////////////////////////////////////////////////////////
								break;
								default:
								break;
							}
							
						}
						
				}	
				
				Rece_RxCounter =0; //接收到的字节个数清零
					// 数据接收错误，清除缓冲区
				Rece_Buffer[0] = 0;            Rece_Buffer[1] = 0;
				Rece_Buffer[2] = 0;            Rece_Buffer[3] = 0;
				Rece_Buffer[4] = 0;            Rece_Buffer[5] = 0;
				Rece_Buffer[6] = 0;            Rece_Buffer[7] = 0;
				
				RS_485_RX();		// 485方向转换为接收;
				USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	// 使能接收中断;
		 }
}


/*******************************************************************************
* Function Name  : void USART1_IRQHandler(void)
* Description    : USART1 Interrupt.485
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART1_IRQHandler(void)
{
		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
		{	
			USART_ReceiveData(USART1); 
			
			/* Read one byte from the receive data register */
 			if((Rece_TimeOut==0)&&(Rece_RxCounter == 0))      //两次接收的间隔时间大于OUTTIME，起始位
 			{
 				Rece_bit = 1 ;    
 				Rece_Buffer[Rece_RxCounter++] = USART_ReceiveData(USART1);     // RxCounter[1] = 0 起始位
 			}
 			else if(Rece_TimeOut== 1)
 			{
 				Rece_Buffer[Rece_RxCounter++] = USART_ReceiveData(USART1); 	
 			}
				Rece_Time=0;		
 			Rece_TimeOut = 1;          //置计时寄存器 该寄存器 在中断0中，
		}
}



