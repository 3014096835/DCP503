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


//////// ���̰���״̬; ������̷��� -> ��Ӧ��е��ֵ;///////
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

/////////////// ��������ű�������״̬ /////////////////////
#define Err	0xff							// ����λ�ô�����;������Ϣ;,error;
#define NoErr	0x00						// ����λ�ô�����;������Ϣ;,no error;

////////////// ����ʿ��� ///////////////////////////////////////////
#define BSP_GPIOD_KIA				GPIO_Pin_1		// KIA,PD1;  
#define BSP_GPIOD_KIB				GPIO_Pin_0		// KIB,PD0;
#define Laser_On		{Laser_State = 1; GPIO_SetBits(GPIOD, BSP_GPIOD_KIA|BSP_GPIOD_KIB);}
#define Laser_Off		{Laser_State = 0; GPIO_ResetBits(GPIOD, BSP_GPIOD_KIA|BSP_GPIOD_KIB);}
	
//////////////// BUZZER; ����������; ///////////////////////////////
#define     BSP_GPIOC_BUZZER          GPIO_Pin_13  // PC13(IO7) 
#define 		BUZZER_On		GPIO_SetBits(GPIOC, BSP_GPIOC_BUZZER)
#define 		BUZZER_Off	GPIO_ResetBits(GPIOC, BSP_GPIOC_BUZZER)

//////////////////////////// �������=0; ��̨����;////////////////////////////////////////
// ���������;
#define       GPIOE_MOTPUL01          	GPIO_Pin_12  	// MOT1 ���������;JH1_ PE1; => JI1_ PE12;
#define       GPIOE_MOTPUL02          	GPIO_Pin_11  	// MOT2 ���������;JH4_ PE4; => JI4_ PE11;
// ʹ�ܶ�;
#define       GPIOE_MOTEN01          	GPIO_Pin_9  		// MOT1 ʹ�ܶ�;JH2_ PE2; => JI2_ PE9;
#define       GPIOE_MOTEN02          	GPIO_Pin_10   	// MOT2 ʹ�ܶ�;JH5_ PE5; => JI5_ PE10;
// ������ƶ�;
#define       GPIOE_MOTDIR01          	GPIO_Pin_14  	// MOT1 ������ƶ�;//JH0_ PE0; => JI0_ PE14;
#define       GPIOE_MOTDIR02          	GPIO_Pin_8  	// MOT2 ������ƶ�;//JH3_ PE3; => JI3_ PE8;

// Motor_1,�������;
#define 	MOTX01_SetEn_LI			GPIO_ResetBits(GPIOE, GPIOE_MOTEN01)
#define 	MOTX01_SetEn_HI			GPIO_SetBits(GPIOE, GPIOE_MOTEN01)
#define 	MOTX01_SetDir_HI			GPIO_ResetBits(GPIOE, GPIOE_MOTDIR01)
#define 	MOTX01_SetDir_LI			GPIO_SetBits(GPIOE, GPIOE_MOTDIR01)
// Motor_2,�������;
#define 	MOTY02_SetEn_LI			GPIO_ResetBits(GPIOE, GPIOE_MOTEN02)
#define 	MOTY02_SetEn_HI			GPIO_SetBits(GPIOE, GPIOE_MOTEN02)
#define 	MOTY02_SetDir_HI			GPIO_ResetBits(GPIOE, GPIOE_MOTDIR02)
#define 	MOTY02_SetDir_LI			GPIO_SetBits(GPIOE, GPIOE_MOTDIR02)
// ��������ߵ�ƽ,"1";
#define MOTX01_SetPul_HI		GPIO_SetBits(GPIOE, GPIOE_MOTPUL01)		// MOT1 ���������;JI5;// PE10;
#define MOTY02_SetPul_HI		GPIO_SetBits(GPIOE, GPIOE_MOTPUL02)			// MOT2 ���������;JH5;// PE5;
// ��������͵�ƽ,"0";
#define MOTX01_SetPul_LI		GPIO_ResetBits(GPIOE, GPIOE_MOTPUL01)	// MOT1 ���������;JI5;// PE10;
#define MOTY02_SetPul_LI		GPIO_ResetBits(GPIOE, GPIOE_MOTPUL02)		// MOT2 ���������;JH5;// PE5;
////////////////////////////// �������=0; ��̨����;////////////////////////////////////////

