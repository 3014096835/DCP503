/*
 * @file          main.c
 * @brief         主程序: 主函数;
 * @ucos-ii       V2.86 
 * @function      QSS-ICBS1 
 * @compiler      RealView MDK-ARM Compiler  V5.14
 * @peripheral    V3.5.0
 * @author  	  	
 * @date   	      2024/05/08
 * @corporation   浙江求是科教设备有限公司
 * @website       http://www.zjqiushi.com/
*/
/* Includes ------------------------------------------------------------------*/
#include  "stm32f10x.h"
#include  "stm32f10x_conf.h" 
#include  "BoardVariable.h"
#include "delay.h"


// 开机界面/运行系统工作;
uint8_t HollowPage_flag=0;	// 处于欢迎界面的标记,=1处于欢迎界面;=0进入系统工作;
// 目标物体特征选择设置;
uint8_t Select_SetHang=0;// 物体特征选择页面:设置内容选择;//0-4;
uint8_t Select_ItemMode=4;// 识别模式,=1,静止单识别;=2,低速单识别;=3,高速单识别;=4,三个模式相同识别;//1/2/3,4;
uint8_t Select_Shape[3]={3,1,5};	// 形状特征；如值为{3,1,5},则说明分别设置静止、低速、高速模式下, 当前检测目标的形状特征为3号、1号、5号形状;
uint8_t Select_Color[3]={4,2,1};	// 颜色特征；如值为{4,2,1},则说明分别设置静止、低速、高速模式下, 当前检测目标的颜色特征为4号、2号、1号颜色;
uint8_t Select_Background=1;	// 背景色特征；如值为2,则说明设置检测视频的背景色为2号背景色;

void StartData_Init(void);
void StepMotor_Codeback(void);




int main(void)
{
		// 云台矫正依靠: Code,磁编码反馈角度;Motor,步进电机驱动位置反馈角度;
			// (USE_JiaoZhun_Code)=0,默认利用磁编码校准定位(16点);
			// (USE_JiaoZhun_Motor)=1,利用步进电机步距校准定位(16点);
		PTZ_JiaoZhun_USE = USE_JiaoZhun_Code;
	
		//************************************************//	
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
		//************************************************//	
		//////////////// 模块初始化 ///////////////////////
    BSP_Configuration(); 		// Init;GPIO;
    TIM7_Configuration(); 	// 定时器,1ms Delay;Init;	
	  delay_init();
		TIM2_Configuration();		// 步进电机驱动; Motor-Step;X;
		TIM3_Configuration();		// 步进电机驱动; Motor-Step;Y;
		AS5600_Init();					// 步进电机反馈; 磁编码 AS5600;IIC;
		UART_Init();						// 串口功能初始化配置;
	
		ADC_Configuration();		// 摇杆; AD_Value,YG;ADC;
	
		//////////////// 数据及状态初始化 ///////////////////////
		StartData_Init();							// 开机数据初始化
		ScreenOrigin_PositionSet();		// 修改定位原点为四个角之一,默认左下角;		
		StepMotor_Codeback();				 	// 获取云台初始角度;
		Init_PTZ_Position();					// 云台自动归零位;
		
		while (1)// 主程序;
    {
			switch(Set_flag)
			{
				case 0:
				case 1:		HollowPage_flag=1;	StartUp_Interface();		// 开机界面;	//进入欢迎界面;
					break;
				case 2:		Set0_Home();								break;		// 功能 2. 主界面 ;
				case 3:		Set1_TimeSel();							break;		// 功能 3. 时钟设置 ;
				case 4:		ManualControl_Onepoint();		break;		// 功能 4. 手动指示( 指示一个目标 ) ;
				case 5:		AutoPick_ModeSel();					break;		// 功能 5. 自动检测( 模式选择 ) ;
				case 6:		AutoPick_ModeRun();					break;		// 功能 6. 自动检测( 识别到暂停视频且激光不跟随运动 ) ;
				case 7:		Material_FeatureSel();				break;		// 设备 7: 物体特征设置;
				case 8:		Material_Adjust();						break;		// 设备 8: 参数校准;
				case 9:		Material_Select();						break;		// 设备 9: 设备参数调整使用功能选择;
				case 10:	Material_PTZposition();				break;		// 设备 10: 云台监测及校准;
				case 11:	ManualControl_Yaogan();			break;		// 功能 11. 手动指示( 手动摇杆控制 ) ;
				case 20:	Show_Wave();	 break;// 示波器显示;////20241020新增;
				default:break;
			}
			
			StepMotor_Codeback();				// 获取云台初始角度;云台磁编码二次调整定位;
			Deal_TranLaserDataMotor();	// 计算出当前实际坐标(步进电机步距定位);
			Deal_TranLaserDataCode();		// 计算出当前实际坐标(绝对位置编码器定位);
			
			if(LCD_Refreshflag==0)// LCD定时发送更新操作;// 启动发送数据至204A;
			{
				LCD_DealData();
				if(MCGS_Sendform==0)	MCGS_Sendform=5;	// 间隔时间,往触摸屏在线发送当前运行状态;
			}
			
			Function_Modbus();					// 判断485接收数据,启动相应发送;(触摸屏)
			Camera_RxRefresh();			// 分析摄像机串口数据;
		}
		
}

