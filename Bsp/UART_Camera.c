/*
 * @file          UART_Camera.c
 * @brief         ����: ����ͨѶ-������Ӿ�ʶ��-���ֺ���;
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


#define DMA_REC_LEN 200  //DMA���ݽ��ջ���������;
unsigned char Receive4_buff[DMA_REC_LEN] = {0};  //DMA���ݽ��ջ�����;
unsigned int UART4_DatUpdate=0;						// ��������֡���±��;
unsigned int UART4_Datbuffer[30][50]={0};	// ��������֡������,һ��һ֡����;
unsigned int UART4_DatIndex=0;						// ��������֡������,�е�ַ����;

// UART4-DMA����ͨ������;
void UART4Rx_DMA_Init(void)
{
		DMA_InitTypeDef  DMA_IniStructure;

		RCC_AHBPeriphClockCmd( RCC_AHBPeriph_DMA2, ENABLE );
		DMA_DeInit( DMA2_Channel3 );
		DMA_IniStructure.DMA_PeripheralBaseAddr = ( u32 )&UART4->DR;
		DMA_IniStructure.DMA_MemoryBaseAddr = ( u32 )Receive4_buff;
		DMA_IniStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
		DMA_IniStructure.DMA_BufferSize = DMA_REC_LEN;

		DMA_IniStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_IniStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_IniStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_IniStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_IniStructure.DMA_Mode = DMA_Mode_Normal;
		DMA_IniStructure.DMA_Priority = DMA_Priority_Medium;
		DMA_IniStructure.DMA_M2M = DMA_M2M_Disable;

		DMA_Init( DMA2_Channel3, &DMA_IniStructure );
		DMA_Cmd( DMA2_Channel3, ENABLE );
}

// ����DMA����;
void UART4Rx_DMA_Enable(void)
{
		DMA_Cmd( DMA2_Channel3, DISABLE );// �ر�DMA;
		DMA_SetCurrDataCounter( DMA2_Channel3, DMA_REC_LEN );	// ����Ҫ���յ��ֽڳ���;
		DMA_Cmd( DMA2_Channel3, ENABLE );// ����DMA;
}

// �ر�DMA����;
void UART4Rx_DMA_Disable(void)
{
		DMA_Cmd( DMA2_Channel3, DISABLE );// �ر�DMA;
}


/*******************************************************************************
* Function Name  : void UART4_IRQHandler(void)
* Description    : UART4 Interrupt.// ���Ӿ������������,��õ�ǰʶ������Ϣ;
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART4_IRQHandler( void )
{
		unsigned int i;
		unsigned int Rx4_cnt;
	
		if( USART_GetITStatus( UART4, USART_IT_IDLE ) != RESET )
		{
			USART_ReceiveData( UART4 );
			UART4Rx_DMA_Disable();	// �ر�DMA,������;
			
			Rx4_cnt = DMA_REC_LEN - DMA_GetCurrDataCounter( DMA2_Channel3 );// ��ý��յ����ֽ���;
			UART4_DatUpdate = 1;			// һ֡���ݽ�����ɱ�־λ;

			//for(i=0;i<50;i++)		{	Cam_Datbuffer[Cam_DatCnt][i] = 0;	}	// �������������;// �ݲ����,�������ݽ���ʱ�����;
			UART4_Datbuffer[UART4_DatIndex][0] = Rx4_cnt;							// �ٽ��ջ�������;��һ����ַ���浱ǰ����֡����;
			for(i=0;i<Rx4_cnt;i++)
			{
				UART4_Datbuffer[UART4_DatIndex][i+1] = Receive4_buff[i];			// �ٽ��ջ�������;�ӵڶ�����ַ��ʼ����;
			}
			if(UART4_DatIndex<28)	UART4_DatIndex++;														// ��ǰ��������֡����;���һ������֡����Ϊ UART4_DatIndex-1 �� 28;
			else			UART4_DatIndex=0;
			
			USART_ClearITPendingBit( UART4, USART_IT_IDLE );					// USART_IT_IDLE, �����־λ;
				//UART5->SR; //�ȶ�SR�Ĵ���;���IDLEλ;1;
				//UART5->DR; //�ٶ�DR�Ĵ���;���IDLEλ;2;
			UART4Rx_DMA_Enable();	// ����DMA,�ȴ����ݽ���;
		}
		if(USART_GetITStatus(UART4, USART_IT_TXE) != RESET)                   //�����Ϊ�˱���STM32 USART ��һ���ֽڷ�����ȥ��BUG 
		{
			USART_ITConfig(UART4, USART_IT_TXE, DISABLE);					     //��ֹ�����������жϣ� 
		}
}



/*******************************************************************************
* Function Name  : void Camera_RxRefresh(void)
* Description    : ���Ӿ������õ�ǰʶ������Ϣ,һ֡���ݽ�������뻺����������,��Ч����֡����;
* Input          : None
* Output         : None
* Return         : Camera_RxRefresh()
*******************************************************************************/
void Camera_RxRefresh(void)
{
	unsigned int i;
	unsigned int tmp_Index=0;
	
	if(UART4_DatUpdate==1)		// ���µ�����֡����;
	{
		tmp_Index = UART4_DatIndex;	// ��ǰ����֡����; UART4_DatIndex-1 �� 28;
		UART4_DatUpdate = 0;		// ���������֡��־λ;
		
			if(tmp_Index==0)tmp_Index = 28;
			else tmp_Index = tmp_Index-1;		// ��ǰ����֡����;���һ������֡����Ϊ Cam_DatCnt-1 �� 28;
			Cam_RxNum = UART4_Datbuffer[tmp_Index][0];		// ��ǰ����֡����;
				memcpy(Cam_RxBuffer, Res_PosInit, 200);			// �������������;
			for(i=0;i<Cam_RxNum+1;i++)										// �ٽ��ջ�������;��ǰ����֡����;
			{
				Cam_RxBuffer[i] = UART4_Datbuffer[tmp_Index][i+1];
			}
			Cam_Rxflag = 1;					// ���յ�������ݸ��±�־λ;
						
			// ���յ���������Ϣ��ʽ��ȷ,�������Ϣ;
			// ��ģ��ʶ��ģʽ/AI�߼�ʶ��ģʽ��ͨѶЭ�鲻ͬ;//20241020�޸�;
			if( (Cam_RxBuffer[0] == 'T') && (Cam_RxBuffer[1] == ';') )
			{
				Camera_mode=1;// =0,Ĭ�ϵ�ģ��ģʽ;=1,�߼�ʶ��ģʽ;
						//Camera_TxRefresh(0);	// �����������;
			}
			else if((Cam_RxBuffer[1] == ';') && (Cam_RxBuffer[3] == ';') && (Cam_RxBuffer[4] != 0))
			{
				Camera_mode=0;// =0,Ĭ�ϵ�ģ��ģʽ;=1,�߼�ʶ��ģʽ;
				// ��ǰ��������������ҵ�Ҫ��ʱ,ǿ��������·���;
				if((Data_AsciiToDec(Cam_RxBuffer[0]) != Arg_ReadAdj.Shape) || (Data_AsciiToDec(Cam_RxBuffer[2]) != Arg_ReadAdj.Color))
				{
						Camera_TxRefresh(0);	// �����������;
				}
			}
			else
				Cam_Rxflag=0;
	}
	else	Cam_Rxflag=0;
	
}


