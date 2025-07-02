/*
 * @file          operate.c
 * @brief         程序: 开机界面、主界面、时钟设置界面、手动操作界面等部分函数;
 * @ucos-ii       V2.86 
 * @function      QSS-ICBS1 
 * @compiler      RealView MDK-ARM Compiler  V5.14
 * @peripheral    V3.5.0
 * @author  	  	
 * @date   	      2023/09/09
 * @corporation   浙江求是科教设备有限公司
 * @website       http://www.zjqiushi.com/
*/
/* Includes ------------------------------------------------------------------*/
#include  "stm32f10x.h"
#include  "stm32f10x_conf.h" 
#include  "BoardVariable.h"
#include "delay.h"
#include "math.h"
/* Private variables ---------------------------------------------------------*/
unsigned char ValIN_Setwei=0;								// 键盘数值输入;光标位选;
unsigned int	ValIN_Value[15]={0};						// 键盘数值输入;设定数值缓存;
unsigned int 	ValIN_OneBUF[40]={0};					// 键盘数值输入;设定数值单个值缓存;
unsigned char ValIN_count=2;								// 键盘数值输入;设定数值变量个数; -> 如2个变量待设定;
unsigned int 	ValIN_eachlen[15]={3,3};			// 键盘数值输入;设定数值变量长度分布; -> 如{3,3}变量分别为百位、百位长度;
unsigned int 	ValIN_eachmax[15]={999,999};	// 键盘数值输入;设定数值变量最大限值分布; -> 如{3,3}变量最大值分别为999、999;



/**********************************************************************************************************
* Function    ：void StartUp_Interface(void)
* Description : 开机界面 ;欢迎界面;
* Para        : none
* Return(s)   : none
* Example     : StartUp_Interface();
**********************************************************************************************************/
void StartUp_Interface(void)
{
		switch(Matrixkey_val)
		{
			case KEY_VAL8:		Video_HandControl=1;		Video_Control(Video_Pause);				break;// "8";暂停播放视频;
			case KEY_VAL9:		Video_HandControl=1;		Video_Control(Video_Play);				break;// "9";启动播放视频;
			case KEY_VAL5:		
//				Camera_TxRefresh(0);
			// 测试AI模式上传切换识别类选;//20241020新增;
			Camera_AIselect(3,0,12,15);
				break;// "5";// "5";更改相机方案;
			case KEY_VAL1:
						Laser_ScreenX = Screen_XRealZero;
						Laser_ScreenY = Screen_YRealZero;
						Deal_EnLaserData(Laser_ScreenX,Laser_ScreenY);
				break;	// "1";To Point(0,0);
			case KEY_VAL2:			Init_PTZ_Position();// 云台自动归零位;
				break;	// "2";To Point_Init;
			case KEY_VAL0:
						Run_Xmotor = 0;	Run_Ymotor = 0;
						Run_Xnum = 0;	Run_Ynum = 0;
						IIC_GetInit=1;//Get_Ini_Val();// 获取初始角度;
						Arg_ReadAdj.CBMzeroX=CodeX.RealRead;
						Arg_ReadAdj.CBMzeroY=CodeY.RealRead;
						Flash_eepromIn();	//EEPROM 模拟 FLASH 存入;
						ScreenOrigin_PositionSet();
						BUZZER_flag=3;
				break;	// "0";ReInitialize Zero Point;云台初始化设置零位点;
			case KEY_VAL6:		Run_Ymotor = Run_Ymotor+Hand_MotorAdd;			break;	// 8,"6";云台上移;
			case KEY_VAL3:		Run_Ymotor = Run_Ymotor-Hand_MotorAdd;			break;	// 12,"3";云台下移;
			case KEY_VAL7:		Run_Xmotor = Run_Xmotor+Hand_MotorAdd;			break;	// 2,"7";云台右移;
			case KEY_VAL4:		Run_Xmotor = Run_Xmotor-Hand_MotorAdd;			break;	// 6,"4";云台左移;
			case KEY_UP:			Laser_On;							break;	//1,"Up";激光笔打开;
			case KEY_DOWN:		Laser_Off;						break;	//5,"Down";激光笔关闭;
			default:break;
		}
		if(Matrixkey_val==KEY_VALE)	// "E";Enter the main interface;
		{
			Matrixkey_val=0;
			LCD_flag = 2;	Set_flag=2;		// 系统启动;// 跳转主界面;
			LCD_DealData();
			HollowPage_flag=0;			//	退出欢迎界面;
			LCD_ReTime=1;						// 启动向204A发送时钟,跟随主系统;
			PTZ_JiaoZhun_USE = USE_JiaoZhun_Code;		// (USE_JiaoZhun_Code)=0,默认利用磁编码校准定位(16点);
		}
		Matrixkey_val=0;
		
		switch(Presskey_val)
		{
			case KEY_F1:	Presskey_val=0;
					if(Hand_MotorAdd==4)	Hand_MotorAdd=16;// 切换按键移动云台时,电机转动的步数;
					else if(Hand_MotorAdd==16)	Hand_MotorAdd=64;
					else	Hand_MotorAdd=4;
				break;
			case KEY_F2:	Presskey_val=0;	
					Set_flag=10;LCD_flag = 10;	// 云台监测界面;Material_PTZposition();
				break;
			case KEY_F3:	Presskey_val=0;	
					Set_flag=8;	LCD_flag = 8;		// 跳转校准界面;Material_Adjust();
					Keyset_ValueInit0(Set_flag);// 有数值输入功能,数值初始化;
				break;
			case KEY_F4:	Presskey_val=0;	
					Set_flag=7;	LCD_flag = 7;		// 跳转物体特征选择界面;Material_FeatureSel();
					LCD_DealData();
				break;
			default:	break;
		}
}

