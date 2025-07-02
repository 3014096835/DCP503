/*
 * @file          UART_485.c
 * @brief         ����: ����ͨѶ-������RS485-���ֺ���;
 * @ucos-ii       V2.86 
 * @function      QSS-ICBS1 
 * @compiler      RealView MDK-ARM Compiler  V5.14
 * @peripheral    V3.5.0
 * @author  	  	
 * @date   	      2024/05/01
 * @corporation   �㽭���ǿƽ��豸���޹�˾
 * @website       http://www.zjqiushi.com/
*/
/* Includes ------------------------------------------------------------------*/
#include	"USART.h"
#include  "BoardVariable.h"


/**********************************************************************************************************
* Function    ��void Massage_Post(void)
* Description : ��Ϣ����;Һ����, ������;
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
		
		MCGS_Sendform=10;// ������;���µ�ǰʶ����Ʒ��Ϣ;
		////////////////////////////////////////////////////////////////
}


/**********************************************************************************************************
* Function    ��void Massage_PostEnd(void)
* Description : �������;������;
* Para        : none
* Return(s)   : none
* Example     : Massage_PostEnd();
**********************************************************************************************************/
void Massage_PostEnd(void)
{
		MCGS_Sendform=20;// ������;���½���������Ϣ;
}


/**********************************************************************************************************
* Function    ��void MCGS_Send(uint8_t M_Cnt,uint8_t M_State,uint8_t M_Mode,long M_Time,uint8_t M_Num,uint8_t M_Message)
* Description : ��������άϵͳ���ĸ���;//2023��������״���������ʹ��;
* 							M_Cnt,����Ϣ�Ƿ����;
* 							M_State,��Ϣ,����״̬; M_Mode,��Ϣ,����ģʽ; M_Time,��Ϣ,����ʱ��;M_Num,��Ϣ,��Ʒ����;
* Para        : none
* Return(s)   : none
* Example     : MCGS_Send();
**********************************************************************************************************/
void MCGS_Send(uint8_t M_Cnt,uint8_t M_State,uint8_t M_Mode,long M_Time,uint8_t M_Num,uint8_t M_Message
								,uint8_t M_Shape,uint8_t M_Color)// MCGS���������ͺ������޸�;//20241020�޸�;
{
	if(M_Cnt==1)
	{
		MCGS_TxBuffer[0] = 0x01;  // ������ַ
		MCGS_TxBuffer[1] = 0x04;  // ������
		MCGS_TxBuffer[2] = 26;
		
		MCGS_TxBuffer[3] = 0x00;		MCGS_TxBuffer[4] = M_State;							// ����״̬; 1,����; 2,����;
		MCGS_TxBuffer[5] = 0x00;		MCGS_TxBuffer[6] = M_Mode+1;						// ����ģʽ;
		MCGS_TxBuffer[7] = M_Time>>8;		// ����ʱ��;(s)
		MCGS_TxBuffer[8] = M_Time&0xff;
			MCGS_TxBuffer[9] = 0x00;		MCGS_TxBuffer[10] = M_Shape;	// ������״;//+5*(Arg_ReadAdj.Type-1)	
			MCGS_TxBuffer[11] = 0x00;		MCGS_TxBuffer[12] = M_Color;	// ������ɫ;
		MCGS_TxBuffer[13] = 0x00;		MCGS_TxBuffer[14] = M_Num;							// ��������;
		MCGS_TxBuffer[15] = 0x00;		MCGS_TxBuffer[16] = M_Message;					// ���;
		if(Auto_Running==0)	//ֹͣ���ʱ;
		{
			MCGS_TxBuffer[17] = 0;	MCGS_TxBuffer[18] = 0;	// X������;
			MCGS_TxBuffer[19] = 0;	MCGS_TxBuffer[20] = 0;	// Y������;
		}
		else
		{
			MCGS_TxBuffer[17] = Laser_ScreenX/256;	MCGS_TxBuffer[18] = Laser_ScreenX%256;	// X������;
			MCGS_TxBuffer[19] = Laser_ScreenY/256;	MCGS_TxBuffer[20] = Laser_ScreenY%256;	// Y������;
		}
		MCGS_TxBuffer[21] = 0x00;		MCGS_TxBuffer[22] = MaterialF1_Num;			// ģʽ1����(F1);
		MCGS_TxBuffer[23] = 0x00;		MCGS_TxBuffer[24] = MaterialF2_Num;			// ģʽ2����(F2);
		MCGS_TxBuffer[25] = 0x00;		MCGS_TxBuffer[26] = MaterialF3_Num;			// ģʽ3����(F3);
		MCGS_TxBuffer[27] = 0x00;		MCGS_TxBuffer[28] = Select_Background;	// ����ɫ;1~2;// 1,��ɫ;2,��ɫ;
			
			Send_CRC = usMBCRC16( MCGS_TxBuffer, 29 );   
		MCGS_TxBuffer[29] = Send_CRC &0xff;
		MCGS_TxBuffer[30] = Send_CRC >> 8;		// ��У��λ���뷢�ͻ���

    USART_Send_Data02(0, MCGS_TxBuffer,31);
	}
	else
	{
		MCGS_TxBuffer[0] = 0x01;  // ������ַ
		MCGS_TxBuffer[1] = 0x04;  // ������
		MCGS_TxBuffer[2] = 26;
		
		MCGS_TxBuffer[3] = 0x00;		MCGS_TxBuffer[4] = M_State;						// ����״̬; 1,����; 2,����;
		MCGS_TxBuffer[5] = 0x00;		MCGS_TxBuffer[6] = M_Mode+1;					// ����ģʽ;
		MCGS_TxBuffer[15] = 0x00;		MCGS_TxBuffer[16] = 0;								// ���;0
		
		MCGS_TxBuffer[17] = 0;			MCGS_TxBuffer[18] = 0;	// X������;
		MCGS_TxBuffer[19] = 0;			MCGS_TxBuffer[20] = 0;	// Y������;
		MCGS_TxBuffer[21] = 0x00;		MCGS_TxBuffer[22] = MaterialF1_Num;		// ģʽ1����(F1);
		MCGS_TxBuffer[23] = 0x00;		MCGS_TxBuffer[24] = MaterialF2_Num;		// ģʽ2����(F2);
		MCGS_TxBuffer[25] = 0x00;		MCGS_TxBuffer[26] = MaterialF3_Num;		// ģʽ3����(F3);
		
			Send_CRC = usMBCRC16( MCGS_TxBuffer, 29 );   
		MCGS_TxBuffer[29] = Send_CRC &0xff;
		MCGS_TxBuffer[30] = Send_CRC >> 8;		// ��У��λ���뷢�ͻ���

    USART_Send_Data02(0, MCGS_TxBuffer,31);
	}
}


