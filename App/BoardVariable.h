#ifndef __BoardVariable_H
#define __BoardVariable_H

#include  "stm32f10x.h"
#include  "stm32f10x_conf.h" 
#include  "math.h"
#include <string.h>
#include <stdlib.h>
#include  "bsp.h"
#include  "BoardVariable.h"
#include  "mbcrc.h"
#include  "Timer.h"
#include  "syn6288.h"
#include  "operate.h"
#include  "adjust.h"
#include 	"AS5600.h"
#include  "Function.h"
#include  "USART.h"
#include  "adc.h"
#include "ConvertPoint.h"

/* Private define ------------------------------------------------------------*/


//////// 键盘按键状态; 定义键盘符号 -> 对应机械键值;///////
#define  KEY_UP					1
#define  KEY_VAL7				2
#define  KEY_VAL8				3
#define  KEY_VAL9				4
//
#define  KEY_DOWN				5
#define  KEY_VAL4				6
#define  KEY_VAL5				7
#define  KEY_VAL6				8
//
#define  KEY_LEFT				9
#define  KEY_VAL1				10
#define  KEY_VAL2				11
#define  KEY_VAL3				12
//
#define  KEY_RIGHT			13
#define  KEY_VALD				14
#define  KEY_VAL0				15
#define  KEY_VALE				16
//
#define  KEY_F1				1
#define  KEY_F2				2
#define  KEY_F3				3
#define  KEY_F4				4

/////////////// 步进电机磁编码连接状态 /////////////////////
#define Err	0xff							// 绝对位置传感器;错误信息;,error;
#define NoErr	0x00						// 绝对位置传感器;错误信息;,no error;

////////////// 激光笔控制 ///////////////////////////////////////////
#define BSP_GPIOD_KIA				GPIO_Pin_1		// KIA,PD1;  
#define BSP_GPIOD_KIB				GPIO_Pin_0		// KIB,PD0;
#define Laser_On		{Laser_State = 1; GPIO_SetBits(GPIOD, BSP_GPIOD_KIA|BSP_GPIOD_KIB);}
#define Laser_Off		{Laser_State = 0; GPIO_ResetBits(GPIOD, BSP_GPIOD_KIA|BSP_GPIOD_KIB);}
	
//////////////// BUZZER; 蜂鸣器控制; ///////////////////////////////
#define     BSP_GPIOC_BUZZER          GPIO_Pin_13  // PC13(IO7) 
#define 		BUZZER_On		GPIO_SetBits(GPIOC, BSP_GPIOC_BUZZER)
#define 		BUZZER_Off	GPIO_ResetBits(GPIOC, BSP_GPIOC_BUZZER)

//////////////////////////// 步进电机=0; 云台控制;////////////////////////////////////////
// 脉冲输出端;
#define       GPIOE_MOTPUL01          	GPIO_Pin_12  	// MOT1 脉冲输出端;JH1_ PE1; => JI1_ PE12;
#define       GPIOE_MOTPUL02          	GPIO_Pin_11  	// MOT2 脉冲输出端;JH4_ PE4; => JI4_ PE11;
// 使能端;
#define       GPIOE_MOTEN01          	GPIO_Pin_9  		// MOT1 使能端;JH2_ PE2; => JI2_ PE9;
#define       GPIOE_MOTEN02          	GPIO_Pin_10   	// MOT2 使能端;JH5_ PE5; => JI5_ PE10;
// 方向控制端;
#define       GPIOE_MOTDIR01          	GPIO_Pin_14  	// MOT1 方向控制端;//JH0_ PE0; => JI0_ PE14;
#define       GPIOE_MOTDIR02          	GPIO_Pin_8  	// MOT2 方向控制端;//JH3_ PE3; => JI3_ PE8;

