/*
 * @file          main.c
 * @brief         ������: ������;
 * @ucos-ii       V2.86 
 * @function      QSS-ICBS1 
 * @compiler      RealView MDK-ARM Compiler  V5.14
 * @peripheral    V3.5.0
 * @author  	  	
 * @date   	      2024/05/08
 * @corporation   �㽭���ǿƽ��豸���޹�˾
 * @website       http://www.zjqiushi.com/
*/
/* Includes ------------------------------------------------------------------*/
#include  "stm32f10x.h"
#include  "stm32f10x_conf.h" 
#include  "BoardVariable.h"
#include "delay.h"


// ��������/����ϵͳ����;
uint8_t HollowPage_flag=0;	// ���ڻ�ӭ����ı��,=1���ڻ�ӭ����;=0����ϵͳ����;
// Ŀ����������ѡ������;
uint8_t Select_SetHang=0;// ��������ѡ��ҳ��:��������ѡ��;//0-4;
uint8_t Select_ItemMode=4;// ʶ��ģʽ,=1,��ֹ��ʶ��;=2,���ٵ�ʶ��;=3,���ٵ�ʶ��;=4,����ģʽ��ͬʶ��;//1/2/3,4;
uint8_t Select_Shape[3]={3,1,5};	// ��״��������ֵΪ{3,1,5},��˵���ֱ����þ�ֹ�����١�����ģʽ��, ��ǰ���Ŀ�����״����Ϊ3�š�1�š�5����״;
uint8_t Select_Color[3]={4,2,1};	// ��ɫ��������ֵΪ{4,2,1},��˵���ֱ����þ�ֹ�����١�����ģʽ��, ��ǰ���Ŀ�����ɫ����Ϊ4�š�2�š�1����ɫ;
uint8_t Select_Background=1;	// ����ɫ��������ֵΪ2,��˵�����ü����Ƶ�ı���ɫΪ2�ű���ɫ;

void StartData_Init(void);
void StepMotor_Codeback(void);




int main(void)
{
		// ��̨��������: Code,�ű��뷴���Ƕ�;Motor,�����������λ�÷����Ƕ�;
			// (USE_JiaoZhun_Code)=0,Ĭ�����ôű���У׼��λ(16��);
			// (USE_JiaoZhun_Motor)=1,���ò����������У׼��λ(16��);
		PTZ_JiaoZhun_USE = USE_JiaoZhun_Code;
	
		//************************************************//	
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
		//************************************************//	
		//////////////// ģ���ʼ�� ///////////////////////
    BSP_Configuration(); 		// Init;GPIO;
    TIM7_Configuration(); 	// ��ʱ��,1ms Delay;Init;	
	  delay_init();
		TIM2_Configuration();		// �����������; Motor-Step;X;
		TIM3_Configuration();		// �����������; Motor-Step;Y;
		AS5600_Init();					// �����������; �ű��� AS5600;IIC;
		UART_Init();						// ���ڹ��ܳ�ʼ������;
	
		ADC_Configuration();		// ҡ��; AD_Value,YG;ADC;
	
		//////////////// ���ݼ�״̬��ʼ�� ///////////////////////
		StartData_Init();							// �������ݳ�ʼ��
		ScreenOrigin_PositionSet();		// �޸Ķ�λԭ��Ϊ�ĸ���֮һ,Ĭ�����½�;		
		StepMotor_Codeback();				 	// ��ȡ��̨��ʼ�Ƕ�;
		Init_PTZ_Position();					// ��̨�Զ�����λ;
		
		while (1)// ������;
    {
			switch(Set_flag)
			{
				case 0:
				case 1:		HollowPage_flag=1;	StartUp_Interface();		// ��������;	//���뻶ӭ����;
					break;
				case 2:		Set0_Home();								break;		// ���� 2. ������ ;
				case 3:		Set1_TimeSel();							break;		// ���� 3. ʱ������ ;
				case 4:		ManualControl_Onepoint();		break;		// ���� 4. �ֶ�ָʾ( ָʾһ��Ŀ�� ) ;
				case 5:		AutoPick_ModeSel();					break;		// ���� 5. �Զ����( ģʽѡ�� ) ;
				case 6:		AutoPick_ModeRun();					break;		// ���� 6. �Զ����( ʶ����ͣ��Ƶ�Ҽ��ⲻ�����˶� ) ;
				case 7:		Material_FeatureSel();				break;		// �豸 7: ������������;
				case 8:		Material_Adjust();						break;		// �豸 8: ����У׼;
				case 9:		Material_Select();						break;		// �豸 9: �豸��������ʹ�ù���ѡ��;
				case 10:	Material_PTZposition();				break;		// �豸 10: ��̨��⼰У׼;
				case 11:	ManualControl_Yaogan();			break;		// ���� 11. �ֶ�ָʾ( �ֶ�ҡ�˿��� ) ;
				case 20:	Show_Wave();	 break;// ʾ������ʾ;////20241020����;
				default:break;
			}
			
			StepMotor_Codeback();				// ��ȡ��̨��ʼ�Ƕ�;��̨�ű�����ε�����λ;
			Deal_TranLaserDataMotor();	// �������ǰʵ������(����������ඨλ);
			Deal_TranLaserDataCode();		// �������ǰʵ������(����λ�ñ�������λ);
			
			if(LCD_Refreshflag==0)// LCD��ʱ���͸��²���;// ��������������204A;
			{
				LCD_DealData();
				if(MCGS_Sendform==0)	MCGS_Sendform=5;	// ���ʱ��,�����������߷��͵�ǰ����״̬;
			}
			
			Function_Modbus();					// �ж�485��������,������Ӧ����;(������)
			Camera_RxRefresh();			// �����������������;
		}
		
}

