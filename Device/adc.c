/*
 * @file          adc.c
 * @brief         adc����
 * @ucos-ii       V2.86 
 * @function      DCP-28 
 * @compiler      RealView MDK-ARM Compiler V5.14
 * @peripheral    V3.5.0
 * @author  	  	
 * @date   	      2018/04/27
 * @corporation   �㽭���ǿƽ��豸���޹�˾
 * @website       http://www.zjqiushi.com/
*/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "adc.h"
#include "math.h"
#include "stdio.h"
#include "boardvariable.h"

/* Private variables ---------------------------------------------------------*/
#define ADC1_DR_Address    ((u32)0x4001244C)
ADC_InitTypeDef ADC_InitStructure;
DMA_InitTypeDef DMA_InitStructure;

//------------------ ADC-DMA ----------------------
vu16 ADCConvertedValue[3];				// �Զ��洢ADC��õ�����;

unsigned long int AD_Data1=0;
unsigned long int AD_Data2=0;
unsigned long int AD_Value1=0;
unsigned long int AD_Value2=0;

unsigned long int AD_ResultX=0;	// ҡ��ADC3;
unsigned long int AD_ResultY=0;	// ҡ��ADC2;

void GPIOC_Init(void);
void NVIC_InitADC(void);
void TIM8_Configuration(void);

// /*******************************************************************************
// * Function Name  : void delay02(u32 t)
// * Description    : ��ʱ����
// * Input          : None
// * Output         : None
// * Return         : delay02(u32 t)								   
// *******************************************************************************/
// void delay02(u32 t)
// {
// 	u32 i,j;
// 	for(i=0;i<t;i++)
// 		for(j=0;j<t;j++);
// }

/*******************************************************************************
* Function Name  : void GPIOC_Init(void)
* Description    : ADC����GPIO��ʼ��: PC0,PC1, ҡ�˵�λ��; PC3, ѹ��������; PC4,PC5, ���ݰ�;
* Input          : None
* Output         : None
* Return         : GPIOC_Init();										   
*******************************************************************************/
void GPIOC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	//PC0/1/3/4/5��Ϊģ��ͨ����������
	
	// JA: PC0,PC1, ҡ�˵�λ�� PC0,IN10=>����, PC1,IN11=>����;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1;	             
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
} 

/*******************************************************************************
* Function Name  : void TIM8_Configuration(void)
* Description    : ��ʱ��TIM8��ʼ��,����ADC_DMA��ʱ����
* Input          : None
* Output         : None
* Return         : TIM8_Configuration								   
*******************************************************************************/
void TIM8_Configuration(void)
{       
	 TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
	
	 TIM_TimeBaseStructure.TIM_Period=72-1;                   // �Զ���װ�ؼĴ�����ֵ     //72��Ƶ   
	 TIM_TimeBaseStructure.TIM_Prescaler=1000;                  // ʱ��Ԥ��Ƶ��        //�Զ���װ�Ĵ�������ֵ;0.312ms�ɼ�һ��
	 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;      // ������Ƶ        
	 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  //���ϼ���                    
	 TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	     
    TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);	 				//ʹ��ʱ��TIM8��ʱ�ж�Դ�ж�      
	 TIM_Cmd(TIM8,ENABLE);                              //��TIM8 
	 
	 //��������Ǹ߼���ʱ�����еģ����pwm�����         
	 TIM_CtrlPWMOutputs(TIM8, ENABLE);                                   //pwm���ʹ�ܣ�һ��Ҫ�ǵô� 
}
/*******************************************************************************
* Function Name  : void NVIC_InitADC(void)
* Description    : DMA1_CH1,TIM8�ж�����
* Input          : None
* Output         : None
* Return         : NVIC_InitADC();										   
*******************************************************************************/
void NVIC_InitADC(void)
{
	NVIC_InitTypeDef  NVIC_InitStructure;
    /* Configure one bit for preemption priority */
    /* ���ȼ��� ˵������ռ���ȼ����õ�λ��������Ӧ���ȼ����õ�λ��   ��������1�� 3 
    0�飺  ��ռ���ȼ�ռ0λ�� ��Ӧ���ȼ�ռ4λ
    1�飺  ��ռ���ȼ�ռ1λ�� ��Ӧ���ȼ�ռ3λ
    2�飺  ��ռ���ȼ�ռ2λ�� ��Ӧ���ȼ�ռ2λ
    3�飺  ��ռ���ȼ�ռ3λ�� ��Ӧ���ȼ�ռ1λ
    4��:   ��ռ���ȼ�ռ4λ�� ��Ӧ���ȼ�ռ0λ  */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);				 // ѡ��ʹ�����ȼ������1��

	NVIC_InitStructure.NVIC_IRQChannel =DMA1_Channel1_IRQn;;        //ADC1_1_IRQn;    //          
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	 // ָ����ռʽ���ȼ���1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQn;					 //��ʱ���ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		 //��ռ���ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				 //��ռ���ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	
}

