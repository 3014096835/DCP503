/*
 * @file          Timer.c
 * @brief         程序:定时器;PWM输出;
 * @ucos-ii       V2.86 
 * @function      DCP-28 
 * @compiler      RealView MDK-ARM Compiler  V5.14
 * @peripheral    V3.5.0
 * @author  	  	
 * @date   	      2019/12/12
 * @corporation   浙江求是科教设备有限公司
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
* Description    : 定时器TIM2初始化;
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
    /* 优先级组 说明了抢占优先级所用的位数，和响应优先级所用的位数   在这里是1， 3 
    0组：  抢占优先级占0位， 响应优先级占4位
    1组：  抢占优先级占1位， 响应优先级占3位
    2组：  抢占优先级占2位， 响应优先级占2位
    3组：  抢占优先级占3位， 响应优先级占1位
    4组:   抢占优先级占4位， 响应优先级占0位  */
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE );
	
	TIM_TimeBaseStructure.TIM_Prescaler = 720-1;        			//72分频
	TIM_TimeBaseStructure.TIM_Period = 100;//311; //155;  				//自动重装寄存器周期值;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);				 //初始化定时器2
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;					 //定时器中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		 //先占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				 //从占优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	
	
    TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);	 				//使能时钟TIM2定时中断源中断
	TIM_Cmd(TIM2, ENABLE);											//使能定时器2
	
	Timer2_Cnt1=0;
}


/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : TIM2中断函数,液晶显示时间隔一秒闪烁;
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update)!= RESET) 	// 检测中断是否发生;
	{
			if((TC1_flagX==0) && (TC1_SNumX != 0) && (Run_Xmotor != Run_Xnum))
			{
				TC1_flagX=1;
				Timer2_Cnt1=1;
				TC1_MOTspeed=50;	TIM_SetAutoreload(TIM2,TC1_MOTspeed);	  // 定时器比较值;
			}
			
			if(Timer2_Cnt1==1)
			{
				Timer2_Cnt1=0;
				MOTX01_SetPul_HI;	// OUT1;
				
				if(YGHand_flag==1)
				{
					switch(YGHand_dirX)// X轴方向;//X=0,静止;//X=1,向右;//X=2,向左;
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
								MOTX01_SetPul_LI;// X,后退;
								Run_Xnum++;
						}
						if(MOT_DirX==2)
						{
								MOTX01_SetDir_LI;	MOTX01_SetEn_HI;	
								MOTX01_SetPul_LI;// X,前进;
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
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);	// 清除中断待处理位;
} 

/*******************************************************************************
* Function Name  : TIM3_Configuration
* Description    : 定时器TIM3初始化;
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
    /* 优先级组 说明了抢占优先级所用的位数，和响应优先级所用的位数   在这里是1， 3 
    0组：  抢占优先级占0位， 响应优先级占4位
    1组：  抢占优先级占1位， 响应优先级占3位
    2组：  抢占优先级占2位， 响应优先级占2位
    3组：  抢占优先级占3位， 响应优先级占1位
    4组:   抢占优先级占4位， 响应优先级占0位  */
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE );
	
	TIM_TimeBaseStructure.TIM_Prescaler = 720-1;        			//72分频
	TIM_TimeBaseStructure.TIM_Period = 100;//311; //155;  				//自动重装寄存器周期值;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);				 //初始化定时器2
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;					 //定时器中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		 //先占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				 //从占优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	
	
    TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);	 				//使能时钟TIM3定时中断源中断
	TIM_Cmd(TIM3, ENABLE);											//使能定时器2
	
	Timer3_Cnt1=0;
}


/*******************************************************************************
* Function Name  : TIM3_IRQHandler
* Description    : TIM3中断函数,液晶显示时间隔一秒闪烁;
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM3_IRQHandler(void)
{	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update)!= RESET) 	// 检测中断是否发生;
	{
			if((TC1_flagY==0) && (TC1_SNumY != 0) && (Run_Ymotor != Run_Ynum))
			{
				TC1_flagY=1;
				Timer3_Cnt1=1;
				TC1_MOTspeed=50;	TIM_SetAutoreload(TIM3,TC1_MOTspeed);  // 定时器比较值;
			}
			
			if(Timer3_Cnt1==1)
			{
				Timer3_Cnt1=0;
				MOTY02_SetPul_HI;	// OUT1;
				if(YGHand_flag==1)
				{
					switch(YGHand_dirY)// Y轴方向;//Y=0,静止;//Y=1,向上;//Y=2,向下;
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
								MOTY02_SetPul_LI;// X,后退;
								Run_Ynum++;
						}
						if(MOT_DirY==2)
						{
								MOTY02_SetDir_LI;	MOTY02_SetEn_HI;	
								MOTY02_SetPul_LI;// X,前进;
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
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);	// 清除中断待处理位;
} 


/*******************************************************************************
* Function Name  : TIM7_Configuration
* Description    : 定时器TIM7初始化;1ms
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
    /* 优先级组 说明了抢占优先级所用的位数，和响应优先级所用的位数   在这里是1， 3 
    0组：  抢占优先级占0位， 响应优先级占4位
    1组：  抢占优先级占1位， 响应优先级占3位
    2组：  抢占优先级占2位， 响应优先级占2位
    3组：  抢占优先级占3位， 响应优先级占1位
    4组:   抢占优先级占4位， 响应优先级占0位  */
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE );
	
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;        			//72分频
	TIM_TimeBaseStructure.TIM_Period = 1000-1;//311; //155;  				//自动重装寄存器周期值;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数模式
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);				 //初始化定时器2
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;					 //定时器中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;		 //先占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;				 //从占优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	
	
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);	 				//使能时钟TIM3定时中断源中断
	TIM_Cmd(TIM7, ENABLE);											//使能定时器2
}


