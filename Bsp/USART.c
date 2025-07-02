/*
 * @file          UART.c
 * @brief         ����: ����ͨѶ-����-���ֺ���;
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
#include  "stm32f10x.h"
#include  "stm32f10x_conf.h" 
#include 	"delay.h"
#include  "USART.h"
#include  "mbcrc.h"


/*
*********************************************************************************************************
* Function    ��void UART_Init(void)
* Description : ���д���ģ���ʼ�� 
* Para        : none
* Return(s)   : none
* Example     : UART_Init();
*********************************************************************************************************
*/
void UART_Init(void)
{
    UART_GPIO_Configuration(); 	//USART1~5,GPIO;
		UART_NVIC_Configuration();	//USART1~5,NVIC;
	
		USART_Configuration02(USART3,9600);	 	// JG, syn6288,Yuyin;//����ģ��;
    USART_Configuration02(UART5,19200);		// RS232-A, Init;video;//����ң����;
		
		USART_Configuration02(USART1,19200);	// RS485 Init;RS485;// ������ͨѶ;
		RS_485_TX();		// ����RS485-DIR����״̬

    USART_Configuration02(USART2,19200);	// JC, Init;LCD;//204A��ʾ��;
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	// ʹ���ж�;204A;
	
		USART_Configuration02(UART4,115200);	// RS232-A, Init;Camera;//�Ӿ�ʶ�����;
		UART4Rx_DMA_Init(); 
		USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);
		USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);
		USART_Cmd( UART4, ENABLE );
}



/*
*********************************************************************************************************
* Function    ��void UART_GPIO_Configuration(void)
* Description : ���д��ڶ˿ڳ�ʼ�� ;
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
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             // �����������;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = BSP_IO_TXD3;		//USART3_TX   GPIOC.10
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	// �����������;
		GPIO_Init(GPIOB, &GPIO_InitStructure);//GPIO

		GPIO_InitStructure.GPIO_Pin = BSP_IO_TXD4|BSP_IO_TXD5;	    //UART4/UART5  TX
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;            // �����������;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);		

		/*********USART RX**************/
		GPIO_InitStructure.GPIO_Pin = BSP_IO_RXD1|BSP_IO_RXD2;                  // USART1/USART2 RX//
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;       // ��������;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = BSP_IO_RXD3;	//USART3_RX	  
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;// ��������;
		GPIO_Init(GPIOB, &GPIO_InitStructure);//GPIO	

		GPIO_InitStructure.GPIO_Pin = BSP_IO_RXD4;                 // UART4	RX
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;       // ��������;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
								
		GPIO_InitStructure.GPIO_Pin = BSP_IO_RXD5;                  // UART5 RX
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;       // ��������;
		GPIO_Init(GPIOD, &GPIO_InitStructure);		          
		/*********USART DIR**************/
		GPIO_InitStructure.GPIO_Pin = BSP_IO_DIR1;	                 // USART1, DIR
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;             // �������;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
						
}

/**********************************************************************************************************
* Function    ��void UART_NVIC_Configuration(void)
* Description : ���д����ж� ����;
* Para        : none
* Return(s)   : none
* Example     : none
**********************************************************************************************************/
void UART_NVIC_Configuration(void)
{
			NVIC_InitTypeDef NVIC_InitStructure;
			//EXTI_InitTypeDef EXTI_InitStructure;
			/* Configure one bit for preemption priority */
			/* ���ȼ��� ˵������ռ���ȼ����õ�λ��������Ӧ���ȼ����õ�λ��   ��������1�� 3 
			0�飺  ��ռ���ȼ�ռ0λ�� ��Ӧ���ȼ�ռ4λ
			1�飺  ��ռ���ȼ�ռ1λ�� ��Ӧ���ȼ�ռ3λ
			2�飺  ��ռ���ȼ�ռ2λ�� ��Ӧ���ȼ�ռ2λ
			3�飺  ��ռ���ȼ�ռ3λ�� ��Ӧ���ȼ�ռ1λ
			4��:   ��ռ���ȼ�ռ4λ�� ��Ӧ���ȼ�ռ0λ  */
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

			/*********USART1,UART4,UART5 **********/
			NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			     //���ô���USART1�ж�
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	     //��ռ���ȼ� 1
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;               // �����ȼ�0
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);

			NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;			     //���ô���UART2�ж�
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	     //��ռ���ȼ� 1
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;               // �����ȼ�0
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);

			NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
			NVIC_Init( &NVIC_InitStructure );  
}


/**********************************************************************************************************
* Function    ��void USART_Configuration02(USART_TypeDef* USARTx, uint32_t bound);
* Description : ����ͨ�Ų������� 02
* Para        : none
* Return(s)   : none
* Example     : void USART_Configuration02(USART_TypeDef* USARTx, uint32_t bound);
**********************************************************************************************************/
void USART_Configuration02(USART_TypeDef* USARTx, uint32_t bound)
{
    USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_BaudRate = bound;					//����115200bps
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//����λ8λ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;			//ֹͣλ1λ
    USART_InitStructure.USART_Parity = USART_Parity_No;				//��У��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //��Ӳ������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ

    /* Configure USART1 */
    USART_Init(USARTx, &USART_InitStructure);						//���ô��ڲ�������
 
    /* Enable USART1 Receive and Transmit interrupts */
    USART_ITConfig(USARTx, USART_IT_RXNE, DISABLE);     //��ֹ�����ж�;
    USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);			//��ֹ���ͻ�����ж�;
    //USART_ITConfig(USARTx, USART_IT_TC, ENABLE);			//ʹ�ܷ�������ж�
		//USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);			//ʹ�ܿ����ж�

    /* Enable the USART1 */
    USART_Cmd(USARTx, ENABLE);	
}

