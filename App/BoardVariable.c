#include  "stm32f10x.h"
#include  "stm32f10x_conf.h" 
#include  "BoardVariable.h"



// Private variables ---------------------------------------------------------//
// Systick delay
__IO uint32_t TimingDelay = 0 ;


// 键盘;
unsigned char	Presskey_val=0;	// 独立按键;
unsigned char	Matrixkey_val=0;// 矩阵按键;

// 运维系统部分;(MCGS 触摸屏)// RS485接收;触摸屏双向通讯控制;
uint8_t  Rece_Time = 0;			// 接收超时时间;
uint8_t	 Rece_TimeOut;			// 接收超时标志;=1,未超时;=0,超时;
uint8_t  Rece_Buffer[50];   // 直接从串口中断中得到的数据
uint8_t  Rece_RxCounter;    // 表示串口USART1接收到的数据个数
uint8_t  Rece_bit;					// =1,表示有接收到首位数据;
uint8_t  MCGS_Sendform=0;		// 不等于0时; 启动,向触摸屏发送对应格式的数据;
uint8_t  MCGS_Message=0;			// MCGS信息数据序号编号;
uint16_t Send_CRC;						// 计算待发送数据的CRC校验值
uint8_t  MCGS_TxBuffer[50];		// 响应“MCGS发送的请求信息”，同时是本地控制器转发数据的数组
// 传送带视频部分;
uint8_t Video_HandControl=0;	// 手动控制视频播放或暂停状态;Play/Pause;
uint8_t Video_State;					// 视频播放或暂停状态;Play/Pause;
uint8_t Video_Mode=0;					// 视频播放模式;静止,低速,高速;
uint8_t Video_timeflag=0;			// 发送暂停视频的指令后标记为1,超时计数;发送播放指令后标记清0;
uint16_t Video_timesec=0;			// 发送暂停视频的指令后标记为1,超时计数值(s);
uint16_t Video_timemisec=0;		// 发送暂停视频的指令后标记为1,超时计数值(ms);
uint8_t Video_TxSend[5]={0};	// 串口发送缓存区;
// 摄像机部分;(UART4)
uint8_t Cam_TxBuffer[20]={0};		// 相机端口发送数据缓冲区 
uint8_t Cam_RxBuffer[200]={0};	// 相机端口接收数据缓冲区 
uint16_t Cam_RxNum;							// 相机接收到有效数据的数据个数;
uint8_t Cam_Rxflag=0;						// W5500接收到有效数据标记为1;
uint8_t USB_BeatNUM=0;
// 摄像机部分;(W5500)
uint16_t W5500_Send_Delay_Counter=0; //W5500发送延时计数变量(ms)
//uint8_t W5500_Rxflag=0;				// W5500接收到有效数据标记为1;
//uint16_t W5500_RxNum;					// W5500接收到有效数据的数据个数;
// 激光部分;
uint8_t Laser_State=0;				// 激光笔控制,亮灭状态;
uint8_t Laser_OpenFlag=0;			// 激光笔控制,自动指示时,启动激光标记为1,超时计数配套Timeout;
uint16_t Laser_OpenTimeout=0;	// 激光笔控制,自动指示时,启动激光标记为1,超时计数值(ms);

// 定位部分;
long int Laser_ScreenX,Laser_ScreenY;		// X,Y. Point;设置屏幕目标位置;屏幕坐标,(mm)
long int Laser_ScrnSaveX,Laser_ScrnSaveY;		// X,Y. Point;设置屏幕目标位置;屏幕坐标,(mm)
long int Run_Xmotor,Run_Ymotor;					// X,Y. Point;设置屏幕目标位置;电机目标移动位置;脉冲数坐标,(Pluse);
uint16_t MOT_DirX,MOT_DirY;			// X,Y. Motor Dir: 0,停止; 1,前进; 2,后退;
long int Run_Xnum,Run_Ynum;			// X,Y. Pluse Count;电机当前移动位置;脉冲数坐标,(Pluse);
long int Obj_DataX,Obj_DataY;		// X,Y. object-Point;电机当前目标移动位置;转换的屏幕坐标,(mm);
uint16_t C_shiftflag=0;			// 云台控制叠加量;//=0,不做偏移标记;//=1,做偏移;
uint16_t Hand_MotorAdd=16;	// 云台手动调整间隔;
//uint8_t NODeal_Runing=0;

// 绝对位置传感器;
CBM_Code CodeX;// 绝对位置数据信息;
CBM_Code CodeY;// 绝对位置数据信息;
uint8_t IIC_GetInit=0;// 绝对位置角度初始化标记;
// 云台校准方案;
unsigned char PTZ_JiaoZhun_USE=0;		// 校准方式模式选择; // =0,默认使用电机磁编码反馈校准; =1,选择使用步进电机脉冲校准;