// Motor_1,步进电机;
#define 	MOTX01_SetEn_LI			GPIO_ResetBits(GPIOE, GPIOE_MOTEN01)
#define 	MOTX01_SetEn_HI			GPIO_SetBits(GPIOE, GPIOE_MOTEN01)
#define 	MOTX01_SetDir_HI			GPIO_ResetBits(GPIOE, GPIOE_MOTDIR01)
#define 	MOTX01_SetDir_LI			GPIO_SetBits(GPIOE, GPIOE_MOTDIR01)
// Motor_2,步进电机;
#define 	MOTY02_SetEn_LI			GPIO_ResetBits(GPIOE, GPIOE_MOTEN02)
#define 	MOTY02_SetEn_HI			GPIO_SetBits(GPIOE, GPIOE_MOTEN02)
#define 	MOTY02_SetDir_HI			GPIO_ResetBits(GPIOE, GPIOE_MOTDIR02)
#define 	MOTY02_SetDir_LI			GPIO_SetBits(GPIOE, GPIOE_MOTDIR02)
// 脉冲输出高电平,"1";
#define MOTX01_SetPul_HI		GPIO_SetBits(GPIOE, GPIOE_MOTPUL01)		// MOT1 脉冲输出端;JI5;// PE10;
#define MOTY02_SetPul_HI		GPIO_SetBits(GPIOE, GPIOE_MOTPUL02)			// MOT2 脉冲输出端;JH5;// PE5;
// 脉冲输出低电平,"0";
#define MOTX01_SetPul_LI		GPIO_ResetBits(GPIOE, GPIOE_MOTPUL01)	// MOT1 脉冲输出端;JI5;// PE10;
#define MOTY02_SetPul_LI		GPIO_ResetBits(GPIOE, GPIOE_MOTPUL02)		// MOT2 脉冲输出端;JH5;// PE5;
////////////////////////////// 步进电机=0; 云台控制;////////////////////////////////////////

/////////////////////// 步进电机=0; 云台调整速度; //////////////////////////////////////////
#define MOT_SetSpeed_HI			TIM_SetAutoreload(TIM2,500);TIM_SetAutoreload(TIM3,500);  // 定时器比较值;
#define MOT_SetSpeed_LI			TIM_SetAutoreload(TIM2,1000);TIM_SetAutoreload(TIM3,1000);  // 定时器比较值;




/* Private define end ------------------------------------------------------------*/




/* Private variables ----------------------------------------------------------------------------------*/


// 键盘;
extern unsigned char	Presskey_val;// 独立按键;
extern unsigned char	Matrixkey_val;// 矩阵按键;


// 运维系统部分;(MCGS 触摸屏)// RS485接收;触摸屏双向通讯控制;
extern uint8_t  Rece_Time;				// 接收超时时间;
extern uint8_t	Rece_TimeOut;			// 接收超时标志;=1,未超时;=0,超时;
extern uint8_t  Rece_Buffer[50];  // 直接从串口中断中得到的数据
extern uint8_t  Rece_RxCounter;   // 表示串口USART1接收到的数据个数
extern uint8_t  Rece_bit;					// =1,表示有接收到首位数据;
extern uint8_t  MCGS_Sendform;		// 不等于0时; 启动,向触摸屏发送对应格式的数据;
extern uint8_t	MCGS_Message;				// MCGS信息数据序号编号;
extern uint16_t Send_CRC ;					// 计算待发送数据的CRC校验值
extern uint8_t  MCGS_TxBuffer[50];	// 响应“MCGS发送的请求信息”，同时是本地控制器转发数据的数组
// 传送带视频部分;
#define  Dir_Leftdown 	0							// 视频播放运动方向;向左;
#define  Dir_Rightup 		1							// 视频播放运动方向;向右;
#define  Video_Pause 	0x01 						// 视频暂停;Pause/0x01;
#define  Video_Play 	0x02 						// 视频播放;Play/0x02;
extern uint8_t  Video_HandControl;		// 手动控制视频播放或暂停状态;Play/Pause;
extern uint8_t  Video_State;					// 视频播放或暂停状态;Play/Pause;
extern uint8_t  Video_Mode;						// 视频播放模式;静止,低速,高速;
extern uint8_t  Video_timeflag;				// 发送暂停视频的指令后标记为1,超时计数;发送播放指令后标记清0;
extern uint16_t Video_timesec;				// 发送暂停视频的指令后标记为1,超时计数值(s);
extern uint16_t Video_timemisec;			// 发送暂停视频的指令后标记为1,超时计数值(s);
extern uint8_t Video_TxSend[5];				// 串口发送缓存区;
// 摄像机部分;(UART4)
extern uint8_t Cam_TxBuffer[20];		// 相机端口发送数据缓冲区 
extern uint8_t Cam_RxBuffer[200];	// 相机端口接收数据缓冲区 
extern uint16_t Cam_RxNum;							// 相机接收到有效数据的数据个数;
extern uint8_t Cam_Rxflag;						// W5500接收到有效数据标记为1;
extern uint8_t USB_BeatNUM;
// 摄像机部分;(W5500)
extern uint16_t W5500_Send_Delay_Counter; //W5500发送延时计数变量(ms)
//extern uint8_t W5500_Rxflag;				// W5500接收到有效数据标记为1;
//extern uint16_t W5500_RxNum;				// W5500接收到有效数据的数据个数;
// 激光部分;
extern uint8_t Laser_State;					// 激光笔控制,亮灭状态;
extern uint8_t Laser_OpenFlag;			// 激光笔控制,自动指示时,启动激光标记为1,超时计数配套Timeout;
extern uint16_t Laser_OpenTimeout;	// 激光笔控制,自动指示时,启动激光标记为1,超时计数值(ms);