/////////////////////// �������=0; ��̨�����ٶ�; //////////////////////////////////////////
#define MOT_SetSpeed_HI			TIM_SetAutoreload(TIM2,500);TIM_SetAutoreload(TIM3,500);  // ��ʱ���Ƚ�ֵ;
#define MOT_SetSpeed_LI			TIM_SetAutoreload(TIM2,1000);TIM_SetAutoreload(TIM3,1000);  // ��ʱ���Ƚ�ֵ;




/* Private define end ------------------------------------------------------------*/




/* Private variables ----------------------------------------------------------------------------------*/


// ����;
extern unsigned char	Presskey_val;// ��������;
extern unsigned char	Matrixkey_val;// ���󰴼�;


// ��άϵͳ����;(MCGS ������)// RS485����;������˫��ͨѶ����;
extern uint8_t  Rece_Time;				// ���ճ�ʱʱ��;
extern uint8_t	Rece_TimeOut;			// ���ճ�ʱ��־;=1,δ��ʱ;=0,��ʱ;
extern uint8_t  Rece_Buffer[50];  // ֱ�ӴӴ����ж��еõ�������
extern uint8_t  Rece_RxCounter;   // ��ʾ����USART1���յ������ݸ���
extern uint8_t  Rece_bit;					// =1,��ʾ�н��յ���λ����;
extern uint8_t  MCGS_Sendform;		// ������0ʱ; ����,���������Ͷ�Ӧ��ʽ������;
extern uint8_t	MCGS_Message;				// MCGS��Ϣ������ű��;
extern uint16_t Send_CRC ;					// ������������ݵ�CRCУ��ֵ
extern uint8_t  MCGS_TxBuffer[50];	// ��Ӧ��MCGS���͵�������Ϣ����ͬʱ�Ǳ��ؿ�����ת�����ݵ�����
// ���ʹ���Ƶ����;
#define  Dir_Leftdown 	0							// ��Ƶ�����˶�����;����;
#define  Dir_Rightup 		1							// ��Ƶ�����˶�����;����;
#define  Video_Pause 	0x01 						// ��Ƶ��ͣ;Pause/0x01;
#define  Video_Play 	0x02 						// ��Ƶ����;Play/0x02;
extern uint8_t  Video_HandControl;		// �ֶ�������Ƶ���Ż���ͣ״̬;Play/Pause;
extern uint8_t  Video_State;					// ��Ƶ���Ż���ͣ״̬;Play/Pause;
extern uint8_t  Video_Mode;						// ��Ƶ����ģʽ;��ֹ,����,����;
extern uint8_t  Video_timeflag;				// ������ͣ��Ƶ��ָ�����Ϊ1,��ʱ����;���Ͳ���ָ�������0;
extern uint16_t Video_timesec;				// ������ͣ��Ƶ��ָ�����Ϊ1,��ʱ����ֵ(s);
extern uint16_t Video_timemisec;			// ������ͣ��Ƶ��ָ�����Ϊ1,��ʱ����ֵ(s);
extern uint8_t Video_TxSend[5];				// ���ڷ��ͻ�����;
// ���������;(UART4)
extern uint8_t Cam_TxBuffer[20];		// ����˿ڷ������ݻ����� 
extern uint8_t Cam_RxBuffer[200];	// ����˿ڽ������ݻ����� 
extern uint16_t Cam_RxNum;							// ������յ���Ч���ݵ����ݸ���;
extern uint8_t Cam_Rxflag;						// W5500���յ���Ч���ݱ��Ϊ1;
extern uint8_t USB_BeatNUM;
// ���������;(W5500)
extern uint16_t W5500_Send_Delay_Counter; //W5500������ʱ��������(ms)
//extern uint8_t W5500_Rxflag;				// W5500���յ���Ч���ݱ��Ϊ1;
//extern uint16_t W5500_RxNum;				// W5500���յ���Ч���ݵ����ݸ���;
// ���ⲿ��;
extern uint8_t Laser_State;					// ����ʿ���,����״̬;
extern uint8_t Laser_OpenFlag;			// ����ʿ���,�Զ�ָʾʱ,����������Ϊ1,��ʱ��������Timeout;
extern uint16_t Laser_OpenTimeout;	// ����ʿ���,�Զ�ָʾʱ,����������Ϊ1,��ʱ����ֵ(ms);


