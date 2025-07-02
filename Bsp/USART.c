/*
 * @file          UART.c
 * @brief         程序: 串口通讯-配置-部分函数;
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
#include 	"delay.h"
#include  "USART.h"
#include  "mbcrc.h"


/*
*********************************************************************************************************
* Function    ：void UART_Init(void)
* Description : 所有串口模块初始化 
* Para        : none
* Return(s)   : none
* Example     : UART_Init();
*********************************************************************************************************
*/
void UART_Init(void)
{
    UART_GPIO_Configuration(); 	//USART1~5,GPIO;
		UART_NVIC_Configuration();	//USART1~5,NVIC;
	
		USART_Configuration02(USART3,9600);	 	// JG, syn6288,Yuyin;//语音模块;
    USART_Configuration02(UART5,19200);		// RS232-A, Init;video;//红外遥控器;
		
		USART_Configuration02(USART1,19200);	// RS485 Init;RS485;// 触摸屏通讯;
		RS_485_TX();		// 设置RS485-DIR发送状态

    USART_Configuration02(USART2,19200);	// JC, Init;LCD;//204A显示屏;
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	// 使能中断;204A;
	
		USART_Configuration02(UART4,115200);	// RS232-A, Init;Camera;//视觉识别相机;
		UART4Rx_DMA_Init(); 
		USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);
		USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);
		USART_Cmd( UART4, ENABLE );
}



/*
*********************************************************************************************************
* Function    ：void UART_GPIO_Configuration(void)
* Description : 所有串口端口初始化 ;
* Para        : none
* Return(s)   : none
* Example     : none
*********************************************************************************************************
*/
void UART_GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

		/*********USART TX**************/
		GPIO_InitStructure.GPIO_Pin = BSP_IO_TXD1|BSP_IO_TXD2;	                //USART1/USART2	//
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             // 复用推挽输出;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = BSP_IO_TXD3;		//USART3_TX   GPIOC.10
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	// 复用推挽输出;
		GPIO_Init(GPIOB, &GPIO_InitStructure);//GPIO

		GPIO_InitStructure.GPIO_Pin = BSP_IO_TXD4|BSP_IO_TXD5;	    //UART4/UART5  TX
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;            // 复用推挽输出;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);		

		/*********USART RX**************/
		GPIO_InitStructure.GPIO_Pin = BSP_IO_RXD1|BSP_IO_RXD2;                  // USART1/USART2 RX//
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;       // 浮空输入;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = BSP_IO_RXD3;	//USART3_RX	  
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;// 浮空输入;
		GPIO_Init(GPIOB, &GPIO_InitStructure);//GPIO	

		GPIO_InitStructure.GPIO_Pin = BSP_IO_RXD4;                 // UART4	RX
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;       // 浮空输入;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
								
		GPIO_InitStructure.GPIO_Pin = BSP_IO_RXD5;                  // UART5 RX
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;       // 浮空输入;
		GPIO_Init(GPIOD, &GPIO_InitStructure);		          
		/*********USART DIR**************/
		GPIO_InitStructure.GPIO_Pin = BSP_IO_DIR1;	                 // USART1, DIR
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;             // 推挽输出;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
						
}

/**********************************************************************************************************
* Function    ：void UART_NVIC_Configuration(void)
* Description : 所有串口中断 配置;
* Para        : none
* Return(s)   : none
* Example     : none
**********************************************************************************************************/
void UART_NVIC_Configuration(void)
{
			NVIC_InitTypeDef NVIC_InitStructure;
			//EXTI_InitTypeDef EXTI_InitStructure;
			/* Configure one bit for preemption priority */
			/* 优先级组 说明了抢占优先级所用的位数，和响应优先级所用的位数   在这里是1， 3 
			0组：  抢占优先级占0位， 响应优先级占4位
			1组：  抢占优先级占1位， 响应优先级占3位
			2组：  抢占优先级占2位， 响应优先级占2位
			3组：  抢占优先级占3位， 响应优先级占1位
			4组:   抢占优先级占4位， 响应优先级占0位  */
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

			/*********USART1,UART4,UART5 **********/
			NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			     //设置串口USART1中断
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	     //抢占优先级 1
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;               // 子优先级0
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);

			NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;			     //设置串口UART2中断
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	     //抢占优先级 1
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;               // 子优先级0
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);

			NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
			NVIC_Init( &NVIC_InitStructure );  
}