// ��ʼ��: ������״̬��ʼ��;
void StartData_Init(void)
{
		// Init: ��ȡEEPROM�ڴ洢��У׼����;
			Flash_Init(1);
			
		// Init: ʱ�ӳ�ʼ��, ϵͳ������־λ��λ: ����,��������һ��,������������;
			GPIO_SetBits(GPIOC, BSP_GPIOC_LED1);
			GPIO_SetBits(GPIOC, BSP_GPIOC_LED2);
			SysTime_Hour = 14;		SysTime_Minute = 27;		SysTime_Second = 1;
			System_Timer = SysTime_Hour*3600 + SysTime_Minute*60 + SysTime_Second;
			System_flag = 1;
			BUZZER_flag = 0;
			Sound_Start();
		// Init: ���Ӳ���״̬, �����״̬;
			Video_Mode=0;		Video_State=Video_Play;
			Laser_Off;
		// Init: ��̨�������(����),��̨����;
			Run_Xnum = 0;	Run_Ynum = 0;
			Run_Xmotor=0;	Run_Ymotor=0;					// ��ʼ���������Ϊ0;
			MOTX01_SetPul_HI;	MOTX01_SetEn_LI;	MOTX01_SetDir_HI;			// OUT1;// EN,DIR;X,
			MOTY02_SetPul_HI;	MOTY02_SetEn_LI;	MOTY02_SetDir_HI;			// OUT2;// EN,DIR;Y,
			StepMotor_StartRun;									// �������������ʱ������;
			Hand_MotorAdd = 16;										// �ֶ�������̨ʱ,����;
			C_shiftflag=1;											// ��̨���Ƶ���У׼ֵ;//=0,��������У׼;//=1,��У׼;
		// Init: ��̨�������(����),����λ�ü��(�ű���);
			CodeX.InitData = Arg_ReadAdj.CBMzeroX;
			CodeY.InitData = Arg_ReadAdj.CBMzeroY;
			CodeX.Compenflag = 0;
			CodeY.Compenflag = 0;
			CodeX.Error = NoErr;
			CodeY.Error = NoErr;
			IIC_GetInit=1;		// ����,��ȡ�ű����ʼ�Ƕ�;
		// Init: ��ʾҳ��,����ѡ��;
			Set_flag = 1;	LCD_flag = 1; LCD_Refreshflag = 0;
			LCD_DealData();
			
			
			// ʶ��ģʽ����-���ֲ�����ʼ��;//20241020����;
			Camera_mode=1;// =0,Ĭ�ϵ�ģ��ģʽ;=1,�߼�ʶ��ģʽ;
			Collect_Static.SUM=0;
			Collect_Low.SUM=0;
			Collect_High.SUM=0;
}


void StepMotor_Codeback(void)
{
		if(IIC_GetInit!=0)
		{
			Get_Ini_Val();// ��ȡ��ʼ�Ƕ�;
			IIC_GetInit=0;
		}
		else
		{
			CodeX.Error=NoErr;
			CodeY.Error=NoErr;// ����ű��������;
			Get_Num_sign(0);// ��ȡ�Ƕ�ֵ;X��;
			Get_Num_sign(1);// ��ȡ�Ƕ�ֵ;Y��;
		}
		
		if((CodeX.Compenflag==2)&&(CodeY.Compenflag==2))
		{
			if(PTZ_JiaoZhun_USE == USE_JiaoZhun_Code)	// (USE_JiaoZhun_Code)=0,Ĭ�����ôű���У׼��λ(16��);
			{
					Init_PTZ_PID(CodeX.RealSet,CodeY.RealSet);// ��̨��λʱ,Ϊʹ��λ����׼,���ε���;
			}
			CodeX.Compenflag=0;CodeY.Compenflag=0;
		}
}






