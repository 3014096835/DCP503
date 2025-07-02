#include  "stm32f10x.h"
#include  "stm32f10x_conf.h" 
#include  "BoardVariable.h"



// Private variables ---------------------------------------------------------//
// Systick delay
__IO uint32_t TimingDelay = 0 ;


// ����;
unsigned char	Presskey_val=0;	// ��������;
unsigned char	Matrixkey_val=0;// ���󰴼�;

// ��άϵͳ����;(MCGS ������)// RS485����;������˫��ͨѶ����;
uint8_t  Rece_Time = 0;			// ���ճ�ʱʱ��;
uint8_t	 Rece_TimeOut;			// ���ճ�ʱ��־;=1,δ��ʱ;=0,��ʱ;
uint8_t  Rece_Buffer[50];   // ֱ�ӴӴ����ж��еõ�������
uint8_t  Rece_RxCounter;    // ��ʾ����USART1���յ������ݸ���
uint8_t  Rece_bit;					// =1,��ʾ�н��յ���λ����;
uint8_t  MCGS_Sendform=0;		// ������0ʱ; ����,���������Ͷ�Ӧ��ʽ������;
uint8_t  MCGS_Message=0;			// MCGS��Ϣ������ű��;
uint16_t Send_CRC;						// ������������ݵ�CRCУ��ֵ
uint8_t  MCGS_TxBuffer[50];		// ��Ӧ��MCGS���͵�������Ϣ����ͬʱ�Ǳ��ؿ�����ת�����ݵ�����
// ���ʹ���Ƶ����;
uint8_t Video_HandControl=0;	// �ֶ�������Ƶ���Ż���ͣ״̬;Play/Pause;
uint8_t Video_State;					// ��Ƶ���Ż���ͣ״̬;Play/Pause;
uint8_t Video_Mode=0;					// ��Ƶ����ģʽ;��ֹ,����,����;
uint8_t Video_timeflag=0;			// ������ͣ��Ƶ��ָ�����Ϊ1,��ʱ����;���Ͳ���ָ�������0;
uint16_t Video_timesec=0;			// ������ͣ��Ƶ��ָ�����Ϊ1,��ʱ����ֵ(s);
uint16_t Video_timemisec=0;		// ������ͣ��Ƶ��ָ�����Ϊ1,��ʱ����ֵ(ms);
uint8_t Video_TxSend[5]={0};	// ���ڷ��ͻ�����;
// ���������;(UART4)
uint8_t Cam_TxBuffer[20]={0};		// ����˿ڷ������ݻ����� 
uint8_t Cam_RxBuffer[200]={0};	// ����˿ڽ������ݻ����� 
uint16_t Cam_RxNum;							// ������յ���Ч���ݵ����ݸ���;
uint8_t Cam_Rxflag=0;						// W5500���յ���Ч���ݱ��Ϊ1;
uint8_t USB_BeatNUM=0;
// ���������;(W5500)
uint16_t W5500_Send_Delay_Counter=0; //W5500������ʱ��������(ms)
//uint8_t W5500_Rxflag=0;				// W5500���յ���Ч���ݱ��Ϊ1;
//uint16_t W5500_RxNum;					// W5500���յ���Ч���ݵ����ݸ���;
// ���ⲿ��;
uint8_t Laser_State=0;				// ����ʿ���,����״̬;
uint8_t Laser_OpenFlag=0;			// ����ʿ���,�Զ�ָʾʱ,����������Ϊ1,��ʱ��������Timeout;
uint16_t Laser_OpenTimeout=0;	// ����ʿ���,�Զ�ָʾʱ,����������Ϊ1,��ʱ����ֵ(ms);