// 定位部分;
extern long int Laser_ScreenX,Laser_ScreenY;					// X,Y. Point;设置屏幕目标位置;屏幕坐标,(mm)
extern long int Laser_ScrnSaveX,Laser_ScrnSaveY;			// X,Y. Point;设置屏幕目标位置;屏幕坐标,(mm)
extern long int Run_Xmotor,Run_Ymotor;								// X,Y. Point;设置屏幕目标位置;电机目标移动位置;脉冲数坐标,(Pluse);
extern uint16_t MOT_DirX,MOT_DirY;										// X,Y. Motor Dir: 0,停止; 1,前进; 2,后退;
extern long int Run_Xnum,Run_Ynum;										// X,Y. Pluse Count;电机当前移动位置;脉冲数坐标,(Pluse);
extern long int Obj_DataX,Obj_DataY;									// X,Y. object-Point;电机当前目标移动位置;转换的屏幕坐标,(mm);
extern uint16_t C_shiftflag;						// 云台控制叠加量;//=0,不做偏移标记;//=1,做偏移;
extern uint16_t Hand_MotorAdd;					// 云台手动调整间隔;

// 绝对位置传感器;
typedef struct
{
	long RealRead;  // 当前磁编码数据值;
	long RealOld;  // 当前磁编码数据值;
	long AddRead;   // 当前磁编码数据增量;
	long AddSet;  	// 当前磁编码数据目标增量;
	long RealSet;  	// 当前磁编码数据目标数值;
	long InitData;   	// 磁编码零位值;
	unsigned char Error;  // 磁编码错误状态;
	unsigned char Compenflag;   // 当前磁编码数据值;启动补偿;
}CBM_Code;
extern CBM_Code CodeX;// 绝对位置数据信息;
extern CBM_Code CodeY;// 绝对位置数据信息;
extern uint8_t IIC_GetInit;// 绝对位置角度初始化标记;
// 云台校准方案;
#define 	USE_JiaoZhun_Code				0				// =0,默认使用电机磁编码反馈校准;
#define 	USE_JiaoZhun_Motor			1				// =1,选择使用步进电机脉冲校准;
extern unsigned char PTZ_JiaoZhun_USE;		// 校准方式模式选择; // =0,默认使用电机磁编码反馈校准; =1,选择使用步进电机脉冲校准;