/**********************************************************************************************************
* Function    ��void BSP_DMA_Configuration(uint8_t UsartNum,uint8_t *TxbufferAddr,uint16_t Txnum)
* Description : ����DMA�������� ;
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
    
    //DMA_Cmd(DMA_Channel, DISABLE);  			//�ر�DMA Tx����
    DMA_DeInit(DMA_Channel);							//DMA Channel---USART2 Tx
		DMA_InitStructure.DMA_PeripheralBaseAddr = USART_DR_Base;  				//USART2���ݼĴ�����ַ
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)TxbufferAddr;	 	//�ڴ��׵�ַ 
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST ;								//����DMA�Ĵ��䷽��(DMA--->USART)
		DMA_InitStructure.DMA_BufferSize = Txnum;						    			    //DMA��������С
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//�����ַ�Զ�++ʧ��
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//�ڴ��ַ�Զ�++ʹ��
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//�������ݵ�λΪ�ֽ�
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;						//�ڴ����ݵ�λΪ�ֽ�
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;											//ģʽΪ����,���δ���
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;							//DMA���ȼ�,��
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;											//DMA�ڴ浽�ڴ�ر�
		DMA_Init(DMA_Channel, &DMA_InitStructure);  											//����DMA1ͨ��

//		DMA_ClearFlag(DMA1_FLAG_GL2);
    USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);  //��USARTģ�����ó�DMA��ʽ����    
//    DMA_Cmd(DMA_Channel, DISABLE);  			//�ر�DMA Tx����
		
		// ��������;
//		DMA_SetCurrDataCounter( DMA_Channel, Txnum );	// ����Ҫ���͵��ֽڳ���;
    DMA_Cmd(DMA_Channel, ENABLE);  //��ʼһ��DMA Tx����
} 


/**********************************************************************************************************
* Function    ��void BSP_USART_OUT02(USART_TypeDef* USARTx, uint8_t *Data,uint16_t Len);
* Description : �������ݷ��ͺ���;
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
        while(DMA_GetFlagStatus(DMA1_FLAG_TC4) == RESET); // �ȴ�DMA�������
    }
    if(USARTx == USART2)
    {
        BSP_DMA_Configuration(1,Data,Len);
        while(DMA_GetFlagStatus(DMA1_FLAG_TC7) == RESET); // �ȴ�DMA�������
    }
    if(USARTx == USART3)
    {
        BSP_DMA_Configuration(2,Data,Len);
        while(DMA_GetFlagStatus(DMA1_FLAG_TC2) == RESET); // �ȴ�DMA�������
    }
    if(USARTx == UART4)
    {
        BSP_DMA_Configuration(3,Data,Len);
        while(DMA_GetFlagStatus(DMA2_FLAG_TC5) == RESET); // �ȴ�DMA�������
    }
    if(USARTx == UART5)
    {
      for(i=0; i<Len; i++)
    	{
    		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);      //�ж��Ƿ�����ɡ�
            USART_SendData(USARTx, Data[i]);	
            while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);      //�ж��Ƿ�����ɡ�
    	}
    }
    else
    {
         delay_ms(2); // ��ʱ2���ֽڷ���ʱ�䣬DMAҪ��
    }
}

/****************************************************************************
* ��    �ƣ�void USART_Send_Data02(uint8_t Channel,uint8_t *Dat,uint8_t Counter)
* ��    �ܣ����Ͷ���ֽ�����;
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����USART_Send_Data02(1,&TxBuffer[0][0],4); // USART2��TxBuffer[0][I]����4���ֽ�����
****************************************************************************/ 
void USART_Send_Data02(uint8_t Channel,uint8_t *Dat,uint8_t Counter)
{
    switch (Channel)   //���ڷ���ָ�����ַ��������ݣ�
    {
        case 0x00:  // ������ͨѶ;
										RS_485_TX();	            // ����RS485-1 ����״̬
										BSP_USART_OUT02(USART1, Dat,Counter);
										RS_485_RX();	            // ����RS485-1����״̬
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
* Function    ��uint8_t REC_CRC_CHECK(uint8_t *RxBuffer,uint8_t num)
* Description : ���ڽ�������CRCУ��
* Para        : *RxBuffer��num:MODBUS���������ݳ��ȣ�����CRCУ��ֵ
* Return(s)   : 0����ȷ��1������
* Example     : REC_CRC_CHECK(RxBuffer1,uint8_t num);
**********************************************************************************************************/
uint8_t REC_CRC_CHECK(unsigned char *RxData,uint16_t num)
{
		uint16_t rec_CRC;  // ���յ���MODBUSУ��ֵ 
		uint16_t JI_CRC;   // ��MODBUSǰ������У��
		rec_CRC = (uint16_t)( RxData[num-2]|(RxData[num-1]<<8)  );  //  �Ͱ�λ���߰�λ
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



