/*
 * @file          adc.c
 * @brief         adc程序
 * @ucos-ii       V2.86 
 * @function      DCP-28 
 * @compiler      RealView MDK-ARM Compiler V5.14
 * @peripheral    V3.5.0
 * @author  	  	
 * @date   	      2018/04/27
 * @corporation   浙江求是科教设备有限公司
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
vu16 ADCConvertedValue[3];				// 自动存储ADC获得的数据;

unsigned long int AD_Data1=0;
unsigned long int AD_Data2=0;
unsigned long int AD_Value1=0;
unsigned long int AD_Value2=0;

unsigned long int AD_ResultX=0;	// 摇杆ADC3;
unsigned long int AD_ResultY=0;	// 摇杆ADC2;

void GPIOC_Init(void);
void NVIC_InitADC(void);
void TIM8_Configuration(void);

// /*******************************************************************************
// * Function Name  : void delay02(u32 t)
// * Description    : 延时程序
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
* Description    : ADC输入GPIO初始化: PC0,PC1, 摇杆电位器; PC3, 压力传感器; PC4,PC5, 电容板;
* Input          : None
* Output         : None
* Return         : GPIOC_Init();										   
*******************************************************************************/
void GPIOC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	//PC0/1/3/4/5作为模拟通道输入引脚
	
	// JA: PC0,PC1, 摇杆电位器 PC0,IN10=>上下, PC1,IN11=>左右;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1;	             
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
} 

/*******************************************************************************
* Function Name  : void TIM8_Configuration(void)
* Description    : 定时器TIM8初始化,用于ADC_DMA定时触发
* Input          : None
* Output         : None
* Return         : TIM8_Configuration								   
*******************************************************************************/
void TIM8_Configuration(void)
{       
	 TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
	
	 TIM_TimeBaseStructure.TIM_Period=72-1;                   // 自动重装载寄存器的值     //72分频   
	 TIM_TimeBaseStructure.TIM_Prescaler=1000;                  // 时钟预分频数        //自动重装寄存器周期值;0.312ms采集一次
	 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;      // 采样分频        
	 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  //向上计数                    
	 TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	     
    TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);	 				//使能时钟TIM8定时中断源中断      
	 TIM_Cmd(TIM8,ENABLE);                              //打开TIM8 
	 
	 //下面这句是高级定时器才有的，输出pwm必须打开         
	 TIM_CtrlPWMOutputs(TIM8, ENABLE);                                   //pwm输出使能，一定要记得打开 
}
/*******************************************************************************
* Function Name  : void NVIC_InitADC(void)
* Description    : DMA1_CH1,TIM8中断设置
* Input          : None
* Output         : None
* Return         : NVIC_InitADC();										   
*******************************************************************************/
void NVIC_InitADC(void)
{
	NVIC_InitTypeDef  NVIC_InitStructure;
    /* Configure one bit for preemption priority */
    /* 优先级组 说明了抢占优先级所用的位数，和响应优先级所用的位数   在这里是1， 3 
    0组：  抢占优先级占0位， 响应优先级占4位
    1组：  抢占优先级占1位， 响应优先级占3位
    2组：  抢占优先级占2位， 响应优先级占2位
    3组：  抢占优先级占3位， 响应优先级占1位
    4组:   抢占优先级占4位， 响应优先级占0位  */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);				 // 选择使用优先级分组第1组

	NVIC_InitStructure.NVIC_IRQChannel =DMA1_Channel1_IRQn;;        //ADC1_1_IRQn;    //          
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	 // 指定抢占式优先级别1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQn;					 //定时器中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		 //先占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				 //从占优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	
}