// 系统其他参数;
uint8_t System_flag=0;			// 系统初始化成功,正常运行则标记为1;
uint8_t BUZZER_flag=0;			// 蜂鸣器;设置标记为1/3/5; 1:响一声1s; 3:响一声0.5s; 5:响两声0.5s; 
uint16_t BUZZER_Count=0;		// 蜂鸣器;对应标记不等于0时的超时计数值(ms);
// 系统时钟;
unsigned int System_TimeCnt=0;	// 系统时间1000ms计时;
long System_Timer=0;			// 当前系统时间(s);
long SysTime_Hour,SysTime_Minute,SysTime_Second; // 当前时钟,时/分/秒;
unsigned int LCD_Refreshflag,LCD_Refreshsec;	
// 屏幕原点(0,0)mm选定位置设置;
unsigned char Screen_OriginPosition=0;	// 左下角,0; 左上角,1; 右下角,2; 右上角,3;
long int Screen_XSizeMax=694;	// 屏幕实际尺寸,X轴长度,mm;
long int Screen_YSizeMax=392;	// 屏幕实际尺寸,Y轴长度,mm;
long int Screen_XRealOffset=0;	// X轴屏幕实际位置计算偏移量,mm;
long int Screen_YRealOffset=0;	// Y轴屏幕实际位置计算偏移量,mm;
long int Screen_XRealZero=0;	// X轴屏幕实际位置中心点,mm;
long int Screen_YRealZero=0;	// Y轴屏幕实际位置中心点,mm;
// 参数校准;
Argument Arg_ReadAdj;	// 从Flash内读取的校准数据;



// LCD显示部分;
uint8_t Set_flag=0;			// 当前子功能状态;1,开机界面;2,主界面;3,时钟设置;4,手动指示;5,难度/模式选择;6,自动分拣;7,校准
uint8_t LCD_flag=0;			// 当前页面状态;1,开机界面;2,主界面;3,时钟设置;4,手动指示;5,难度/模式选择;6,自动分拣;7,物品特征选择;
uint8_t LCD_Data[30]={0};	// SPI1(或USART2)发送给DCP-204A的页面更新信息;
uint8_t LCD_AutoEnter;		// 页面更新;从任意界面刚刚切换到LCD_flag=5页面,仅更新lcd不刷新统计信息;
uint8_t LCD_ReTime;				// 页面更新,时钟同步;1,向DCP-204A更新当前时钟;
uint8_t LCD_PicSuccess;		// 页面更新,物品检测信息打印;

// 物品检测部分;检测;
uint8_t Pic_Success=0;				// 物品检测成功状态;1,已识别到一个目标物体,正在语音播报及激光照射;
uint8_t Auto_Running=0;				// 物品检测启动状态;0,已停止物品检测;1,已启动物品检测;
uint8_t Auto_RunTimeflag=0;		// 工作计时标记;启动物品检测后置1;对应时钟计数启动;
uint16_t Auto_RunTimesec;			// 工作计时标记;启动物品检测后置1;对应时钟计数启动;(s)
uint16_t Auto_RunTimemisec;			// 工作计时标记;启动物品检测后置1;对应时钟计数启动;(ms)
//uint8_t StartNext_flag=0;			// 上一个物品检测结束标记为1;直到时间间隔过去后归0,可检测下一个物品;
//uint16_t StartNext_Timeout=0;		// 上一个物品检测结束标记为1;开始超时计数值(ms);

// 物品检测部分;信息统计;
uint8_t Page_Up=0;			// 信息统计页面;向上翻页;
uint8_t Page_Down=0;		// 信息统计页面;向下翻页;
uint8_t Material_Xinxi=0;					// 信息统计;更新至LCD显示的信息数量统计;
uint16_t Material_TimeOut=0;	// 信息统计;最新识别到的物品信息,时间点;
uint8_t Material_Color=0;					// 信息统计;最新识别到的物品信息,物品形状;
uint8_t Material_Shape=0;					// 信息统计;最新识别到的物品信息,物品颜色;
uint8_t Material_Number=0;				// 信息统计;最新识别到的物品信息,物品数量;
uint16_t Material_PicX,Material_PicY;	// 信息统计;最新识别到的物品信息,物品坐标(mm);
uint8_t MaterialF1_Num=0;			// 信息统计;静止模式,识别到的物品信息数量;
uint8_t MaterialF2_Num=0;			// 信息统计;低速模式,识别到的物品信息数量;
uint8_t MaterialF3_Num=0;			// 信息统计;高速模式,识别到的物品信息数量;
// Private variables end ---------------------------------------------------------//

// Private variables ---------------------------------------------------------//
// 摇杆电位器部分;
unsigned char YGHand_flag=0;		// 摇杆手动控制状态;标记为1,可手动控制云台转动及激光启动;
unsigned char YGHand_dirX=0;		// 摇杆手动控制状态;X轴方向;//X=0,静止;//X=1,向右;//X=2,向左;
unsigned char YGHand_dirY=0;		// 摇杆手动控制状态;Y轴方向;//Y=0,静止;//Y=1,向上;//Y=2,向下;
unsigned char AN1_flag=0;				// 摇杆按键;
unsigned char GS_StartFlag=0;			// 运动视频跟随;启动跟随标志位;用于相机数据处理模式分离;
unsigned char Loop_Runing=0;				// 自动运动启动;
unsigned char Loop_RunHoldtime=0;		// 自动运动,计时;

// Private variables end ---------------------------------------------------------//


// 单模型/多模型AI识别程序-全局变量新增;//20241020新增;

unsigned int Camera_mode=0;// =0,默认单模型模式;=1,高级识别模式;
Cam_typeRx Camera_RxType;
Material_Collect Collect_Static;
Material_Collect Collect_Low;
Material_Collect Collect_High;