/*******************************************************************************
* Function Name  : void ADC_Configuration(void)
* Description    : DMA,ADC��������
* Input          : None
* Output         : None
* Return         : ADC_Configuration();										   
*******************************************************************************/
void ADC_Configuration(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  
	
	NVIC_InitADC();		// DMA1_CH1,TIM2�ж�����;
	/* GPIO configuration ------------------------------------------------------*/
	GPIOC_Init();		// ADC����GPIO��ʼ��;
	TIM8_Configuration();		// ��ʱ��TIM2��ʼ��;
	/* DMA1 channel1 configuration ----------------------------------------------*/
	DMA_DeInit(DMA1_Channel1);

	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;			 //DMA��Ӧ���������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADCConvertedValue;		 //�ڴ�洢����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;					 //DMA��ת��ģʽΪSRCģʽ����������Ƶ��ڴ�
	DMA_InitStructure.DMA_BufferSize = 2;								 //DMA�����С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	 //����һ�����ݺ��豸��ַ��ֹ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;				 //����һ�����ݺ�Ŀ���ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	 //�����������ݿ��Ϊ16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			 //DMA�������ݳߴ磬HalfWord����Ϊ16λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;						   //ת��ģʽ��ѭ������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;					    //DMA���ȼ���
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;						   //M2Mģʽ����
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); 
	
	DMA_Cmd(DMA1_Channel1, ENABLE); // Enable DMA1 channel1 
	//DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);   		  //ʹ�ܴ�������ж� ,���жϷ�������ж�ȡת��ֵ
	
	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;				 //�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode =ENABLE;      	//ɨ��ģʽ,���ڶ�ͨ���ɼ�
	ADC_InitStructure.ADC_ContinuousConvMode =ENABLE;                 //DISABLE;	   //����ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv =ADC_ExternalTrigConv_None;               //�����ⲿ����ת���������������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;			//�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 2;							//ת��9��ͨ��
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/* ADC1 regular channel configuration */
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 					  //����adʱ��Ϊ72/6=12M,ad���ʱ��Ƶ��Ϊ14M
	
	//�趨ָ����ADCͨ����,����ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_239Cycles5);    //UB1   19.95us�ɼ�һ����;239.5����,239.5/12M=19.95us;
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 2, ADC_SampleTime_239Cycles5);    //UA1   //PC3,IN13  PC0,IN10=>����, PC1,IN11=>����;
	//ADC_Cmd(ADC1, ENABLE);				// ʹ��ADC;
	
	ADC_ResetCalibration(ADC1);//ʹ��ADC1��λУ׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));	//��ȡADC1����У׼�Ĵ�����״̬,����״̬��ȴ�
    ADC_StartCalibration(ADC1); //��ʼָ��ADC1��У׼״̬
	while(ADC_GetCalibrationStatus(ADC1));  //��ȡָ��ADC1��У׼����,����״̬��ȴ�   
	/* Start ADC1 Software Conversion */ 
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);		 //����û�в����ⲿ����������ʹ���������ADCת��
		
	DMA_ClearITPendingBit(DMA1_IT_TC1);  	// ���DMA1�жϴ������־λ;
	//DMA_Cmd(DMA1_Channel1, ENABLE); // Enable DMA1 channel1 
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);   		  //ʹ�ܴ�������ж� ,���жϷ�������ж�ȡת��ֵ
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);		 //����û�в����ⲿ����������ʹ���������ADCת��
	ADC_DMACmd(ADC1, DISABLE);	// ʹ��ADC��DMA1������;
	ADC_Cmd(ADC1, DISABLE);				// ��ֹʹ��ADC;
	
	
}

