/*
 * @file          UART_Camera.c
 * @brief         程序: 串口通讯-摄像机视觉识别-部分函数;
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


#define DMA_REC_LEN 200  //DMA数据接收缓冲区长度;
unsigned char Receive4_buff[DMA_REC_LEN] = {0};  //DMA数据接收缓冲区;
unsigned int UART4_DatUpdate=0;						// 串口数据帧更新标记;
unsigned int UART4_Datbuffer[30][50]={0};	// 串口数据帧缓存区,一行一帧数据;
unsigned int UART4_DatIndex=0;						// 串口数据帧缓存区,行地址索引;

// UART4-DMA接收通道配置;
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

// 启动DMA接收;
void UART4Rx_DMA_Enable(void)
{
		DMA_Cmd( DMA2_Channel3, DISABLE );// 关闭DMA;
		DMA_SetCurrDataCounter( DMA2_Channel3, DMA_REC_LEN );	// 设置要接收的字节长度;
		DMA_Cmd( DMA2_Channel3, ENABLE );// 启动DMA;
}

// 关闭DMA接收;
void UART4Rx_DMA_Disable(void)
{
		DMA_Cmd( DMA2_Channel3, DISABLE );// 关闭DMA;
}


/*******************************************************************************
* Function Name  : void UART4_IRQHandler(void)
* Description    : UART4 Interrupt.// 从视觉软件接收数据,获得当前识别结果信息;
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
			UART4Rx_DMA_Disable();	// 关闭DMA,防干扰;
			
			Rx4_cnt = DMA_REC_LEN - DMA_GetCurrDataCounter( DMA2_Channel3 );// 获得接收到的字节数;
			UART4_DatUpdate = 1;			// 一帧数据接收完成标志位;

			//for(i=0;i<50;i++)		{	Cam_Datbuffer[Cam_DatCnt][i] = 0;	}	// 先清除缓存内容;// 暂不清除,分析数据接收时会清除;
			UART4_Datbuffer[UART4_DatIndex][0] = Rx4_cnt;							// 再接收缓存内容;第一个地址缓存当前数据帧长度;
			for(i=0;i<Rx4_cnt;i++)
			{
				UART4_Datbuffer[UART4_DatIndex][i+1] = Receive4_buff[i];			// 再接收缓存内容;从第二个地址开始缓存;
			}
			if(UART4_DatIndex<28)	UART4_DatIndex++;														// 当前缓存数据帧索引;最后一条数据帧索引为 UART4_DatIndex-1 或 28;
			else			UART4_DatIndex=0;
			
			USART_ClearITPendingBit( UART4, USART_IT_IDLE );					// USART_IT_IDLE, 清除标志位;
				//UART5->SR; //先读SR寄存器;清除IDLE位;1;
				//UART5->DR; //再读DR寄存器;清除IDLE位;2;
			UART4Rx_DMA_Enable();	// 开启DMA,等待数据接收;
		}
		if(USART_GetITStatus(UART4, USART_IT_TXE) != RESET)                   //这段是为了避免STM32 USART 第一个字节发不出去的BUG 
		{
			USART_ITConfig(UART4, USART_IT_TXE, DISABLE);					     //禁止发缓冲器空中断， 
		}
}



/*******************************************************************************
* Function Name  : void Camera_RxRefresh(void)
* Description    : 从视觉软件获得当前识别结果信息,一帧数据结束后存入缓存区待处理,有效数据帧则保留;
* Input          : None
* Output         : None
* Return         : Camera_RxRefresh()
*******************************************************************************/
void Camera_RxRefresh(void)
{
	unsigned int i;
	unsigned int tmp_Index=0;
	
	if(UART4_DatUpdate==1)		// 有新的数据帧出现;
	{
		tmp_Index = UART4_DatIndex;	// 当前数据帧索引; UART4_DatIndex-1 或 28;
		UART4_DatUpdate = 0;		// 清除新数据帧标志位;
		
			if(tmp_Index==0)tmp_Index = 28;
			else tmp_Index = tmp_Index-1;		// 当前数据帧索引;最后一条数据帧索引为 Cam_DatCnt-1 或 28;
			Cam_RxNum = UART4_Datbuffer[tmp_Index][0];		// 当前数据帧长度;
				memcpy(Cam_RxBuffer, Res_PosInit, 200);			// 先清除缓存内容;
			for(i=0;i<Cam_RxNum+1;i++)										// 再接收缓存内容;当前数据帧内容;
			{
				Cam_RxBuffer[i] = UART4_Datbuffer[tmp_Index][i+1];
			}
			Cam_Rxflag = 1;					// 接收到相机数据更新标志位;
						
			// 接收到的数据信息格式正确,则接收信息;
			// 单模型识别模式/AI高级识别模式：通讯协议不同;//20241020修改;
			if( (Cam_RxBuffer[0] == 'T') && (Cam_RxBuffer[1] == ';') )
			{
				Camera_mode=1;// =0,默认单模型模式;=1,高级识别模式;
						//Camera_TxRefresh(0);	// 更改相机方案;
			}
			else if((Cam_RxBuffer[1] == ';') && (Cam_RxBuffer[3] == ';') && (Cam_RxBuffer[4] != 0))
			{
				Camera_mode=0;// =0,默认单模型模式;=1,高级识别模式;
				// 当前相机方案不符合我的要求时,强制相机更新方案;
				if((Data_AsciiToDec(Cam_RxBuffer[0]) != Arg_ReadAdj.Shape) || (Data_AsciiToDec(Cam_RxBuffer[2]) != Arg_ReadAdj.Color))
				{
						Camera_TxRefresh(0);	// 更改相机方案;
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
* Description    : 向视觉软件发送指令;// 替换 Refresh_PicTxSel();	// 更改相机方案;
* Input          : None
* Output         : None
* Return         : Camera_TxRefresh()
*******************************************************************************/
void Camera_TxRefresh(unsigned int T_Beat)
{
	unsigned int ai_cnti=0;
	USB_BeatNUM=0;
	
	// 单模型识别模式/AI高级识别模式：通讯协议不同;//20241020修改;
	if(Camera_mode==0)	// =0,默认单模型模式;=1,高级识别模式;
	{
		if(T_Beat == 0)
		{
			// 发送至相机,更新相机方案指令;
				//memcpy(Cam_TxBuffer, "12 12", 5);	
			Cam_TxBuffer[0] = 0x30+Arg_ReadAdj.Shape;
			Cam_TxBuffer[1] = 0x30+Arg_ReadAdj.Color;
			Cam_TxBuffer[2] = ' ';
			Cam_TxBuffer[3] = 0x30+Arg_ReadAdj.Shape;
			Cam_TxBuffer[4] = 0x30+Arg_ReadAdj.Color;
		
			USART_Send_Data02(3, Cam_TxBuffer, 5);	// 发送数据处理,发送 5 字节数据
		}
		else
		{
			// 发送至相机,更新相机方案指令;
				//memcpy(Cam_TxBuffer, "OK", 5);	
			Cam_TxBuffer[0] = 'O';
			Cam_TxBuffer[1] = 'K';
			Cam_TxBuffer[2] = 0X0D;
			Cam_TxBuffer[3] = 0X0A;
		
			USART_Send_Data02(3, Cam_TxBuffer, 4);	// 发送数据处理,发送 5 字节数据
		}
	}
	else	// =0,默认单模型模式;=1,高级识别模式;
	{
		if((T_Beat == 0)&&(AI_NUM>0))
		{
			// 发送至相机,更新相机方案指令;
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
		
			USART_Send_Data02(3, Cam_TxBuffer, ai_cnti);	// 发送数据处理,发送 5 字节数据
		}
	}
}

// AI高级识别模式：通讯协议不同;//20241020修改;
///////////////////////////////////////////////////////////////////////////////
///
///**   使用此功能时上位机视觉软件通讯处, "不更新方案" 必须必须取消勾选;   **//
///
///////////////////////////////////////////////////////////////////////////////
//
// 控制器向上位机传输指令,控制识别项;
// 			Camera_AIselect(pic_num, pic_list1, pic_list2, pic_list3);
// 调用方法: 	
//			pic_num, 同时识别物品类数量, 1~3; 
//			pic_list1,pic_list2,pic_list3; 分别为识别的第1个、第2个、第3个物品序号;
// 			序号参考识别软件高级识别列表数量;序号从0开始计数,如20类物品,范围为 0~19;
// 例如, 识别一个物品, 序号为7号物品; 				=>	Camera_AIselect(1,7,0,0);
// 例如, 识别两个物品, 序号为7、13号物品; 		=>	Camera_AIselect(2,7,13,0);
// 例如, 识别三个物品, 序号为0、12、15号物品; =>	Camera_AIselect(3,0,12,15);
//
///////////////////////////////////////////////////////////////////////////////
void Camera_AIselect(uint16_t pic_num, uint16_t pic_list1, uint16_t pic_list2, uint16_t  pic_list3)
{
		Camera_mode=1;// =0,默认单模型模式;=1,高级识别模式;
	
		AI_NUM = pic_num;
		AI_List01 = pic_list1;
		AI_List02 = pic_list2;
		AI_List03 = pic_list3;
		Camera_TxRefresh(0);
}