// ��λ����;
extern long int Laser_ScreenX,Laser_ScreenY;					// X,Y. Point;������ĻĿ��λ��;��Ļ����,(mm)
extern long int Laser_ScrnSaveX,Laser_ScrnSaveY;			// X,Y. Point;������ĻĿ��λ��;��Ļ����,(mm)
extern long int Run_Xmotor,Run_Ymotor;								// X,Y. Point;������ĻĿ��λ��;���Ŀ���ƶ�λ��;����������,(Pluse);
extern uint16_t MOT_DirX,MOT_DirY;										// X,Y. Motor Dir: 0,ֹͣ; 1,ǰ��; 2,����;
extern long int Run_Xnum,Run_Ynum;										// X,Y. Pluse Count;�����ǰ�ƶ�λ��;����������,(Pluse);
extern long int Obj_DataX,Obj_DataY;									// X,Y. object-Point;�����ǰĿ���ƶ�λ��;ת������Ļ����,(mm);
extern uint16_t C_shiftflag;						// ��̨���Ƶ�����;//=0,����ƫ�Ʊ��;//=1,��ƫ��;
extern uint16_t Hand_MotorAdd;					// ��̨�ֶ��������;

// ����λ�ô�����;
typedef struct
{
	long RealRead;  // ��ǰ�ű�������ֵ;
	long RealOld;  // ��ǰ�ű�������ֵ;
	long AddRead;   // ��ǰ�ű�����������;
	long AddSet;  	// ��ǰ�ű�������Ŀ������;
	long RealSet;  	// ��ǰ�ű�������Ŀ����ֵ;
	long InitData;   	// �ű�����λֵ;
	unsigned char Error;  // �ű������״̬;
	unsigned char Compenflag;   // ��ǰ�ű�������ֵ;��������;
}CBM_Code;
extern CBM_Code CodeX;// ����λ��������Ϣ;
extern CBM_Code CodeY;// ����λ��������Ϣ;
extern uint8_t IIC_GetInit;// ����λ�ýǶȳ�ʼ�����;
// ��̨У׼����;
#define 	USE_JiaoZhun_Code				0				// =0,Ĭ��ʹ�õ���ű��뷴��У׼;
#define 	USE_JiaoZhun_Motor			1				// =1,ѡ��ʹ�ò����������У׼;
extern unsigned char PTZ_JiaoZhun_USE;		// У׼��ʽģʽѡ��; // =0,Ĭ��ʹ�õ���ű��뷴��У׼; =1,ѡ��ʹ�ò����������У׼;