// 系统其他参数;
extern uint8_t System_flag;			// 系统初始化成功,正常运行则标记为1;
extern uint8_t BUZZER_flag;			// 蜂鸣器;设置标记为1/3/5; 1:响一声1s; 3:响一声0.5s; 5:响两声0.5s; 
extern uint16_t BUZZER_Count;		// 蜂鸣器;对应标记不等于0时的超时计数值(ms);
extern uint8_t HollowPage_flag;	// 处于欢迎界面的标记,=1处于欢迎界面;=0进入系统工作;
extern uint8_t Select_SetHang;// 物体特征选择页面:设置内容选择;//0-4;
extern uint8_t Select_ItemMode;// 识别模式,=1,静止单识别;=2,低速单识别;=3,高速单识别;=4,三个模式相同识别;//1/2/3,4;
extern uint8_t Select_Shape[3];	// 形状特征；如值为{3,1,5},则说明分别设置静止、低速、高速模式下, 当前检测目标的形状特征为3号、1号、5号形状;
extern uint8_t Select_Color[3];	// 颜色特征；如值为{4,2,1},则说明分别设置静止、低速、高速模式下, 当前检测目标的颜色特征为4号、2号、1号颜色;
extern uint8_t Select_Background; // 背景色特征；如值为2,则说明设置检测视频的背景色为2号背景色;
// 系统时钟;
extern unsigned int System_TimeCnt;	// 系统时间1000ms计时;
extern long System_Timer;			// 当前系统时间(s);
extern long SysTime_Hour,SysTime_Minute,SysTime_Second; // 当前时钟,时/分/秒;
extern unsigned int LCD_Refreshflag,LCD_Refreshsec;	
// 屏幕原点(0,0)mm选定位置设置;
extern unsigned char Screen_OriginPosition;	// 左下角,0; 左上角,1; 右下角,2; 右上角,3;
extern long int Screen_XSizeMax;	// 屏幕实际尺寸,X轴长度,mm;
extern long int Screen_YSizeMax;	// 屏幕实际尺寸,Y轴长度,mm;
extern long int Screen_XRealOffset;	// X轴屏幕实际位置计算偏移量,mm;
extern long int Screen_YRealOffset;	// Y轴屏幕实际位置计算偏移量,mm;
extern long int Screen_XRealZero;	// X轴屏幕实际位置中心点,mm;
extern long int Screen_YRealZero;	// Y轴屏幕实际位置中心点,mm;
// 参数校准;
typedef struct
{
	long int PointaX,PointaY;			// 校准参数设定;零点(346,0)像素坐标;
	long int LASER_Axis;								// 校准参数设定;激光笔中轴与电视的距离;
	long int MatchY;							// 校准参数设定;无矫正时激光笔指向(346,360)时Y轴坐标;
	// long int SPWMzeroX,SPWMzeroY;				// 校准参数设定;舵机云台零位校准;
	long int CBMzeroX,CBMzeroY;		// 校准参数设定;坐标中心零位的磁编码值;
	unsigned char Shape,Color;			// 校准参数设定;检测目标物体_形状/颜色;//,Type   类型;;1~5/1~4;1~4;
	unsigned char Volume;		// 校准参数设定;语音音量设定;1~16;
	unsigned int LowSpeed,HighSpeed;		// 校准参数设定;运动速度;低速3~10cm/s;高速10~30cm/s;
	unsigned char RunDir;		// 校准参数设定;视频运动方向;//Dir_Leftdown=0;Dir_Rightup=1;
	long int PlotReal[50];
	long int MotorReal[50];	// 步进电机坐标点校准值存储;
}Argument;
extern Argument Arg_ReadAdj;	// 从Flash内读取的校准数据;

// LCD显示部分;
extern uint8_t Set_flag;					// 当前子功能状态;1,开机界面;2,主界面;3,时钟设置;4,手动指示;5,难度/模式选择;6,自动分拣;7,校准
extern uint8_t LCD_flag;					// 当前页面状态;1,开机界面;2,主界面;3,时钟设置;4,手动指示;5,难度/模式选择;6,自动分拣;7,物品特征选择;
extern uint8_t LCD_Data[30];			// SPI1(或UART5)发送给DCP-204A的页面更新信息;
extern uint8_t LCD_AutoEnter;			// 页面更新;从任意界面刚刚切换到LCD_flag=5页面,仅更新lcd不刷新统计信息;
extern uint8_t LCD_ReTime;				// 页面更新,时钟同步;1,向DCP-204A更新当前时钟;
extern uint8_t LCD_PicSuccess;		// 页面更新,物品检测信息打印;