/**********************************************************************************************************
* Function    ：void Set0_Home(void)
* Description : 主界面 ;
* Para        : none
* Return(s)   : none
* Example     : Set0_Home();
**********************************************************************************************************/
void Set0_Home(void)
{
		switch(Matrixkey_val)
		{
			case KEY_VAL0:// 进入设备使用功能设置;
							Matrixkey_val=0;
							Set_flag=9;		LCD_flag=9;		BUZZER_flag=3;
							LCD_DealData();
				break;
			case KEY_VAL1:	// Set_flag/LCD_flag = 3.时间设置 ;
							Matrixkey_val=0;
							Set_flag=3;		LCD_flag=3;
							Keyset_ValueInit0(Set_flag);// 有数值输入功能,数值初始化;
							LCD_DealData();
				break;
			case KEY_VAL2:	// Set_flag/LCD_flag = 4.手动指示 一个目标;
							Matrixkey_val=0;
							Set_flag=4;		LCD_flag=4;
					Loop_Runing=0;	Loop_RunHoldtime=0;	//自动控制,计时;
							Keyset_ValueInit0(Set_flag);// 有数值输入功能,数值初始化;
							LCD_DealData();
				break;
			case KEY_VAL3:	// Set_flag/LCD_flag = 5.自动检测( 模式选择 ) ;
							Matrixkey_val=0;
							Set_flag=5;		LCD_flag=5;
							LCD_DealData();
				break;
			case KEY_VAL4:	// Set_flag/LCD_flag = 11.手动指示 摇杆控制 ;
							Matrixkey_val=0;
							Set_flag=11;		LCD_flag=11;
					Loop_Runing=0;	Loop_RunHoldtime=0;	//自动控制,计时;
							LCD_DealData();
				break;
			case KEY_VAL5: // Set_flag/LCD_flag = 20.示波器显示 ;//20241020新增;
				            Matrixkey_val=0;
							Set_flag=20;		LCD_flag=20;
							LCD_DealData();
			default:break;
		}
		Presskey_val=0;
		Matrixkey_val=0;
}