// ��λ����;
long int Laser_ScreenX,Laser_ScreenY;		// X,Y. Point;������ĻĿ��λ��;��Ļ����,(mm)
long int Laser_ScrnSaveX,Laser_ScrnSaveY;		// X,Y. Point;������ĻĿ��λ��;��Ļ����,(mm)
long int Run_Xmotor,Run_Ymotor;					// X,Y. Point;������ĻĿ��λ��;���Ŀ���ƶ�λ��;����������,(Pluse);
uint16_t MOT_DirX,MOT_DirY;			// X,Y. Motor Dir: 0,ֹͣ; 1,ǰ��; 2,����;
long int Run_Xnum,Run_Ynum;			// X,Y. Pluse Count;�����ǰ�ƶ�λ��;����������,(Pluse);
long int Obj_DataX,Obj_DataY;		// X,Y. object-Point;�����ǰĿ���ƶ�λ��;ת������Ļ����,(mm);
uint16_t C_shiftflag=0;			// ��̨���Ƶ�����;//=0,����ƫ�Ʊ��;//=1,��ƫ��;
uint16_t Hand_MotorAdd=16;	// ��̨�ֶ��������;
//uint8_t NODeal_Runing=0;

// ����λ�ô�����;
CBM_Code CodeX;// ����λ��������Ϣ;
CBM_Code CodeY;// ����λ��������Ϣ;
uint8_t IIC_GetInit=0;// ����λ�ýǶȳ�ʼ�����;
// ��̨У׼����;
unsigned char PTZ_JiaoZhun_USE=0;		// У׼��ʽģʽѡ��; // =0,Ĭ��ʹ�õ���ű��뷴��У׼; =1,ѡ��ʹ�ò����������У׼;


// ϵͳ��������;
uint8_t System_flag=0;			// ϵͳ��ʼ���ɹ�,������������Ϊ1;
uint8_t BUZZER_flag=0;			// ������;���ñ��Ϊ1/3/5; 1:��һ��1s; 3:��һ��0.5s; 5:������0.5s; 
uint16_t BUZZER_Count=0;		// ������;��Ӧ��ǲ�����0ʱ�ĳ�ʱ����ֵ(ms);
// ϵͳʱ��;
unsigned int System_TimeCnt=0;	// ϵͳʱ��1000ms��ʱ;
long System_Timer=0;			// ��ǰϵͳʱ��(s);
long SysTime_Hour,SysTime_Minute,SysTime_Second; // ��ǰʱ��,ʱ/��/��;
unsigned int LCD_Refreshflag,LCD_Refreshsec;	
// ��Ļԭ��(0,0)mmѡ��λ������;
unsigned char Screen_OriginPosition=0;	// ���½�,0; ���Ͻ�,1; ���½�,2; ���Ͻ�,3;
long int Screen_XSizeMax=694;	// ��Ļʵ�ʳߴ�,X�᳤��,mm;
long int Screen_YSizeMax=392;	// ��Ļʵ�ʳߴ�,Y�᳤��,mm;
long int Screen_XRealOffset=0;	// X����Ļʵ��λ�ü���ƫ����,mm;
long int Screen_YRealOffset=0;	// Y����Ļʵ��λ�ü���ƫ����,mm;
long int Screen_XRealZero=0;	// X����Ļʵ��λ�����ĵ�,mm;
long int Screen_YRealZero=0;	// Y����Ļʵ��λ�����ĵ�,mm;
// ����У׼;
Argument Arg_ReadAdj;	// ��Flash�ڶ�ȡ��У׼����;



// LCD��ʾ����;
uint8_t Set_flag=0;			// ��ǰ�ӹ���״̬;1,��������;2,������;3,ʱ������;4,�ֶ�ָʾ;5,�Ѷ�/ģʽѡ��;6,�Զ��ּ�;7,У׼
uint8_t LCD_flag=0;			// ��ǰҳ��״̬;1,��������;2,������;3,ʱ������;4,�ֶ�ָʾ;5,�Ѷ�/ģʽѡ��;6,�Զ��ּ�;7,��Ʒ����ѡ��;
uint8_t LCD_Data[30]={0};	// SPI1(��USART2)���͸�DCP-204A��ҳ�������Ϣ;
uint8_t LCD_AutoEnter;		// ҳ�����;���������ո��л���LCD_flag=5ҳ��,������lcd��ˢ��ͳ����Ϣ;
uint8_t LCD_ReTime;				// ҳ�����,ʱ��ͬ��;1,��DCP-204A���µ�ǰʱ��;
uint8_t LCD_PicSuccess;		// ҳ�����,��Ʒ�����Ϣ��ӡ;