/**********************************************************************************************************
* Function    ：void USART_Configuration02(USART_TypeDef* USARTx, uint32_t bound);
* Description : 串口通信参数设置 02
* Para        : none
* Return(s)   : none
* Example     : void USART_Configuration02(USART_TypeDef* USARTx, uint32_t bound);
**********************************************************************************************************/
void USART_Configuration02(USART_TypeDef* USARTx, uint32_t bound)
{
    USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_BaudRate = bound;					//速率115200bps
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//数据位8位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;			//停止位1位
    USART_InitStructure.USART_Parity = USART_Parity_No;				//无校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //无硬件流控
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式

    /* Configure USART1 */
    USART_Init(USARTx, &USART_InitStructure);						//配置串口参数函数
 
    /* Enable USART1 Receive and Transmit interrupts */
    USART_ITConfig(USARTx, USART_IT_RXNE, DISABLE);     //禁止接收中断;
    USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);			//禁止发送缓冲空中断;
    //USART_ITConfig(USARTx, USART_IT_TC, ENABLE);			//使能发送完成中断
		//USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);			//使能空闲中断

    /* Enable the USART1 */
    USART_Cmd(USARTx, ENABLE);	
}

/**********************************************************************************************************
* Function    ：void BSP_DMA_Configuration(uint8_t UsartNum,uint8_t *TxbufferAddr,uint16_t Txnum)
* Description : 串口DMA发送配置 ;
* Para        : none
* Return(s)   : none
* Example     : DMA_USART_Configuration(1,&Txbuffer[1][0],TxCounter[1]); // usart2 tx dma
**********************************************************************************************************/
void BSP_DMA_Configuration(uint8_t UsartNum,uint8_t *TxbufferAddr,uint16_t Txnum)
{
    /*  USART1 TX---DMA1_Channel4
    *   USART2 TX---DMA1_Channel7
    *   USART3 TX---DMA1_Channel2
    *   UART4 TX---DMA2_Channel5
    */
    DMA_InitTypeDef DMA_InitStructure;

    uint32_t USART_DR_Base;
    DMA_Channel_TypeDef* DMA_Channel;
    USART_TypeDef* USARTx;
    switch (UsartNum)
    {
        case 0:     // usart1 TX DMA1
                    USARTx = USART1;
                    USART_DR_Base = USART1_DR_Base;
                    DMA_Channel =  DMA1_Channel4;
            break;
        case 1:     // usart2 TX DMA1
                    USARTx = USART2;
                    USART_DR_Base = USART2_DR_Base;
                    DMA_Channel =  DMA1_Channel7;
            break;
        case 2:     // usart3 TX DMA1
                    USARTx = USART3;
                    USART_DR_Base = USART3_DR_Base;
                    DMA_Channel =  DMA1_Channel2; 
            break;
        case 3:     // usart4 TX DMA2
                    USARTx = UART4;
                    USART_DR_Base = UART4_DR_Base;
                    DMA_Channel =  DMA2_Channel5; 
            break;
        default:
            break;
    }
    
    //DMA_Cmd(DMA_Channel, DISABLE);  			//关闭DMA Tx传输
    DMA_DeInit(DMA_Channel);							//DMA Channel---USART2 Tx
		DMA_InitStructure.DMA_PeripheralBaseAddr = USART_DR_Base;  				//USART2数据寄存器地址
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)TxbufferAddr;	 	//内存首地址 
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST ;								//配置DMA的传输方向(DMA--->USART)
		DMA_InitStructure.DMA_BufferSize = Txnum;						    			    //DMA缓冲区大小
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//外设地址自动++失能
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//内存地址自动++使能
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//外设数据单位为字节
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;						//内存数据单位为字节
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;											//模式为正常,单次传输
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;							//DMA优先级,中
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;											//DMA内存到内存关闭
		DMA_Init(DMA_Channel, &DMA_InitStructure);  											//设置DMA1通道

//		DMA_ClearFlag(DMA1_FLAG_GL2);
    USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);  //将USART模块设置成DMA方式工作    
//    DMA_Cmd(DMA_Channel, DISABLE);  			//关闭DMA Tx传输
		
		// 启动发送;