// 示波器显示;////20241020新增;
/**********************************************************************************************************
* Function    ：void AutoPick_NopauseFllow(void)
* Description : // "5" 示波器采集显示 ;
* Para        : none
* Return(s)   : none
* Example     : AutoPick_NopauseFllow();
**********************************************************************************************************/
void Show_Wave(void)
{
		if(Matrixkey_val==14)			// "D";退回主界面;
		{
			Matrixkey_val=0;
			Set_flag=2;LCD_flag=2;
			LCD_DealData();
		}
	
}

/**********************************************************************************************************
* Function    ：void Set1_TimeSel(void)
* Description : 1.时钟设置 ;
* Para        : none
* Return(s)   : none
* Example     : Set1_TimeSel();
**********************************************************************************************************/
void Set1_TimeSel(void)
{
		// 按键操作函数;注意此处函数,Matrixkey_val未清0;
		Keyset_ValueInput();
		if(Matrixkey_val == KEY_VALD)// D ;退回主界面;
		{
				Set_flag=2;LCD_flag=2;
				LCD_DealData();
		}
		else if(Matrixkey_val == KEY_VALE)// E ;确认时间;
		{
				System_Timer = ValIN_Value[0]*3600 + ValIN_Value[1]*60 + ValIN_Value[2];
				SysTime_Hour = ValIN_Value[0];
				SysTime_Minute = ValIN_Value[1];
				SysTime_Second = ValIN_Value[2];
				LCD_ReTime=1;
				LCD_DealData();
				BUZZER_flag=3;
		}
		Matrixkey_val = 0;
		
		Presskey_val=0;
}


////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////


/**********************************************************************************************************
* Function    ：void ManualControl_Onepoint(void)
* Description : 手动指示 ;手动指示( 指示一个目标 ) ;
* Para        : none
* Return(s)   : none
* Example     : ManualControl_Onepoint();
**********************************************************************************************************/
void ManualControl_Onepoint(void)
{
		switch(Presskey_val)
		{
			case KEY_F1:			break;
			case KEY_F2:			break;
			case KEY_F3:			break;
			case KEY_F4:
				ValIN_Setwei=0;
				if(Laser_State==0)	// 启动或关闭激光笔;
				{
						Laser_ScreenX = ValIN_Value[0];
						Laser_ScreenY = ValIN_Value[1];
						Deal_EnLaserData(Laser_ScreenX,Laser_ScreenY);
					Laser_OpenFlag=1;		// 先不开激光笔,等待云台到达目的点后打开;
					Laser_OpenTimeout=0;
					Loop_Runing=1;	Loop_RunHoldtime=0;	//自动控制,计时;
				}
				else 
				{
					Laser_Off;// 关闭激光;
					Laser_OpenFlag=0;
					Loop_Runing=0;	//自动控制,计时;
					LCD_DealData();
				}
				break;
			default:	break;
		}
		Presskey_val=0;
		
		// 等待云台到达目标位置;
		if((Run_Xmotor == Run_Xnum)&&(Run_Ymotor == Run_Ynum) )
			{
				if(Laser_OpenFlag==1)// 激光在等待被打开;打开激光;语音播报;
				{
					Laser_On;
					Laser_OpenFlag=0;
					BUZZER_flag=3;	Sound_HandSet();
					LCD_DealData();
				}
			}
			
		// 按键操作函数;注意此处函数,Matrixkey_val未清0;
		Keyset_ValueInput();
		if(Matrixkey_val == KEY_VALD)// D ;退回手动模式选择界面(非主界面);关闭激光;
		{
				Laser_Off;Laser_OpenFlag=0;Loop_Runing=0;	//自动控制,计时;
				Set_flag=2;LCD_flag=2;
				LCD_DealData();
		}
		Matrixkey_val = 0;
			
}