// 初始化: 参数及状态初始化;
void StartData_Init(void)
{
		// Init: 读取EEPROM内存储的校准数据;
			Flash_Init(1);
			
		// Init: 时钟初始化, 系统启动标志位置位: 灯亮,蜂鸣器响一声,启动语音播报;
			GPIO_SetBits(GPIOC, BSP_GPIOC_LED1);
			GPIO_SetBits(GPIOC, BSP_GPIOC_LED2);
			SysTime_Hour = 14;		SysTime_Minute = 27;		SysTime_Second = 1;
			System_Timer = SysTime_Hour*3600 + SysTime_Minute*60 + SysTime_Second;
			System_flag = 1;
			BUZZER_flag = 0;
			Sound_Start();
		// Init: 电视播放状态, 激光笔状态;
			Video_Mode=0;		Video_State=Video_Play;
			Laser_Off;
		// Init: 云台步进电机(控制),云台驱动;
			Run_Xnum = 0;	Run_Ynum = 0;
			Run_Xmotor=0;	Run_Ymotor=0;					// 初始化坐标参数为0;
			MOTX01_SetPul_HI;	MOTX01_SetEn_LI;	MOTX01_SetDir_HI;			// OUT1;// EN,DIR;X,
			MOTY02_SetPul_HI;	MOTY02_SetEn_LI;	MOTY02_SetDir_HI;			// OUT2;// EN,DIR;Y,
			StepMotor_StartRun;									// 启动步进电机定时器运行;
			Hand_MotorAdd = 16;										// 手动控制云台时,步长;
			C_shiftflag=1;											// 云台控制叠加校准值;//=0,不做二次校准;//=1,做校准;
		// Init: 云台步进电机(反馈),绝对位置检测(磁编码);
			CodeX.InitData = Arg_ReadAdj.CBMzeroX;
			CodeY.InitData = Arg_ReadAdj.CBMzeroY;
			CodeX.Compenflag = 0;
			CodeY.Compenflag = 0;
			CodeX.Error = NoErr;
			CodeY.Error = NoErr;
			IIC_GetInit=1;		// 启动,读取磁编码初始角度;
		// Init: 显示页面,功能选项;
			Set_flag = 1;	LCD_flag = 1; LCD_Refreshflag = 0;
			LCD_DealData();
			
			
			// 识别模式区分-部分参数初始化;//20241020新增;
			Camera_mode=1;// =0,默认单模型模式;=1,高级识别模式;
			Collect_Static.SUM=0;
			Collect_Low.SUM=0;
			Collect_High.SUM=0;
}


void StepMotor_Codeback(void)
{
		if(IIC_GetInit!=0)
		{
			Get_Ini_Val();// 获取初始角度;
			IIC_GetInit=0;
		}
		else
		{
			CodeX.Error=NoErr;
			CodeY.Error=NoErr;// 清除磁编码错误标记;
			Get_Num_sign(0);// 获取角度值;X轴;
			Get_Num_sign(1);// 获取角度值;Y轴;
		}
		
		if((CodeX.Compenflag==2)&&(CodeY.Compenflag==2))
		{
			if(PTZ_JiaoZhun_USE == USE_JiaoZhun_Code)	// (USE_JiaoZhun_Code)=0,默认利用磁编码校准定位(16点);
			{
					Init_PTZ_PID(CodeX.RealSet,CodeY.RealSet);// 云台定位时,为使定位更精准,二次调整;
			}
			CodeX.Compenflag=0;CodeY.Compenflag=0;
		}
}






