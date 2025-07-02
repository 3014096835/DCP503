/*
 * @file          Timer.c
 * @brief         ����:��ʱ��;PWM���;
 * @ucos-ii       V2.86 
 * @function      DCP-28 
 * @compiler      RealView MDK-ARM Compiler  V5.14
 * @peripheral    V3.5.0
 * @author  	  	
 * @date   	      2019/12/12
 * @corporation   �㽭���ǿƽ��豸���޹�˾
 * @website       http://www.zjqiushi.com/
*/
/* Includes ------------------------------------------------------------------*/
//#include  "stm32f10x.h"
//#include  "stm32f10x_conf.h" 
#include  "BoardVariable.h"

unsigned int	Timer2_Cnt1=0;
unsigned int	Timer3_Cnt1=0;


unsigned char TC1_SNumX=0;
unsigned char TC1_SNumY=0;
unsigned int TC1_MOTspeed=100;
unsigned int TC1_flagX=0;
unsigned int TC1_flagY=0;


/*******************************************************************************
* Function Name  : TIM2_Configuration
* Description    : ��ʱ��TIM2��ʼ��;
* Input          : None
* Output         : None
* Return         : None										   
*******************************************************************************/
void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    //EXTI_InitTypeDef EXTI_InitStructure;
    /* Configure one bit for preemption priority */
    /* ���ȼ��� ˵������ռ���ȼ����õ�λ��������Ӧ���ȼ����õ�λ��   ��������1�� 3 
    0�飺  ��ռ���ȼ�ռ0λ�� ��Ӧ���ȼ�ռ4λ
    1�飺  ��ռ���ȼ�ռ1λ�� ��Ӧ���ȼ�ռ3λ
    2�飺  ��ռ���ȼ�ռ2λ�� ��Ӧ���ȼ�ռ2λ
    3�飺  ��ռ���ȼ�ռ3λ�� ��Ӧ���ȼ�ռ1λ
    4��:   ��ռ���ȼ�ռ4λ�� ��Ӧ���ȼ�ռ0λ  */
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE );
	
	TIM_TimeBaseStructure.TIM_Prescaler = 720-1;        			//72��Ƶ
	TIM_TimeBaseStructure.TIM_Period = 100;//311; //155;  				//�Զ���װ�Ĵ�������ֵ;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);				 //��ʼ����ʱ��2
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;					 //��ʱ���ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		 //��ռ���ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				 //��ռ���ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	
	
    TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);	 				//ʹ��ʱ��TIM2��ʱ�ж�Դ�ж�
	TIM_Cmd(TIM2, ENABLE);											//ʹ�ܶ�ʱ��2
	
	Timer2_Cnt1=0;
}


/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : TIM2�жϺ���,Һ����ʾʱ���һ����˸;
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update)!= RESET) 	// ����ж��Ƿ���;
	{
			if((TC1_flagX==0) && (TC1_SNumX != 0) && (Run_Xmotor != Run_Xnum))
			{
				TC1_flagX=1;
				Timer2_Cnt1=1;
				TC1_MOTspeed=50;	TIM_SetAutoreload(TIM2,TC1_MOTspeed);	  // ��ʱ���Ƚ�ֵ;
			}
			
			if(Timer2_Cnt1==1)
			{
				Timer2_Cnt1=0;
				MOTX01_SetPul_HI;	// OUT1;
				
				if(YGHand_flag==1)
				{
					switch(YGHand_dirX)// X�᷽��;//X=0,��ֹ;//X=1,����;//X=2,����;
					{
						case 1:	Run_Xmotor++;	break;
						case 2:	Run_Xmotor--;	break;
						default:	break;
					}
				}
			}
			else
			{
				Timer2_Cnt1=1;
				
				if(Run_Xmotor != Run_Xnum)
				{
					if(Run_Xmotor > Run_Xnum)	MOT_DirX=1;
					else	MOT_DirX=2;
					if(MOT_DirX!=0)
					{
						if(MOT_DirX==1)
						{
								MOTX01_SetDir_HI;	MOTX01_SetEn_HI;	
								MOTX01_SetPul_LI;// X,����;
								Run_Xnum++;
						}
						if(MOT_DirX==2)
						{
								MOTX01_SetDir_LI;	MOTX01_SetEn_HI;	
								MOTX01_SetPul_LI;// X,ǰ��;
								//if(Run_Xnum>0)	
									Run_Xnum--;
						}
					}
					else
					{
							MOTX01_SetEn_HI;			
					}
				}
				else
				{
					MOT_DirX=0;
					
						TC1_SNumX=0;
						if(TC1_flagX==1)
						{
							TC1_flagX=0;
							if((CodeX.Error==NoErr)&&(CodeX.Compenflag==1))
							{
									CodeX.Compenflag=2;
							}
							else	CodeX.Compenflag=0;
						}
						else if(CodeX.Compenflag==1)
						{
							CodeX.Compenflag=2;
						}
				}
				
			}
			
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);	// ����жϴ�����λ;
} 