void  TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)!=RESET)
	{
		if(System_flag)
		{
			if(System_TimeCnt++>=1000)	//1.0s秒溢出标志
			{
				// 系统计时;
					System_TimeCnt=0;
					System_Timer++;
				SysTime_Hour = System_Timer/3600;
				SysTime_Minute = (System_Timer%3600)/60;
				SysTime_Second = (System_Timer%3600)%60;
				
				
					/* 系统状态指示：每秒闪一次*/
					BSP_LED_Toggle(2);   // 翻转 LED2	 

					//  视频时间 1s 计时;
					if(Video_timeflag==1)
					{
						Video_timesec++;
					}
					else Video_timesec=0;
					
					//  自动运行时间 1s 计时;
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
					
					// 自动运行时间 1s 计时;
					if(Loop_Runing==1)	Loop_RunHoldtime++;	//自动控制,计时;
					
					
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
						Camera_TxRefresh(1);	// USB给视觉软件发送心跳;
					}
			}
			W5500_Send_Delay_Counter++; 
			
			//  自动运行时间 1ms 计时;
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
			
			//  视频时间 1ms 计时;
			if(Video_timeflag==1)
			{
				Video_timemisec++;
			}
			else Video_timemisec=0;
				
			//  激光笔时间 1ms 计时;
			if(Laser_OpenFlag==1)
			{
				Laser_OpenTimeout++;
			}
			
//			//  自动运行死区时间 1ms 计时;
//			if(StartNext_flag==1)
//			{
//				StartNext_Timeout++;
//			}
//			else	StartNext_Timeout=0;
			
			//////////////////// 蜂鸣器鸣叫操作 =1鸣叫一次时间1s; =3鸣叫一次时间0.5s; =5鸣叫两次0.5s; ///////////////////
			if(BUZZER_flag==1)
			{
				BUZZER_Count=0;	BUZZER_flag=2;	BUZZER_On;	//蜂鸣器启动;//=1鸣叫一次时间1s;
			}
			else if(BUZZER_flag==2)
			{
				BUZZER_Count++;
				if(BUZZER_Count<1000)	{	BUZZER_On;	}//蜂鸣器启动;
				else
				{
					BUZZER_Count=0;	BUZZER_flag=0;	BUZZER_Off;	//蜂鸣器停止;
				}
			}
			else if(BUZZER_flag==3)
			{
				BUZZER_Count=0;	BUZZER_flag=4;	BUZZER_On;	//蜂鸣器启动;//=3鸣叫一次时间0.5s;
			}
			else if(BUZZER_flag==4)
			{
				BUZZER_Count++;
				if(BUZZER_Count<500)	{	BUZZER_On;	}//蜂鸣器启动;
				else
				{
					BUZZER_Count=0;	BUZZER_flag=0;	BUZZER_Off;	//蜂鸣器停止;
				}
			}
			else if(BUZZER_flag==5)
			{
				BUZZER_Count=0;	BUZZER_flag=6;	BUZZER_On;	//蜂鸣器启动;//=5鸣叫两次0.5s;
			}
			else if(BUZZER_flag==6)
			{
				BUZZER_Count++;
				if(BUZZER_Count<500)	{	BUZZER_On;	}//蜂鸣器启动;	
				else if(BUZZER_Count<1000)	{	BUZZER_Off;	}//蜂鸣器停止;
				else if(BUZZER_Count<1500)	{	BUZZER_On;	}//蜂鸣器启动;
				else
				{
					BUZZER_Count=0;	BUZZER_flag=0;	BUZZER_Off;	//蜂鸣器停止;
				}
			}
			//////////////////// 蜂鸣器鸣叫操作 ,////////////////////////

				// MCGS RS485;超时结束一帧数据的接收;
				Rece_Time++;
				if(Rece_Time>40)
				{
					 Rece_TimeOut = 0 ;    //已超时，就不用加了

					 if(Rece_bit == 1)     // 最后一个字节，t3.5结束符
					 {
							Rece_bit = 0 ;
							USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);    // 禁止接收中断
					 }
				}
		}
		
		TIM_ClearITPendingBit(TIM7,TIM_IT_Update);
	}
}

/*************************************** 结束 ********************************************************/