/*******************************************************************************
* Function Name  : void ADC_Configuration(void)
* Description    : DMA,ADC参数设置
* Input          : None
* Output         : None
* Return         : ADC_Configuration();										   
*******************************************************************************/
void ADC_Configuration(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  
	
	NVIC_InitADC();		// DMA1_CH1,TIM2中断设置;
	/* GPIO configuration ------------------------------------------------------*/
	GPIOC_Init();		// ADC输入GPIO初始化;
	TIM8_Configuration();		// 定时器TIM2初始化;
	/* DMA1 channel1 configuration ----------------------------------------------*/
	DMA_DeInit(DMA1_Channel1);

	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;			 //DMA对应的外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADCConvertedValue;		 //内存存储基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;					 //DMA的转换模式为SRC模式，由外设搬移到内存
	DMA_InitStructure.DMA_BufferSize = 2;								 //DMA缓存大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	 //接收一次数据后，设备地址禁止后移
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;				 //接收一次数据后，目标内存地址后移
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	 //定义外设数据宽度为16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			 //DMA搬移数据尺寸，HalfWord就是为16位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;						   //转换模式，循环缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;					    //DMA优先级高
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;						   //M2M模式禁用
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); 
	
	DMA_Cmd(DMA1_Channel1, ENABLE); // Enable DMA1 channel1 
	//DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);   		  //使能传输完成中断 ,在中断服务程序中读取转换值
	
	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;				 //工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode =ENABLE;      	//扫描模式,用于多通道采集
	ADC_InitStructure.ADC_ContinuousConvMode =ENABLE;                 //DISABLE;	   //连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv =ADC_ExternalTrigConv_None;               //不用外部触发转换，软件开启即可
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;			//数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 2;							//转换9个通道
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/* ADC1 regular channel configuration */
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 					  //设置ad时钟为72/6=12M,ad最大时钟频率为14M
	
	//设定指定的ADC通道组,设置转化顺序和采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_239Cycles5);    //UB1   19.95us采集一个点;239.5周期,239.5/12M=19.95us;
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 2, ADC_SampleTime_239Cycles5);    //UA1   //PC3,IN13  PC0,IN10=>上下, PC1,IN11=>左右;
	//ADC_Cmd(ADC1, ENABLE);				// 使能ADC;
	
	ADC_ResetCalibration(ADC1);//使能ADC1复位校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1));	//获取ADC1重置校准寄存器的状态,设置状态则等待
    ADC_StartCalibration(ADC1); //开始指定ADC1的校准状态
	while(ADC_GetCalibrationStatus(ADC1));  //获取指定ADC1的校准程序,设置状态则等待   
	/* Start ADC1 Software Conversion */ 
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);		 //由于没有采用外部触发，所以使用软件触发ADC转换
		
	DMA_ClearITPendingBit(DMA1_IT_TC1);  	// 清除DMA1中断待处理标志位;
	//DMA_Cmd(DMA1_Channel1, ENABLE); // Enable DMA1 channel1 
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);   		  //使能传输完成中断 ,在中断服务程序中读取转换值
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);		 //由于没有采用外部触发，所以使用软件触发ADC转换
	ADC_DMACmd(ADC1, DISABLE);	// 使能ADC的DMA1的请求;
	ADC_Cmd(ADC1, DISABLE);				// 禁止使能ADC;
	
	
}

/*******************************************************************************
* Function Name  : TIM8_IRQHandler
* Description    : TIM8中断函数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_UP_IRQHandler(void)	 								
{   		 
	if (TIM_GetITStatus(TIM8, TIM_IT_Update)!= RESET) 	// 检测中断是否发生;
	{
	  ADC_DMACmd(ADC1, ENABLE);	// 使能ADC的DMA1的请求;
	  ADC_Cmd(ADC1, ENABLE);		// 使能ADC;
	}
	TIM_ClearITPendingBit(TIM8, TIM_IT_Update);	// 清除中断待处理位;
} 

/*******************************************************************************
* Function Name  : DMA1_Channel1_IRQHandler
* Description    : DMA1_CH1中断函数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/							 
void  DMA1_Channel1_IRQHandler(void)  
{
	static  int m=0;
	
	if(DMA_GetITStatus(DMA1_IT_TC1)!=RESET)	// DMA1通道是否发生中断
  {  
		AD_Value1=(unsigned int)ADCConvertedValue[0];// 外部数据向内存传数据，此时读取外部数据;
		AD_Value2=(unsigned int)ADCConvertedValue[1];
	DMA_ClearITPendingBit(DMA1_IT_TC1);  	// 清除DMA1中断待处理标志位;
	ADC_DMACmd(ADC1, DISABLE);		// 禁止使能ADC的DMA1的请求;
	ADC_Cmd(ADC1, DISABLE);				// 禁止使能ADC;
		
		TIM_ITConfig(TIM8, TIM_IT_Update, DISABLE);	 	// 禁止使能TIM3中断;
		TIM_Cmd(TIM8, DISABLE);		// 禁止使能TIM3;
		
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
			
			if(YGHand_flag==1)//摇杆控制启动情况下;
			{
				if(AD_ResultX>270)	// 顺时针,右移; X轴 ;
					YGHand_dirX=1;				// X轴方向;//X=1,向右;
				else if(AD_ResultX<50)	// 逆时针,左移; X轴 ;
					YGHand_dirX=2;				// X轴方向;//X=2,向左;
				else
					YGHand_dirX=0;				// X轴方向;//X=0,静止;
				
				if(AD_ResultY<50)	// 顺时针,往上; Y轴;
					YGHand_dirY=1;				// Y轴方向;//Y=1,向上;
				else if(AD_ResultY>270)	//逆时针,往下; Y轴;
					YGHand_dirY=2;				// Y轴方向;//Y=2,向下;
				else
					YGHand_dirY=0;				// Y轴方向;//Y=0,静止;
			}
		}
		
		TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);	// 使能TIM3中断;
		TIM_Cmd(TIM8, ENABLE);		// 使能TIM3;
  }
}