// MCGS���������ͺ������޸�;//20241020�޸�;
unsigned int Cnt_i=0;
unsigned int Cnt_j=0;
unsigned int Cnt_k=0;
unsigned int Cnt_n=0;

/*
*********************************************************************************************************
* Function    ��void Function_Modbus(void)
* Description : ��ȡ���յ�������
* Para        : none
* Return(s)   : none
* Example     : Function_Modbus();
*********************************************************************************************************
*/
void Function_Modbus(void)// ������ͨѶ;
{
		uint8_t bt=0;
		unsigned int Value_Temp[10];
	
		if(Rece_TimeOut==0) //���յ������ݴ���3,�����ϴν��յ����ݵ������ʱ���ѵ���0
		{
				if(Rece_RxCounter > 3)
				{
						bt = !REC_CRC_CHECK(&Rece_Buffer[0],Rece_RxCounter);//CRCУ����ȷ������
						///* �ж��źţ����ֻ��ǵ�ַ*/	
						if(bt == 1 )  // CRCУ����ȷ
						{
							USART_ITConfig(USART1, USART_IT_RXNE, DISABLE); // ��ֹ�����ж�;
							
							switch(MCGS_Sendform)
							{
								case 5:
									if(Auto_Running==0)		{MCGS_Message=0;	MCGS_Send(0,2,Video_Mode,0,0,0,0,0);}// ����,����;
									else				MCGS_Send(0,1,Video_Mode,0,0,0,0,0);// ����,����;
									MCGS_Sendform=0;
								break;
								case 10:
									MCGS_Message++;
									MCGS_Send(1,1,Video_Mode,Material_TimeOut,Material_Number,MCGS_Message,Material_Shape,Material_Color);// ����,����;
									MCGS_Sendform++;		//MCGS_A1=0;		
								break;
								case 11:
									//MCGS_Message++;
									MCGS_Send(1,1,Video_Mode,Material_TimeOut,Material_Number,MCGS_Message,Material_Shape,Material_Color);// ����,����;
									MCGS_Sendform=0;
								break;
								case 20:// MCGS���������ͺ������޸�;//20241020�޸�;
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
									MCGS_Send(1,2,Value_Temp[0],Value_Temp[1],Value_Temp[2],MCGS_Message,Value_Temp[3],Value_Temp[4]);// ����,����;
											////////////////////////////////////////////////////////////////
									break;
								case 21:// MCGS���������ͺ������޸�;//20241020�޸�;
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
									MCGS_Send(1,2,Value_Temp[0],Value_Temp[1],Value_Temp[2],MCGS_Message,Value_Temp[3],Value_Temp[4]);// ����,����;
											////////////////////////////////////////////////////////////////
								break;
								default:
								break;
							}
							
						}
						
				}	
				
				Rece_RxCounter =0; //���յ����ֽڸ�������
					// ���ݽ��մ������������
				Rece_Buffer[0] = 0;            Rece_Buffer[1] = 0;
				Rece_Buffer[2] = 0;            Rece_Buffer[3] = 0;
				Rece_Buffer[4] = 0;            Rece_Buffer[5] = 0;
				Rece_Buffer[6] = 0;            Rece_Buffer[7] = 0;
				
				RS_485_RX();		// 485����ת��Ϊ����;
				USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	// ʹ�ܽ����ж�;
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
 			if((Rece_TimeOut==0)&&(Rece_RxCounter == 0))      //���ν��յļ��ʱ�����OUTTIME����ʼλ
 			{
 				Rece_bit = 1 ;    
 				Rece_Buffer[Rece_RxCounter++] = USART_ReceiveData(USART1);     // RxCounter[1] = 0 ��ʼλ
 			}
 			else if(Rece_TimeOut== 1)
 			{
 				Rece_Buffer[Rece_RxCounter++] = USART_ReceiveData(USART1); 	
 			}
				Rece_Time=0;		
 			Rece_TimeOut = 1;          //�ü�ʱ�Ĵ��� �üĴ��� ���ж�0�У�
		}
}



