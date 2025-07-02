/*
 * @file          UART_TFTLCD.c
 * @brief         ����: ����ͨѶ-204A(TFTLCD��ʾ�������)-���ֺ���;
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
#include "delay.h"


int Rx2_cnt=0;	// 	�������ݻ�����,��ַ����;
unsigned int Rx2_buf[15]={0};	// 	�������ݻ�����;

/*******************************************************************************
* Function Name  : void USART2_IRQHandler(void)
* Description    : USART2 Interrupt.// ��204A��������,��þ���������������״ֵ̬; Matrixkey_val,Presskey_val;
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_IRQHandler(void)
{ 
		unsigned int Tmp_Rxd2;
	
		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
		{
			Tmp_Rxd2 = USART_ReceiveData(USART2);   //�����Ĵ��������ݻ��浽���ջ�������
			
			if(Tmp_Rxd2==0XC1)	Rx2_cnt=1;// ֡ͷ;
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
					if((Rx2_buf[Rx2_cnt-2]==0xFD) && (Rx2_buf[Rx2_cnt-1]==0xFC))// ֡β;
					{
						Matrixkey_val = Rx2_buf[2];	// �������ֵ����;
						Presskey_val = Rx2_buf[3];	// ��������ֵ����;
					}
					Rx2_cnt=0;
					break;
				default:	break;
			}
		}
		
		if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)                   //�����Ϊ�˱���STM32 USART ��һ���ֽڷ�����ȥ��BUG 
		{ 
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);					     //��ֹ�����������жϣ� 
		}	
}



/**********************************************************************************************************
* Function    ��void LCD_DealData(void)
* Description : �������ݸ��´��� ;
* Para        : none
* Return(s)   : none
* Example     : LCD_DealData();
**********************************************************************************************************/
void LCD_DealData(void)
{
	if(LCD_ReTime == 1)
	{
		LCD_ReTime=0;
			LCD_Data[2] = 9;// ������;
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
				LCD_Data[2] = 1;// ������;
				LCD_Data[3] = 5;// ��λ��;5;
				LCD_Data[4] = 1;// ��˸ʱ����;(s);1S;
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
				LCD_Data[2] = 2;// ������;
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
				LCD_Data[2] = 10;// ������;
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
				LCD_Data[2] = 3;// ������;
				LCD_Data[3] = Laser_State;// �����״̬;0,close;1,open;
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
				LCD_Data[2] = 4;// ������;
				LCD_Data[3] = Video_Mode+1;// ��Ƶ�Ѷ�״̬;0;1;2;
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
			if(Page_Up==1)// "Up";���Ϸ�ҳ;
			{
				Page_Up=0;
				LCD_Data[2] = 7;// ������;
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
			else if(Page_Down==1)// "Down";���·�ҳ;
			{
				Page_Down=0;
				LCD_Data[2] = 8;// ������;
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
				LCD_Data[2] = 5;// ������;
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
				LCD_Data[2] = 5;// ������;
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
				LCD_Data[2] = 11;// ������;
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
				LCD_Data[2] = 6;// ������;
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
				LCD_Data[2] = 12;// ������;
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
				LCD_Data[2] = 14;// ������;
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
				LCD_Data[2] = 3;// ������;
				LCD_Data[3] = Laser_State;// �����״̬;0,close;1,open;
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
			
			case 20:// ʾ������ʾ;////20241020����;
				LCD_Data[2] = 0x00;// ������;
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
		        LCD_Data[0] = 0xA1;// ֡ͷ;
				if(LCD_flag==20)// ʾ������ʾ;////20241020����;
				{
					LCD_Data[0] = 0xB1;// ֡ͷ;
				}
				LCD_Data[1] = 0x11;// �����ֽ���;
				LCD_Data[20] = 0xFD;// βУ��;
				LCD_Data[21] = 0xFC;// βУ��;
	
				USART_Send_Data02(1,&LCD_Data[0],22);
				delay_ms(50);
	
}