/*******************************************************************************
* Function Name  : TIM8_IRQHandler
* Description    : TIM8�жϺ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_UP_IRQHandler(void)	 								
{   		 
	if (TIM_GetITStatus(TIM8, TIM_IT_Update)!= RESET) 	// ����ж��Ƿ���;
	{
	  ADC_DMACmd(ADC1, ENABLE);	// ʹ��ADC��DMA1������;
	  ADC_Cmd(ADC1, ENABLE);		// ʹ��ADC;
	}
	TIM_ClearITPendingBit(TIM8, TIM_IT_Update);	// ����жϴ�����λ;
} 

/*******************************************************************************
* Function Name  : DMA1_Channel1_IRQHandler
* Description    : DMA1_CH1�жϺ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/							 
void  DMA1_Channel1_IRQHandler(void)  
{
	static  int m=0;
	
	if(DMA_GetITStatus(DMA1_IT_TC1)!=RESET)	// DMA1ͨ���Ƿ����ж�
  {  
		AD_Value1=(unsigned int)ADCConvertedValue[0];// �ⲿ�������ڴ洫���ݣ���ʱ��ȡ�ⲿ����;
		AD_Value2=(unsigned int)ADCConvertedValue[1];
	DMA_ClearITPendingBit(DMA1_IT_TC1);  	// ���DMA1�жϴ������־λ;
	ADC_DMACmd(ADC1, DISABLE);		// ��ֹʹ��ADC��DMA1������;
	ADC_Cmd(ADC1, DISABLE);				// ��ֹʹ��ADC;
		
		TIM_ITConfig(TIM8, TIM_IT_Update, DISABLE);	 	// ��ֹʹ��TIM3�ж�;
		TIM_Cmd(TIM8, DISABLE);		// ��ֹʹ��TIM3;
		
		AD_Data1 = AD_Data1+(AD_Value1&0x0FFF);
		AD_Data2 = AD_Data2+(AD_Value2&0x0FFF);
		m++;
		if(m>2)
		{
			m=0;
				AD_Data1 = AD_Data1/3;
				AD_ResultX=AD_Data1*1.0/409.6*33;
				AD_Data2 = AD_Data2/3;
				AD_ResultY=AD_Data2*1.0/409.6*33;
				AD_Data1 = 0;
				AD_Data2 = 0;
			
			if(YGHand_flag==1)//ҡ�˿������������;
			{
				if(AD_ResultX>270)	// ˳ʱ��,����; X�� ;
					YGHand_dirX=1;				// X�᷽��;//X=1,����;
				else if(AD_ResultX<50)	// ��ʱ��,����; X�� ;
					YGHand_dirX=2;				// X�᷽��;//X=2,����;
				else
					YGHand_dirX=0;				// X�᷽��;//X=0,��ֹ;
				
				if(AD_ResultY<50)	// ˳ʱ��,����; Y��;
					YGHand_dirY=1;				// Y�᷽��;//Y=1,����;
				else if(AD_ResultY>270)	//��ʱ��,����; Y��;
					YGHand_dirY=2;				// Y�᷽��;//Y=2,����;
				else
					YGHand_dirY=0;				// Y�᷽��;//Y=0,��ֹ;
			}
		}
		
		TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);	// ʹ��TIM3�ж�;
		TIM_Cmd(TIM8, ENABLE);		// ʹ��TIM3;
  }
}