// ϵͳ��������;
extern uint8_t System_flag;			// ϵͳ��ʼ���ɹ�,������������Ϊ1;
extern uint8_t BUZZER_flag;			// ������;���ñ��Ϊ1/3/5; 1:��һ��1s; 3:��һ��0.5s; 5:������0.5s; 
extern uint16_t BUZZER_Count;		// ������;��Ӧ��ǲ�����0ʱ�ĳ�ʱ����ֵ(ms);
extern uint8_t HollowPage_flag;	// ���ڻ�ӭ����ı��,=1���ڻ�ӭ����;=0����ϵͳ����;
extern uint8_t Select_SetHang;// ��������ѡ��ҳ��:��������ѡ��;//0-4;
extern uint8_t Select_ItemMode;// ʶ��ģʽ,=1,��ֹ��ʶ��;=2,���ٵ�ʶ��;=3,���ٵ�ʶ��;=4,����ģʽ��ͬʶ��;//1/2/3,4;
extern uint8_t Select_Shape[3];	// ��״��������ֵΪ{3,1,5},��˵���ֱ����þ�ֹ�����١�����ģʽ��, ��ǰ���Ŀ�����״����Ϊ3�š�1�š�5����״;
extern uint8_t Select_Color[3];	// ��ɫ��������ֵΪ{4,2,1},��˵���ֱ����þ�ֹ�����١�����ģʽ��, ��ǰ���Ŀ�����ɫ����Ϊ4�š�2�š�1����ɫ;
extern uint8_t Select_Background; // ����ɫ��������ֵΪ2,��˵�����ü����Ƶ�ı���ɫΪ2�ű���ɫ;
// ϵͳʱ��;
extern unsigned int System_TimeCnt;	// ϵͳʱ��1000ms��ʱ;
extern long System_Timer;			// ��ǰϵͳʱ��(s);
extern long SysTime_Hour,SysTime_Minute,SysTime_Second; // ��ǰʱ��,ʱ/��/��;
extern unsigned int LCD_Refreshflag,LCD_Refreshsec;	
// ��Ļԭ��(0,0)mmѡ��λ������;
extern unsigned char Screen_OriginPosition;	// ���½�,0; ���Ͻ�,1; ���½�,2; ���Ͻ�,3;
extern long int Screen_XSizeMax;	// ��Ļʵ�ʳߴ�,X�᳤��,mm;
extern long int Screen_YSizeMax;	// ��Ļʵ�ʳߴ�,Y�᳤��,mm;
extern long int Screen_XRealOffset;	// X����Ļʵ��λ�ü���ƫ����,mm;
extern long int Screen_YRealOffset;	// Y����Ļʵ��λ�ü���ƫ����,mm;
extern long int Screen_XRealZero;	// X����Ļʵ��λ�����ĵ�,mm;
extern long int Screen_YRealZero;	// Y����Ļʵ��λ�����ĵ�,mm;
// ����У׼;
typedef struct
{
	long int PointaX,PointaY;			// У׼�����趨;���(346,0)��������;
	long int LASER_Axis;								// У׼�����趨;�������������ӵľ���;
	long int MatchY;							// У׼�����趨;�޽���ʱ�����ָ��(346,360)ʱY������;
	// long int SPWMzeroX,SPWMzeroY;				// У׼�����趨;�����̨��λУ׼;
	long int CBMzeroX,CBMzeroY;		// У׼�����趨;����������λ�Ĵű���ֵ;
	unsigned char Shape,Color;			// У׼�����趨;���Ŀ������_��״/��ɫ;//,Type   ����;;1~5/1~4;1~4;
	unsigned char Volume;		// У׼�����趨;���������趨;1~16;
	unsigned int LowSpeed,HighSpeed;		// У׼�����趨;�˶��ٶ�;����3~10cm/s;����10~30cm/s;
	unsigned char RunDir;		// У׼�����趨;��Ƶ�˶�����;//Dir_Leftdown=0;Dir_Rightup=1;
	long int PlotReal[50];
	long int MotorReal[50];	// ������������У׼ֵ�洢;
}Argument;
extern Argument Arg_ReadAdj;	// ��Flash�ڶ�ȡ��У׼����;

// LCD��ʾ����;
extern uint8_t Set_flag;					// ��ǰ�ӹ���״̬;1,��������;2,������;3,ʱ������;4,�ֶ�ָʾ;5,�Ѷ�/ģʽѡ��;6,�Զ��ּ�;7,У׼
extern uint8_t LCD_flag;					// ��ǰҳ��״̬;1,��������;2,������;3,ʱ������;4,�ֶ�ָʾ;5,�Ѷ�/ģʽѡ��;6,�Զ��ּ�;7,��Ʒ����ѡ��;
extern uint8_t LCD_Data[30];			// SPI1(��UART5)���͸�DCP-204A��ҳ�������Ϣ;
extern uint8_t LCD_AutoEnter;			// ҳ�����;���������ո��л���LCD_flag=5ҳ��,������lcd��ˢ��ͳ����Ϣ;
extern uint8_t LCD_ReTime;				// ҳ�����,ʱ��ͬ��;1,��DCP-204A���µ�ǰʱ��;
extern uint8_t LCD_PicSuccess;		// ҳ�����,��Ʒ�����Ϣ��ӡ;