/**********************************************************************************************************
* Function    ：void ManualControl_Yaogan(void)
* Description : .手动摇杆指示 ;
* Para        : none
* Return(s)   : none
* Example     : ManualControl_Yaogan();
**********************************************************************************************************/
void ManualControl_Yaogan(void)
{
		YGHand_flag=1;	// 开放摇杆控制状态;	
	
		// 摇杆电位器按键;状态检测;
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2 )==0)	
		{
			while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2 )==0);
			AN1_flag=1;
		}
		else AN1_flag=0;
		
		// 摇杆按下状态;
		if(AN1_flag==1)	
		{
			if(Laser_State==0)
			{
				Laser_On;	// 打开激光;
				Laser_OpenFlag=0;	Laser_OpenTimeout=0;
				Laser_ScreenX = XRealAll;
				Laser_ScreenY = YRealAll;// XRealAll, YRealAll, 为当前实时坐标值;
					Sound_HandSet();// 语音播报;
				Loop_Runing=1;	Loop_RunHoldtime=0;	//自动控制,计时;
			}
			else 
			{
				Laser_Off;	Laser_OpenFlag=0;// 关闭激光;
				Loop_Runing=0;	//自动控制,计时;
			}
		}
		switch(Presskey_val)
		{
			case KEY_F1:	MOT_SetSpeed_HI;	// 步进电机云台运动速度调节;快速;
				break;
			case KEY_F2:	MOT_SetSpeed_LI;	// 步进电机云台运动速度调节;慢速;
				break;
			case KEY_F3:		break;
			case KEY_F4:		break;
			default:	break;
		}
		Presskey_val=0;

		switch(Matrixkey_val)
		{
			case KEY_VALD:// D ;退回手动模式选择界面(非主界面);关闭激光;
					Laser_Off;Laser_OpenFlag=0;
				YGHand_flag=0;Loop_Runing=0;	//自动控制,计时;
					Set_flag=2;LCD_flag=2;
					LCD_DealData();
				break;
			case KEY_VALE:
				break;
			default:	break;
		}
		Matrixkey_val=0;
		
		LCD_DealData();
			
}


///////////////////////////////////////////////////////////////////////////////////////////////////