// 物品检测部分;检测;
extern uint8_t Pic_Success;		// 物品检测成功状态;1,已识别到一个目标物体,正在语音播报及激光照射;
extern uint8_t Auto_Running;	// 物品检测启动状态;0,已停止物品检测;1,已启动物品检测;
extern uint8_t Auto_RunTimeflag;		// 工作计时标记;启动物品检测后置1;对应时钟计数启动;
extern uint16_t Auto_RunTimesec;		// 工作计时标记;启动物品检测后置1;对应时钟计数启动;(s)
extern uint16_t Auto_RunTimemisec;		// 工作计时标记;启动物品检测后置1;对应时钟计数启动;(ms)
//extern uint8_t StartNext_flag;			// 上一个物品检测结束标记为1;直到时间间隔过去后归0,可检测下一个物品;
//extern uint16_t StartNext_Timeout;	// 上一个物品检测结束标记为1;开始超时计数值(ms);
// 物品检测部分;信息统计;
extern uint8_t Page_Up;			// 信息统计页面;向上翻页;
extern uint8_t Page_Down;		// 信息统计页面;向下翻页;
extern uint8_t Material_Xinxi;				// 信息统计;更新至LCD显示的信息数量统计;
extern uint16_t Material_TimeOut;	// 信息统计;最新识别到的物品信息,时间点;
extern uint8_t Material_Color;				// 信息统计;最新识别到的物品信息,物品形状;
extern uint8_t Material_Shape;				// 信息统计;最新识别到的物品信息,物品颜色;
extern uint8_t Material_Number;				// 信息统计;最新识别到的物品信息,物品数量;
extern uint16_t Material_PicX,Material_PicY;	// 	信息统计;最新识别到的物品信息,物品坐标(mm);
extern uint8_t MaterialF1_Num;		// 信息统计;静止模式,识别到的物品信息数量;
extern uint8_t MaterialF2_Num;		// 信息统计;低速模式,识别到的物品信息数量;
extern uint8_t MaterialF3_Num;		// 信息统计;高速模式,识别到的物品信息数量;
//- Private variables end ---------------------------------------------------------------------------//

//- Private variables ---------------------------------------------------------------------------//

// 摇杆电位器部分;
extern unsigned char AN1_flag;				// 摇杆按键;
extern unsigned char YGHand_flag;			// 摇杆手动控制状态;标记为1,可手动控制云台转动及激光启动;
extern unsigned char YGHand_dirX;			// 摇杆手动控制状态;X轴方向;//X=0,静止;//X=1,向右;//X=2,向左;
extern unsigned char YGHand_dirY;			// 摇杆手动控制状态;Y轴方向;//Y=0,静止;//Y=1,向上;//Y=2,向下;

extern unsigned char GS_StartFlag;			// 运动视频跟随;启动跟随标志位;用于相机数据处理模式分离;
extern unsigned char Loop_Runing;				// 自动运动启动;
extern unsigned char Loop_RunHoldtime;	// 自动运动,计时;

//- Private variables end ---------------------------------------------------------------------------//



// 单模型/多模型AI识别程序-全局变量新增;//20241020新增;

extern unsigned int Camera_mode;// =0,默认单模型模式;=1,高级识别模式;

typedef struct
{
	unsigned int Flag;
	unsigned int Cnt;
	unsigned int Typelist[20];
	uint16_t pointx[20];
	uint16_t pointy[20];
	unsigned int Shape;
	unsigned int Color;
}Cam_typeRx;
extern Cam_typeRx Camera_RxType;

typedef struct
{
	unsigned int SUM;
	unsigned int List[20];
	unsigned int Listcnt[20];
	unsigned int Shape[20];
	unsigned int Color[20];
}Material_Collect;
extern Material_Collect Collect_Static;
extern Material_Collect Collect_Low;
extern Material_Collect Collect_High;

#endif