uint16_t AI_NUM=0;
uint16_t AI_List01,AI_List02,AI_List03;


/*******************************************************************************
* Function Name  : void Camera_TxRefresh(void)
* Description    : ���Ӿ��������ָ��;// �滻 Refresh_PicTxSel();	// �����������;
* Input          : None
* Output         : None
* Return         : Camera_TxRefresh()
*******************************************************************************/
void Camera_TxRefresh(unsigned int T_Beat)
{
	unsigned int ai_cnti=0;
	USB_BeatNUM=0;
	
	// ��ģ��ʶ��ģʽ/AI�߼�ʶ��ģʽ��ͨѶЭ�鲻ͬ;//20241020�޸�;
	if(Camera_mode==0)	// =0,Ĭ�ϵ�ģ��ģʽ;=1,�߼�ʶ��ģʽ;
	{
		if(T_Beat == 0)
		{
			// ���������,�����������ָ��;
				//memcpy(Cam_TxBuffer, "12 12", 5);	
			Cam_TxBuffer[0] = 0x30+Arg_ReadAdj.Shape;
			Cam_TxBuffer[1] = 0x30+Arg_ReadAdj.Color;
			Cam_TxBuffer[2] = ' ';
			Cam_TxBuffer[3] = 0x30+Arg_ReadAdj.Shape;
			Cam_TxBuffer[4] = 0x30+Arg_ReadAdj.Color;
		
			USART_Send_Data02(3, Cam_TxBuffer, 5);	// �������ݴ���,���� 5 �ֽ�����
		}
		else
		{
			// ���������,�����������ָ��;
				//memcpy(Cam_TxBuffer, "OK", 5);	
			Cam_TxBuffer[0] = 'O';
			Cam_TxBuffer[1] = 'K';
			Cam_TxBuffer[2] = 0X0D;
			Cam_TxBuffer[3] = 0X0A;
		
			USART_Send_Data02(3, Cam_TxBuffer, 4);	// �������ݴ���,���� 5 �ֽ�����
		}
	}
	else	// =0,Ĭ�ϵ�ģ��ģʽ;=1,�߼�ʶ��ģʽ;
	{
		if((T_Beat == 0)&&(AI_NUM>0))
		{
			// ���������,�����������ָ��;
			Cam_TxBuffer[0] = 'T';
			Cam_TxBuffer[1] = ';';
			ai_cnti=2;
				if(AI_NUM>=1)	
				{
					if(AI_List01<9)	Cam_TxBuffer[ai_cnti++] = 0x30+AI_List01;
					else
					{
						Cam_TxBuffer[ai_cnti++] = 0x30+AI_List01/10%10;
						Cam_TxBuffer[ai_cnti++] = 0x30+AI_List01%10;
					}
					Cam_TxBuffer[ai_cnti++] = ';';
				}
				if(AI_NUM>=2)	
				{
					if(AI_List02<9)	Cam_TxBuffer[ai_cnti++] = 0x30+AI_List02;
					else
					{
						Cam_TxBuffer[ai_cnti++] = 0x30+AI_List02/10%10;
						Cam_TxBuffer[ai_cnti++] = 0x30+AI_List02%10;
					}
					Cam_TxBuffer[ai_cnti++] = ';';
				}
				if(AI_NUM>=3)	
				{
					if(AI_List03<9)	Cam_TxBuffer[ai_cnti++] = 0x30+AI_List03;
					else
					{
						Cam_TxBuffer[ai_cnti++] = 0x30+AI_List03/10%10;
						Cam_TxBuffer[ai_cnti++] = 0x30+AI_List03%10;
					}
					Cam_TxBuffer[ai_cnti++] = ';';
				}
			Cam_TxBuffer[ai_cnti++] = 0X0D;
			Cam_TxBuffer[ai_cnti++] = 0X0A;
		
			USART_Send_Data02(3, Cam_TxBuffer, ai_cnti);	// �������ݴ���,���� 5 �ֽ�����
		}
	}
}