/**********************************************************************************************************
* Function    ：void Keyset_ValueInit0(void)
* Description : 键盘功能 - 数值输入及设定功能初始化;置于首次进入当前页面的设置;
* Para        : none
* Return(s)   : none
* Example     : Keyset_ValueInit0();
**********************************************************************************************************/
void Keyset_ValueInit0(unsigned int type)
{
		unsigned char iv=0;
		unsigned int tmpcnt=0;
		
		switch(type)
		{
			case 3://时钟设置页面;
					ValIN_Setwei = 0;
					ValIN_count = 3;
						ValIN_Value[0] = SysTime_Hour;
						ValIN_Value[1] = SysTime_Minute;
						ValIN_Value[2] = SysTime_Second;
					ValIN_eachlen[0] = 2;
					ValIN_eachlen[1] = 2;
					ValIN_eachlen[2] = 2;
						ValIN_eachmax[0] = 30;//
						ValIN_eachmax[1] = 60;
						ValIN_eachmax[2] = 60;
					ValIN_OneBUF[0] = ValIN_Value[0]/10%10;
					ValIN_OneBUF[1] = ValIN_Value[0]/1%10;
					ValIN_OneBUF[2] = ValIN_Value[1]/10%10;
					ValIN_OneBUF[3] = ValIN_Value[1]/1%10;
					ValIN_OneBUF[4] = ValIN_Value[2]/10%10;
					ValIN_OneBUF[5] = ValIN_Value[2]/1%10;
				break;
			case 4://手动指示-1;
					ValIN_Setwei = 0;
					ValIN_count = 2;
						if(PTZ_JiaoZhun_USE == USE_JiaoZhun_Motor)		// (USE_JiaoZhun_Motor)=1,利用步进电机步距校准定位(16点);
						{
							ValIN_Value[0]=XReal_Motor;
							ValIN_Value[1]=YReal_Motor;
						}
						else		// (USE_JiaoZhun_Code)=0,默认利用磁编码校准定位(16点);
						{
							ValIN_Value[0]=XReal_Code;
							ValIN_Value[1]=YReal_Code;
						}
					for(iv=0;iv<ValIN_count;iv++)
					{
						ValIN_eachlen[iv] = 3;
						ValIN_eachmax[iv] = 999;
					}
					for(iv=0;iv<ValIN_count;iv++)
					{
						ValIN_OneBUF[iv*3+0] = ValIN_Value[iv]/100%10;
						ValIN_OneBUF[iv*3+1] = ValIN_Value[iv]/10%10;
						ValIN_OneBUF[iv*3+2] = ValIN_Value[iv]/1%10;
					}
				break;
			case 8://参数校准设置;
					ValIN_Setwei = 0;
					ValIN_count = 10;
					ValIN_Value[0] = Arg_ReadAdj.PointaX;
					ValIN_eachlen[0] = 4;
					ValIN_eachmax[0] = 9999;
					ValIN_Value[1] = Arg_ReadAdj.PointaY;
					ValIN_eachlen[1] = 4;
					ValIN_eachmax[1] = 9999;
					ValIN_Value[2] = Arg_ReadAdj.LASER_Axis;
					ValIN_eachlen[2] = 4;
					ValIN_eachmax[2] = 9999;
					ValIN_Value[3] = Arg_ReadAdj.MatchY;
					ValIN_eachlen[3] = 4;
					ValIN_eachmax[3] = 9999;
					ValIN_Value[4] = Arg_ReadAdj.CBMzeroX;
					ValIN_eachlen[4] = 4;
					ValIN_eachmax[4] = 9999;
					ValIN_Value[5] = Arg_ReadAdj.CBMzeroY;
					ValIN_eachlen[5] = 4;
					ValIN_eachmax[5] = 9999;
					ValIN_Value[6] = Arg_ReadAdj.Volume;
					ValIN_eachlen[6] = 2;
					ValIN_eachmax[6] = 16;
					ValIN_Value[7] = Arg_ReadAdj.LowSpeed;
					ValIN_eachlen[7] = 2;
					ValIN_eachmax[7] = 99;
					ValIN_Value[8] = Arg_ReadAdj.HighSpeed;
					ValIN_eachlen[8] = 2;
					ValIN_eachmax[8] = 99;
					ValIN_Value[9] = Arg_ReadAdj.RunDir;
					ValIN_eachlen[9] = 1;
					ValIN_eachmax[9] = 1;
					tmpcnt=0;
					for(iv=0;iv<ValIN_count;iv++)
					{
						if(ValIN_eachlen[iv] == 1)
						{
							ValIN_OneBUF[tmpcnt+0] = ValIN_Value[iv]/1%10;
						}
						else if(ValIN_eachlen[iv] == 2)
						{
							ValIN_OneBUF[tmpcnt+0] = ValIN_Value[iv]/10%10;
							ValIN_OneBUF[tmpcnt+1] = ValIN_Value[iv]/1%10;
						}
						else if(ValIN_eachlen[iv] == 3)
						{
							ValIN_OneBUF[tmpcnt+0] = ValIN_Value[iv]/100%10;
							ValIN_OneBUF[tmpcnt+1] = ValIN_Value[iv]/10%10;
							ValIN_OneBUF[tmpcnt+2] = ValIN_Value[iv]/1%10;
						}
						else if(ValIN_eachlen[iv] == 4)
						{
							ValIN_OneBUF[tmpcnt+0] = ValIN_Value[iv]/1000%10;
							ValIN_OneBUF[tmpcnt+1] = ValIN_Value[iv]/100%10;
							ValIN_OneBUF[tmpcnt+2] = ValIN_Value[iv]/10%10;
							ValIN_OneBUF[tmpcnt+3] = ValIN_Value[iv]/1%10;
						}
						tmpcnt = tmpcnt + ValIN_eachlen[iv];
					}
					break;
			default:	break;
		}
}