/*******************************************************************************
* Function Name  : TIM3_Configuration
* Description    : ��ʱ��TIM3��ʼ��;
* Input          : None
* Output         : None
* Return         : None										   
*******************************************************************************/
void TIM3_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    //EXTI_InitTypeDef EXTI_InitStructure;
    /* Configure one bit for preemption priority */
    /* ���ȼ��� ˵������ռ���ȼ����õ�λ��������Ӧ���ȼ����õ�λ��   ��������1�� 3 
    0�飺  ��ռ���ȼ�ռ0λ�� ��Ӧ���ȼ�ռ4λ
    1�飺  ��ռ���ȼ�ռ1λ�� ��Ӧ���ȼ�ռ3λ
    2�飺  ��ռ���ȼ�ռ2λ�� ��Ӧ���ȼ�ռ2λ
    3�飺  ��ռ���ȼ�ռ3λ�� ��Ӧ���ȼ�ռ1λ
    4��:   ��ռ���ȼ�ռ4λ�� ��Ӧ���ȼ�ռ0λ  */
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE );
	
	TIM_TimeBaseStructure.TIM_Prescaler = 720-1;        			//72��Ƶ
	TIM_TimeBaseStructure.TIM_Period = 100;//311; //155;  				//�Զ���װ�Ĵ�������ֵ;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);				 //��ʼ����ʱ��2
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;					 //��ʱ���ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		 //��ռ���ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				 //��ռ���ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	
	
    TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);	 				//ʹ��ʱ��TIM3��ʱ�ж�Դ�ж�
	TIM_Cmd(TIM3, ENABLE);											//ʹ�ܶ�ʱ��2
	
	Timer3_Cnt1=0;
}