// ��Ʒ��ⲿ��;���;
extern uint8_t Pic_Success;		// ��Ʒ���ɹ�״̬;1,��ʶ��һ��Ŀ������,����������������������;
extern uint8_t Auto_Running;	// ��Ʒ�������״̬;0,��ֹͣ��Ʒ���;1,��������Ʒ���;
extern uint8_t Auto_RunTimeflag;		// ������ʱ���;������Ʒ������1;��Ӧʱ�Ӽ�������;
extern uint16_t Auto_RunTimesec;		// ������ʱ���;������Ʒ������1;��Ӧʱ�Ӽ�������;(s)
extern uint16_t Auto_RunTimemisec;		// ������ʱ���;������Ʒ������1;��Ӧʱ�Ӽ�������;(ms)
//extern uint8_t StartNext_flag;			// ��һ����Ʒ���������Ϊ1;ֱ��ʱ������ȥ���0,�ɼ����һ����Ʒ;
//extern uint16_t StartNext_Timeout;	// ��һ����Ʒ���������Ϊ1;��ʼ��ʱ����ֵ(ms);
// ��Ʒ��ⲿ��;��Ϣͳ��;
extern uint8_t Page_Up;			// ��Ϣͳ��ҳ��;���Ϸ�ҳ;
extern uint8_t Page_Down;		// ��Ϣͳ��ҳ��;���·�ҳ;
extern uint8_t Material_Xinxi;				// ��Ϣͳ��;������LCD��ʾ����Ϣ����ͳ��;
extern uint16_t Material_TimeOut;	// ��Ϣͳ��;����ʶ�𵽵���Ʒ��Ϣ,ʱ���;
extern uint8_t Material_Color;				// ��Ϣͳ��;����ʶ�𵽵���Ʒ��Ϣ,��Ʒ��״;
extern uint8_t Material_Shape;				// ��Ϣͳ��;����ʶ�𵽵���Ʒ��Ϣ,��Ʒ��ɫ;
extern uint8_t Material_Number;				// ��Ϣͳ��;����ʶ�𵽵���Ʒ��Ϣ,��Ʒ����;
extern uint16_t Material_PicX,Material_PicY;	// 	��Ϣͳ��;����ʶ�𵽵���Ʒ��Ϣ,��Ʒ����(mm);
extern uint8_t MaterialF1_Num;		// ��Ϣͳ��;��ֹģʽ,ʶ�𵽵���Ʒ��Ϣ����;
extern uint8_t MaterialF2_Num;		// ��Ϣͳ��;����ģʽ,ʶ�𵽵���Ʒ��Ϣ����;
extern uint8_t MaterialF3_Num;		// ��Ϣͳ��;����ģʽ,ʶ�𵽵���Ʒ��Ϣ����;
//- Private variables end ---------------------------------------------------------------------------//

//- Private variables ---------------------------------------------------------------------------//

// ҡ�˵�λ������;
extern unsigned char AN1_flag;				// ҡ�˰���;
extern unsigned char YGHand_flag;			// ҡ���ֶ�����״̬;���Ϊ1,���ֶ�������̨ת������������;
extern unsigned char YGHand_dirX;			// ҡ���ֶ�����״̬;X�᷽��;//X=0,��ֹ;//X=1,����;//X=2,����;
extern unsigned char YGHand_dirY;			// ҡ���ֶ�����״̬;Y�᷽��;//Y=0,��ֹ;//Y=1,����;//Y=2,����;

extern unsigned char GS_StartFlag;			// �˶���Ƶ����;���������־λ;����������ݴ���ģʽ����;
extern unsigned char Loop_Runing;				// �Զ��˶�����;
extern unsigned char Loop_RunHoldtime;	// �Զ��˶�,��ʱ;

//- Private variables end ---------------------------------------------------------------------------//



// ��ģ��/��ģ��AIʶ�����-ȫ�ֱ�������;//20241020����;

extern unsigned int Camera_mode;// =0,Ĭ�ϵ�ģ��ģʽ;=1,�߼�ʶ��ģʽ;

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