// ��Ʒ��ⲿ��;���;
uint8_t Pic_Success=0;				// ��Ʒ���ɹ�״̬;1,��ʶ��һ��Ŀ������,����������������������;
uint8_t Auto_Running=0;				// ��Ʒ�������״̬;0,��ֹͣ��Ʒ���;1,��������Ʒ���;
uint8_t Auto_RunTimeflag=0;		// ������ʱ���;������Ʒ������1;��Ӧʱ�Ӽ�������;
uint16_t Auto_RunTimesec;			// ������ʱ���;������Ʒ������1;��Ӧʱ�Ӽ�������;(s)
uint16_t Auto_RunTimemisec;			// ������ʱ���;������Ʒ������1;��Ӧʱ�Ӽ�������;(ms)
//uint8_t StartNext_flag=0;			// ��һ����Ʒ���������Ϊ1;ֱ��ʱ������ȥ���0,�ɼ����һ����Ʒ;
//uint16_t StartNext_Timeout=0;		// ��һ����Ʒ���������Ϊ1;��ʼ��ʱ����ֵ(ms);

// ��Ʒ��ⲿ��;��Ϣͳ��;
uint8_t Page_Up=0;			// ��Ϣͳ��ҳ��;���Ϸ�ҳ;
uint8_t Page_Down=0;		// ��Ϣͳ��ҳ��;���·�ҳ;
uint8_t Material_Xinxi=0;					// ��Ϣͳ��;������LCD��ʾ����Ϣ����ͳ��;
uint16_t Material_TimeOut=0;	// ��Ϣͳ��;����ʶ�𵽵���Ʒ��Ϣ,ʱ���;
uint8_t Material_Color=0;					// ��Ϣͳ��;����ʶ�𵽵���Ʒ��Ϣ,��Ʒ��״;
uint8_t Material_Shape=0;					// ��Ϣͳ��;����ʶ�𵽵���Ʒ��Ϣ,��Ʒ��ɫ;
uint8_t Material_Number=0;				// ��Ϣͳ��;����ʶ�𵽵���Ʒ��Ϣ,��Ʒ����;
uint16_t Material_PicX,Material_PicY;	// ��Ϣͳ��;����ʶ�𵽵���Ʒ��Ϣ,��Ʒ����(mm);
uint8_t MaterialF1_Num=0;			// ��Ϣͳ��;��ֹģʽ,ʶ�𵽵���Ʒ��Ϣ����;
uint8_t MaterialF2_Num=0;			// ��Ϣͳ��;����ģʽ,ʶ�𵽵���Ʒ��Ϣ����;
uint8_t MaterialF3_Num=0;			// ��Ϣͳ��;����ģʽ,ʶ�𵽵���Ʒ��Ϣ����;
// Private variables end ---------------------------------------------------------//

// Private variables ---------------------------------------------------------//
// ҡ�˵�λ������;
unsigned char YGHand_flag=0;		// ҡ���ֶ�����״̬;���Ϊ1,���ֶ�������̨ת������������;
unsigned char YGHand_dirX=0;		// ҡ���ֶ�����״̬;X�᷽��;//X=0,��ֹ;//X=1,����;//X=2,����;
unsigned char YGHand_dirY=0;		// ҡ���ֶ�����״̬;Y�᷽��;//Y=0,��ֹ;//Y=1,����;//Y=2,����;
unsigned char AN1_flag=0;				// ҡ�˰���;
unsigned char GS_StartFlag=0;			// �˶���Ƶ����;���������־λ;����������ݴ���ģʽ����;
unsigned char Loop_Runing=0;				// �Զ��˶�����;
unsigned char Loop_RunHoldtime=0;		// �Զ��˶�,��ʱ;

// Private variables end ---------------------------------------------------------//


// ��ģ��/��ģ��AIʶ�����-ȫ�ֱ�������;//20241020����;

unsigned int Camera_mode=0;// =0,Ĭ�ϵ�ģ��ģʽ;=1,�߼�ʶ��ģʽ;
Cam_typeRx Camera_RxType;
Material_Collect Collect_Static;
Material_Collect Collect_Low;
Material_Collect Collect_High;