// AI�߼�ʶ��ģʽ��ͨѶЭ�鲻ͬ;//20241020�޸�;
///////////////////////////////////////////////////////////////////////////////
///
///**   ʹ�ô˹���ʱ��λ���Ӿ����ͨѶ��, "�����·���" �������ȡ����ѡ;   **//
///
///////////////////////////////////////////////////////////////////////////////
//
// ����������λ������ָ��,����ʶ����;
// 			Camera_AIselect(pic_num, pic_list1, pic_list2, pic_list3);
// ���÷���: 	
//			pic_num, ͬʱʶ����Ʒ������, 1~3; 
//			pic_list1,pic_list2,pic_list3; �ֱ�Ϊʶ��ĵ�1������2������3����Ʒ���;
// 			��Ųο�ʶ������߼�ʶ���б�����;��Ŵ�0��ʼ����,��20����Ʒ,��ΧΪ 0~19;
// ����, ʶ��һ����Ʒ, ���Ϊ7����Ʒ; 				=>	Camera_AIselect(1,7,0,0);
// ����, ʶ��������Ʒ, ���Ϊ7��13����Ʒ; 		=>	Camera_AIselect(2,7,13,0);
// ����, ʶ��������Ʒ, ���Ϊ0��12��15����Ʒ; =>	Camera_AIselect(3,0,12,15);
//
///////////////////////////////////////////////////////////////////////////////
void Camera_AIselect(uint16_t pic_num, uint16_t pic_list1, uint16_t pic_list2, uint16_t  pic_list3)
{
		Camera_mode=1;// =0,Ĭ�ϵ�ģ��ģʽ;=1,�߼�ʶ��ģʽ;
	
		AI_NUM = pic_num;
		AI_List01 = pic_list1;
		AI_List02 = pic_list2;
		AI_List03 = pic_list3;
		Camera_TxRefresh(0);
}