/**********************************************************************************************************
* Function    ：void Keyset_ValueInput(void)
* Description : 键盘功能 - 数值输入及设定;使用时注意Matrixkey_val未清0;
* Para        : none
* Return(s)   : none
* Example     : Keyset_ValueInput();
**********************************************************************************************************/
unsigned int Keyset_ValueInput(void)
{
		unsigned char i=0;
		unsigned char j=0;
		unsigned char tmp_sum=0;
		unsigned int Data_change=0;
		unsigned int tmp_valin=0;
		unsigned int Len=0;

		// 统计处理数字的位数长度;
		tmp_sum = 0;
		for(i=0;i<ValIN_count;i++)
		{
			tmp_sum = tmp_sum + ValIN_eachlen[i];
		}
		Len = tmp_sum;
		
		if((Len<40) && (Matrixkey_val != 0) && (Matrixkey_val != KEY_VALD) && (Matrixkey_val != KEY_VALE))// 限制条件;
		{
			// 矩阵键盘:0~9,up,dowm,left,right;构成数值输入功能;
			switch(Matrixkey_val)
			{
					case KEY_UP:			if(ValIN_Setwei==Len)	ValIN_Setwei=0;
														if(ValIN_OneBUF[ValIN_Setwei]<9)			ValIN_OneBUF[ValIN_Setwei]=ValIN_OneBUF[ValIN_Setwei]+1;
														else		ValIN_OneBUF[ValIN_Setwei]=0;
														Data_change = 2;	
						break;	// Up;
					case KEY_DOWN:		if(ValIN_Setwei==Len)	ValIN_Setwei=0;
														if(ValIN_OneBUF[ValIN_Setwei]>0)			ValIN_OneBUF[ValIN_Setwei]=ValIN_OneBUF[ValIN_Setwei]-1;
														else		ValIN_OneBUF[ValIN_Setwei]=9;
														Data_change = 2;	
						break;	// Down;
					case KEY_LEFT:		if(ValIN_Setwei>0)	ValIN_Setwei=ValIN_Setwei-1;
														else	ValIN_Setwei=Len;// 往后递推;
						break;	// Left;
					case KEY_RIGHT:		if(ValIN_Setwei<Len)	ValIN_Setwei=ValIN_Setwei+1;
														else ValIN_Setwei=0;// 往后递推;
						break;	// Right;
					case KEY_VAL0:		ValIN_OneBUF[ValIN_Setwei]=0;	Data_change = 1;	break;// 0 ;Start_X  Start_Y  Stop_X  Stop_Y
					case KEY_VAL1:		ValIN_OneBUF[ValIN_Setwei]=1;	Data_change = 1;	break;// 1 ;
					case KEY_VAL2:		ValIN_OneBUF[ValIN_Setwei]=2;	Data_change = 1;	break;// 2 ;
					case KEY_VAL3:		ValIN_OneBUF[ValIN_Setwei]=3;	Data_change = 1;	break;// 3 ;
					case KEY_VAL4:		ValIN_OneBUF[ValIN_Setwei]=4;	Data_change = 1;	break;// 4 ;
					case KEY_VAL5:		ValIN_OneBUF[ValIN_Setwei]=5;	Data_change = 1;	break;// 5 ;
					case KEY_VAL6:		ValIN_OneBUF[ValIN_Setwei]=6;	Data_change = 1;	break;// 6 ;
					case KEY_VAL7:		ValIN_OneBUF[ValIN_Setwei]=7;	Data_change = 1;	break;// 7 ;
					case KEY_VAL8:		ValIN_OneBUF[ValIN_Setwei]=8;	Data_change = 1;	break;// 8 ;
					case KEY_VAL9:		ValIN_OneBUF[ValIN_Setwei]=9;	Data_change = 1;	break;// 9 ;
					default:	break;
			}

			// 根据键盘输入值变化,推算单项数值等信息;
			if(Data_change != 0)
			{
					if(ValIN_Setwei==Len)
					{
							ValIN_OneBUF[0] = ValIN_OneBUF[Len];
							i=0;	tmp_sum = 0;
							tmp_valin = 0;
							for(j=0;j<ValIN_eachlen[i];j++)
							{
								tmp_valin = tmp_valin*10 + ValIN_OneBUF[tmp_sum+j];
							}
							if(tmp_valin<=ValIN_eachmax[i])			ValIN_Value[i] = tmp_valin;
							else	
							{
								Data_change=0;
									if(ValIN_eachlen[i] == 1)
									{
										ValIN_OneBUF[tmp_sum-1] = ValIN_Value[i]/1%10;
									}
									else if(ValIN_eachlen[i] == 2)
									{
										ValIN_OneBUF[tmp_sum-2] = ValIN_Value[i]/10%10;
										ValIN_OneBUF[tmp_sum-1] = ValIN_Value[i]/1%10;
									}
									else if(ValIN_eachlen[i] == 3)
									{
										ValIN_OneBUF[tmp_sum-3] = ValIN_Value[i]/100%10;
										ValIN_OneBUF[tmp_sum-2] = ValIN_Value[i]/10%10;
										ValIN_OneBUF[tmp_sum-1] = ValIN_Value[i]/1%10;
									}
									else if(ValIN_eachlen[i] == 4)
									{
										ValIN_OneBUF[tmp_sum-4] = ValIN_Value[i]/1000%10;
										ValIN_OneBUF[tmp_sum-3] = ValIN_Value[i]/100%10;
										ValIN_OneBUF[tmp_sum-2] = ValIN_Value[i]/10%10;
										ValIN_OneBUF[tmp_sum-1] = ValIN_Value[i]/1%10;
									}
							}
					}
					else
					{
							tmp_sum = 0;
							for(i=0;i<ValIN_count;i++)
							{
								tmp_valin = 0;
								for(j=0;j<ValIN_eachlen[i];j++)
								{
									tmp_valin = tmp_valin*10 + ValIN_OneBUF[tmp_sum+j];
								}
								tmp_sum = tmp_sum + ValIN_eachlen[i];
								if(tmp_valin<=ValIN_eachmax[i])			ValIN_Value[i] = tmp_valin;
								else	
								{
									Data_change=0;
										if(ValIN_eachlen[i] == 1)
										{
											ValIN_OneBUF[tmp_sum-1] = ValIN_Value[i]/1%10;
										}
										else if(ValIN_eachlen[i] == 2)
										{
											ValIN_OneBUF[tmp_sum-2] = ValIN_Value[i]/10%10;
											ValIN_OneBUF[tmp_sum-1] = ValIN_Value[i]/1%10;
										}
										else if(ValIN_eachlen[i] == 3)
										{
											ValIN_OneBUF[tmp_sum-3] = ValIN_Value[i]/100%10;
											ValIN_OneBUF[tmp_sum-2] = ValIN_Value[i]/10%10;
											ValIN_OneBUF[tmp_sum-1] = ValIN_Value[i]/1%10;
										}
										else if(ValIN_eachlen[i] == 4)
										{
											ValIN_OneBUF[tmp_sum-4] = ValIN_Value[i]/1000%10;
											ValIN_OneBUF[tmp_sum-3] = ValIN_Value[i]/100%10;
											ValIN_OneBUF[tmp_sum-2] = ValIN_Value[i]/10%10;
											ValIN_OneBUF[tmp_sum-1] = ValIN_Value[i]/1%10;
										}
								}
							}
					}
					if(Data_change==1)
					{
						if(ValIN_Setwei<Len)	ValIN_Setwei=ValIN_Setwei+1;// 往后递推;
						else if(ValIN_Setwei==Len)	ValIN_Setwei=1;// 往后递推;
						else ValIN_Setwei=0;
					}
					else if(Data_change==2)
					{
						if(ValIN_Setwei==Len)	ValIN_Setwei=0;
					}
			}
			Matrixkey_val=0;
			LCD_DealData();
			
			if(Data_change!=0)
			{
				Data_change=0;
				return 1;
			}
			else	return 0;
		}
		else	return 0;
		
}