/*******************************************************************************
* Function Name  : TIM3_IRQHandler
* Description    : TIM3�жϺ���,Һ����ʾʱ���һ����˸;
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM3_IRQHandler(void)
{	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update)!= RESET) 	// ����ж��Ƿ���;
	{
			if((TC1_flagY==0) && (TC1_SNumY != 0) && (Run_Ymotor != Run_Ynum))
			{
				TC1_flagY=1;
				Timer3_Cnt1=1;
				TC1_MOTspeed=50;	TIM_SetAutoreload(TIM3,TC1_MOTspeed);  // ��ʱ���Ƚ�ֵ;
			}
			
			if(Timer3_Cnt1==1)
			{
				Timer3_Cnt1=0;
				MOTY02_SetPul_HI;	// OUT1;
				if(YGHand_flag==1)
				{
					switch(YGHand_dirY)// Y�᷽��;//Y=0,��ֹ;//Y=1,����;//Y=2,����;
					{
						case 1:	Run_Ymotor++;	break;
						case 2:	Run_Ymotor--;	break;
						default:	break;
					}
				}
			}
			else
			{
				Timer3_Cnt1=1;
				
				if(Run_Ymotor != Run_Ynum)
				{
					if(Run_Ymotor > Run_Ynum)	MOT_DirY=1;
					else	MOT_DirY=2;
					if(MOT_DirY!=0)
					{
						if(MOT_DirY==1)
						{
								MOTY02_SetDir_HI;	MOTY02_SetEn_HI;	
								MOTY02_SetPul_LI;// X,����;
								Run_Ynum++;
						}
						if(MOT_DirY==2)
						{
								MOTY02_SetDir_LI;	MOTY02_SetEn_HI;	
								MOTY02_SetPul_LI;// X,ǰ��;
								//if(Run_Ynum>0)	
									Run_Ynum--;
						}
					}
					else
					{
							MOTY02_SetEn_HI;			
					}
				}
				else
				{
					MOT_DirY=0;	
					TC1_SNumY=0;
					if(TC1_flagY==1)
					{
						TC1_flagY=0;
						if((CodeY.Error==NoErr)&&(CodeY.Compenflag==1))
						{
							CodeY.Compenflag=2;
						}
						else CodeY.Compenflag=0;
					}
					else if(CodeY.Compenflag==1)
					{
						CodeY.Compenflag=2;
					}
				}
				
			}
			
	}
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);	// ����жϴ�����λ;
} 


/*******************************************************************************
* Function Name  : TIM7_Configuration
* Description    : ��ʱ��TIM7��ʼ��;1ms
* Input          : None
* Output         : None
* Return         : None										   
*******************************************************************************/
void TIM7_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    //EXTI_InitTypeDef EXTI_InitStructure;
    /* Configure one bit for preemption priority */
    /* ���ȼ��� ˵������ռ���ȼ����õ�λ��������Ӧ���ȼ����õ�λ��   ��������1�� 3 
    0�飺  ��ռ���ȼ�ռ0λ�� ��Ӧ���ȼ�ռ4λ
    1�飺  ��ռ���ȼ�ռ1λ�� ��Ӧ���ȼ�ռ3λ
    2�飺  ��ռ���ȼ�ռ2λ�� ��Ӧ���ȼ�ռ2λ
    3�飺  ��ռ���ȼ�ռ3λ�� ��Ӧ���ȼ�ռ1λ
    4��:   ��ռ���ȼ�ռ4λ�� ��Ӧ���ȼ�ռ0λ  */
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE );
	
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;        			//72��Ƶ
	TIM_TimeBaseStructure.TIM_Period = 1000-1;//311; //155;  				//�Զ���װ�Ĵ�������ֵ;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���ģʽ
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);				 //��ʼ����ʱ��2
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;					 //��ʱ���ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;		 //��ռ���ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;				 //��ռ���ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	
	
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);	 				//ʹ��ʱ��TIM3��ʱ�ж�Դ�ж�
	TIM_Cmd(TIM7, ENABLE);											//ʹ�ܶ�ʱ��2
}