//		DMA_SetCurrDataCounter( DMA_Channel, Txnum );	// 设置要发送的字节长度;
    DMA_Cmd(DMA_Channel, ENABLE);  //开始一次DMA Tx传输
} 


/**********************************************************************************************************
* Function    ：void BSP_USART_OUT02(USART_TypeDef* USARTx, uint8_t *Data,uint16_t Len);
* Description : 串口数据发送函数;
* Para        : none
* Return(s)   : none
* Example     : void BSP_USART_OUT02(USART_TypeDef* USARTx, uint8_t *Data,uint16_t Len);
**********************************************************************************************************/
void BSP_USART_OUT02(USART_TypeDef* USARTx, uint8_t *Data,uint16_t Len)
{
	uint16_t i;
    if(USARTx == USART1)
    {
        BSP_DMA_Configuration(0,Data,Len);
        while(DMA_GetFlagStatus(DMA1_FLAG_TC4) == RESET); // 等待DMA传输完成
    }
    if(USARTx == USART2)
    {
        BSP_DMA_Configuration(1,Data,Len);
        while(DMA_GetFlagStatus(DMA1_FLAG_TC7) == RESET); // 等待DMA传输完成
    }
    if(USARTx == USART3)
    {
        BSP_DMA_Configuration(2,Data,Len);
        while(DMA_GetFlagStatus(DMA1_FLAG_TC2) == RESET); // 等待DMA传输完成
    }
    if(USARTx == UART4)
    {
        BSP_DMA_Configuration(3,Data,Len);
        while(DMA_GetFlagStatus(DMA2_FLAG_TC5) == RESET); // 等待DMA传输完成
    }
    if(USARTx == UART5)
    {
      for(i=0; i<Len; i++)
    	{
    		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);      //判断是否发送完成。
            USART_SendData(USARTx, Data[i]);	
            while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);      //判断是否发送完成。
    	}
    }
    else
    {
         delay_ms(2); // 延时2个字节发送时间，DMA要求
    }
}

/****************************************************************************
* 名    称：void USART_Send_Data02(uint8_t Channel,uint8_t *Dat,uint8_t Counter)
* 功    能：发送多个字节数据;
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：USART_Send_Data02(1,&TxBuffer[0][0],4); // USART2从TxBuffer[0][I]发送4个字节数据
****************************************************************************/ 
void USART_Send_Data02(uint8_t Channel,uint8_t *Dat,uint8_t Counter)
{
    switch (Channel)   //串口发送指定长字符串（数据）
    {
        case 0x00:  // 触摸屏通讯;
										RS_485_TX();	            // 设置RS485-1 发送状态
										BSP_USART_OUT02(USART1, Dat,Counter);
										RS_485_RX();	            // 设置RS485-1接收状态
            break;
        case 0x01:  
                    BSP_USART_OUT02(USART2, Dat,Counter);
            break;
        case 0x02:  
                    BSP_USART_OUT02(USART3, Dat,Counter);
            break;
        case 0x03: 
                    BSP_USART_OUT02(UART4, Dat,Counter);
            break;
        case 0x04: 
                    BSP_USART_OUT02(UART5, Dat,Counter);
            break;
        default:
            break;
    }       
}




/**********************************************************************************************************
* Function    ：uint8_t REC_CRC_CHECK(uint8_t *RxBuffer,uint8_t num)
* Description : 串口接受数据CRC校验
* Para        : *RxBuffer，num:MODBUS接收总数据长度，包括CRC校验值
* Return(s)   : 0：正确，1：错误
* Example     : REC_CRC_CHECK(RxBuffer1,uint8_t num);
**********************************************************************************************************/
uint8_t REC_CRC_CHECK(unsigned char *RxData,uint16_t num)
{
		uint16_t rec_CRC;  // 接收到的MODBUS校验值 
		uint16_t JI_CRC;   // 对MODBUS前面数据校验
		rec_CRC = (uint16_t)( RxData[num-2]|(RxData[num-1]<<8)  );  //  低八位、高八位
		JI_CRC = usMBCRC16( RxData, (num-2));
		if( rec_CRC != JI_CRC )
		{
				return 1;
		}
		else
		{
				return 0;
		}
}