void  TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)!=RESET)
	{
		if(System_flag)
		{
			if(System_TimeCnt++>=1000)	//1.0s�������־
			{
				// ϵͳ��ʱ;
					System_TimeCnt=0;
					System_Timer++;
				SysTime_Hour = System_Timer/3600;
				SysTime_Minute = (System_Timer%3600)/60;
				SysTime_Second = (System_Timer%3600)%60;
				
				
					/* ϵͳ״ָ̬ʾ��ÿ����һ��*/
					BSP_LED_Toggle(2);   // ��ת LED2	 

					//  ��Ƶʱ�� 1s ��ʱ;
					if(Video_timeflag==1)
					{
						Video_timesec++;
					}
					else Video_timesec=0;
					
					//  �Զ�����ʱ�� 1s ��ʱ;
					if(Auto_RunTimeflag==1)
					{
						Auto_RunTimeflag=2;
						Auto_RunTimesec=0;
						Auto_RunTimemisec=0;
					}
					else if(Auto_RunTimeflag==2)
					{
						Auto_RunTimesec++;
					}
					
					// �Զ�����ʱ�� 1s ��ʱ;
					if(Loop_Runing==1)	Loop_RunHoldtime++;	//�Զ�����,��ʱ;
					
					
					if(LCD_Refreshflag==1)
					{
						LCD_Refreshflag=2;
						LCD_Refreshsec=0;
					}
					else if(LCD_Refreshflag==2)
					{
						LCD_Refreshsec++;
						if(LCD_Refreshsec>200)LCD_Refreshflag=0;
					}
					else LCD_Refreshflag=0;
					
					USB_BeatNUM++;
					if(USB_BeatNUM >= 6)
					{
						USB_BeatNUM = 0;
						Camera_TxRefresh(1);	// USB���Ӿ������������;
					}
			}
			W5500_Send_Delay_Counter++; 
			
			//  �Զ�����ʱ�� 1ms ��ʱ;
			if(Auto_RunTimeflag==1)
			{
				Auto_RunTimeflag=2;
				Auto_RunTimesec=0;
				Auto_RunTimemisec=0;
			}
			else if(Auto_RunTimeflag==2)
			{
				Auto_RunTimemisec++;
			}
			
			//  ��Ƶʱ�� 1ms ��ʱ;
			if(Video_timeflag==1)
			{
				Video_timemisec++;
			}
			else Video_timemisec=0;
				
			//  �����ʱ�� 1ms ��ʱ;
			if(Laser_OpenFlag==1)
			{
				Laser_OpenTimeout++;
			}
			
//			//  �Զ���������ʱ�� 1ms ��ʱ;
//			if(StartNext_flag==1)
//			{
//				StartNext_Timeout++;
//			}
//			else	StartNext_Timeout=0;
			
			//////////////////// ���������в��� =1����һ��ʱ��1s; =3����һ��ʱ��0.5s; =5��������0.5s; ///////////////////
			if(BUZZER_flag==1)
			{
				BUZZER_Count=0;	BUZZER_flag=2;	BUZZER_On;	//����������;//=1����һ��ʱ��1s;
			}
			else if(BUZZER_flag==2)
			{
				BUZZER_Count++;
				if(BUZZER_Count<1000)	{	BUZZER_On;	}//����������;
				else
				{
					BUZZER_Count=0;	BUZZER_flag=0;	BUZZER_Off;	//������ֹͣ;
				}
			}
			else if(BUZZER_flag==3)
			{
				BUZZER_Count=0;	BUZZER_flag=4;	BUZZER_On;	//����������;//=3����һ��ʱ��0.5s;
			}
			else if(BUZZER_flag==4)
			{
				BUZZER_Count++;
				if(BUZZER_Count<500)	{	BUZZER_On;	}//����������;
				else
				{
					BUZZER_Count=0;	BUZZER_flag=0;	BUZZER_Off;	//������ֹͣ;
				}
			}
			else if(BUZZER_flag==5)
			{
				BUZZER_Count=0;	BUZZER_flag=6;	BUZZER_On;	//����������;//=5��������0.5s;
			}
			else if(BUZZER_flag==6)
			{
				BUZZER_Count++;
				if(BUZZER_Count<500)	{	BUZZER_On;	}//����������;	
				else if(BUZZER_Count<1000)	{	BUZZER_Off;	}//������ֹͣ;
				else if(BUZZER_Count<1500)	{	BUZZER_On;	}//����������;
				else
				{
					BUZZER_Count=0;	BUZZER_flag=0;	BUZZER_Off;	//������ֹͣ;
				}
			}
			//////////////////// ���������в��� ,////////////////////////

				// MCGS RS485;��ʱ����һ֡���ݵĽ���;
				Rece_Time++;
				if(Rece_Time>40)
				{
					 Rece_TimeOut = 0 ;    //�ѳ�ʱ���Ͳ��ü���

					 if(Rece_bit == 1)     // ���һ���ֽڣ�t3.5������
					 {
							Rece_bit = 0 ;
							USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);    // ��ֹ�����ж�
					 }
				}
		}
		
		TIM_ClearITPendingBit(TIM7,TIM_IT_Update);
	}
}

/*************************************** ���� ********************************************************/
