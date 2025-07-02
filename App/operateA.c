/*
 * @file          operate.c
 * @brief         ����: �������桢�����桢ʱ�����ý��桢�ֶ���������Ȳ��ֺ���;
 * @ucos-ii       V2.86 
 * @function      QSS-ICBS1 
 * @compiler      RealView MDK-ARM Compiler  V5.14
 * @peripheral    V3.5.0
 * @author  	  	
 * @date   	      2023/09/09
 * @corporation   �㽭���ǿƽ��豸���޹�˾
 * @website       http://www.zjqiushi.com/
*/
/* Includes ------------------------------------------------------------------*/
#include  "stm32f10x.h"
#include  "stm32f10x_conf.h" 
#include  "BoardVariable.h"
#include "delay.h"
#include "math.h"
/* Private variables ---------------------------------------------------------*/
unsigned char ValIN_Setwei=0;								// ������ֵ����;���λѡ;
unsigned int	ValIN_Value[15]={0};						// ������ֵ����;�趨��ֵ����;
unsigned int 	ValIN_OneBUF[40]={0};					// ������ֵ����;�趨��ֵ����ֵ����;
unsigned char ValIN_count=2;								// ������ֵ����;�趨��ֵ��������; -> ��2���������趨;
unsigned int 	ValIN_eachlen[15]={3,3};			// ������ֵ����;�趨��ֵ�������ȷֲ�; -> ��{3,3}�����ֱ�Ϊ��λ����λ����;
unsigned int 	ValIN_eachmax[15]={999,999};	// ������ֵ����;�趨��ֵ���������ֵ�ֲ�; -> ��{3,3}�������ֵ�ֱ�Ϊ999��999;



/**********************************************************************************************************
* Function    ��void StartUp_Interface(void)
* Description : �������� ;��ӭ����;
* Para        : none
* Return(s)   : none
* Example     : StartUp_Interface();
**********************************************************************************************************/
void StartUp_Interface(void)
{
		switch(Matrixkey_val)
		{
			case KEY_VAL8:		Video_HandControl=1;		Video_Control(Video_Pause);				break;// "8";��ͣ������Ƶ;
			case KEY_VAL9:		Video_HandControl=1;		Video_Control(Video_Play);				break;// "9";����������Ƶ;
			case KEY_VAL5:		
//				Camera_TxRefresh(0);
			// ����AIģʽ�ϴ��л�ʶ����ѡ;//20241020����;
			Camera_AIselect(3,0,12,15);
				break;// "5";// "5";�����������;
			case KEY_VAL1:
						Laser_ScreenX = Screen_XRealZero;
						Laser_ScreenY = Screen_YRealZero;
						Deal_EnLaserData(Laser_ScreenX,Laser_ScreenY);
				break;	// "1";To Point(0,0);
			case KEY_VAL2:			Init_PTZ_Position();// ��̨�Զ�����λ;
				break;	// "2";To Point_Init;
			case KEY_VAL0:
						Run_Xmotor = 0;	Run_Ymotor = 0;
						Run_Xnum = 0;	Run_Ynum = 0;
						IIC_GetInit=1;//Get_Ini_Val();// ��ȡ��ʼ�Ƕ�;
						Arg_ReadAdj.CBMzeroX=CodeX.RealRead;
						Arg_ReadAdj.CBMzeroY=CodeY.RealRead;
						Flash_eepromIn();	//EEPROM ģ�� FLASH ����;
						ScreenOrigin_PositionSet();
						BUZZER_flag=3;
				break;	// "0";ReInitialize Zero Point;��̨��ʼ��������λ��;
			case KEY_VAL6:		Run_Ymotor = Run_Ymotor+Hand_MotorAdd;			break;	// 8,"6";��̨����;
			case KEY_VAL3:		Run_Ymotor = Run_Ymotor-Hand_MotorAdd;			break;	// 12,"3";��̨����;
			case KEY_VAL7:		Run_Xmotor = Run_Xmotor+Hand_MotorAdd;			break;	// 2,"7";��̨����;
			case KEY_VAL4:		Run_Xmotor = Run_Xmotor-Hand_MotorAdd;			break;	// 6,"4";��̨����;
			case KEY_UP:			Laser_On;							break;	//1,"Up";����ʴ�;
			case KEY_DOWN:		Laser_Off;						break;	//5,"Down";����ʹر�;
			default:break;
		}
		if(Matrixkey_val==KEY_VALE)	// "E";Enter the main interface;
		{
			Matrixkey_val=0;
			LCD_flag = 2;	Set_flag=2;		// ϵͳ����;// ��ת������;
			LCD_DealData();
			HollowPage_flag=0;			//	�˳���ӭ����;
			LCD_ReTime=1;						// ������204A����ʱ��,������ϵͳ;
			PTZ_JiaoZhun_USE = USE_JiaoZhun_Code;		// (USE_JiaoZhun_Code)=0,Ĭ�����ôű���У׼��λ(16��);
		}
		Matrixkey_val=0;
		
		switch(Presskey_val)
		{
			case KEY_F1:	Presskey_val=0;
					if(Hand_MotorAdd==4)	Hand_MotorAdd=16;// �л������ƶ���̨ʱ,���ת���Ĳ���;
					else if(Hand_MotorAdd==16)	Hand_MotorAdd=64;
					else	Hand_MotorAdd=4;
				break;
			case KEY_F2:	Presskey_val=0;	
					Set_flag=10;LCD_flag = 10;	// ��̨������;Material_PTZposition();
				break;
			case KEY_F3:	Presskey_val=0;	
					Set_flag=8;	LCD_flag = 8;		// ��תУ׼����;Material_Adjust();
					Keyset_ValueInit0(Set_flag);// ����ֵ���빦��,��ֵ��ʼ��;
				break;
			case KEY_F4:	Presskey_val=0;	
					Set_flag=7;	LCD_flag = 7;		// ��ת��������ѡ�����;Material_FeatureSel();
					LCD_DealData();
				break;
			default:	break;
		}
}

/**********************************************************************************************************
* Function    ��void Set0_Home(void)
* Description : ������ ;
* Para        : none
* Return(s)   : none
* Example     : Set0_Home();
**********************************************************************************************************/
void Set0_Home(void)
{
		switch(Matrixkey_val)
		{
			case KEY_VAL0:// �����豸ʹ�ù�������;
							Matrixkey_val=0;
							Set_flag=9;		LCD_flag=9;		BUZZER_flag=3;
							LCD_DealData();
				break;
			case KEY_VAL1:	// Set_flag/LCD_flag = 3.ʱ������ ;
							Matrixkey_val=0;
							Set_flag=3;		LCD_flag=3;
							Keyset_ValueInit0(Set_flag);// ����ֵ���빦��,��ֵ��ʼ��;
							LCD_DealData();
				break;
			case KEY_VAL2:	// Set_flag/LCD_flag = 4.�ֶ�ָʾ һ��Ŀ��;
							Matrixkey_val=0;
							Set_flag=4;		LCD_flag=4;
					Loop_Runing=0;	Loop_RunHoldtime=0;	//�Զ�����,��ʱ;
							Keyset_ValueInit0(Set_flag);// ����ֵ���빦��,��ֵ��ʼ��;
							LCD_DealData();
				break;
			case KEY_VAL3:	// Set_flag/LCD_flag = 5.�Զ����( ģʽѡ�� ) ;
							Matrixkey_val=0;
							Set_flag=5;		LCD_flag=5;
							LCD_DealData();
				break;
			case KEY_VAL4:	// Set_flag/LCD_flag = 11.�ֶ�ָʾ ҡ�˿��� ;
							Matrixkey_val=0;
							Set_flag=11;		LCD_flag=11;
					Loop_Runing=0;	Loop_RunHoldtime=0;	//�Զ�����,��ʱ;
							LCD_DealData();
				break;
			case KEY_VAL5: // Set_flag/LCD_flag = 20.ʾ������ʾ ;//20241020����;
				            Matrixkey_val=0;
							Set_flag=20;		LCD_flag=20;
							LCD_DealData();
			default:break;
		}
		Presskey_val=0;
		Matrixkey_val=0;
}


// ʾ������ʾ;////20241020����;
/**********************************************************************************************************
* Function    ��void AutoPick_NopauseFllow(void)
* Description : // "5" ʾ�����ɼ���ʾ ;
* Para        : none
* Return(s)   : none
* Example     : AutoPick_NopauseFllow();
**********************************************************************************************************/
void Show_Wave(void)
{
		if(Matrixkey_val==14)			// "D";�˻�������;
		{
			Matrixkey_val=0;
			Set_flag=2;LCD_flag=2;
			LCD_DealData();
		}
	
}

/**********************************************************************************************************
* Function    ��void Set1_TimeSel(void)
* Description : 1.ʱ������ ;
* Para        : none
* Return(s)   : none
* Example     : Set1_TimeSel();
**********************************************************************************************************/
void Set1_TimeSel(void)
{
		// ������������;ע��˴�����,Matrixkey_valδ��0;
		Keyset_ValueInput();
		if(Matrixkey_val == KEY_VALD)// D ;�˻�������;
		{
				Set_flag=2;LCD_flag=2;
				LCD_DealData();
		}
		else if(Matrixkey_val == KEY_VALE)// E ;ȷ��ʱ��;
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
* Function    ��void ManualControl_Onepoint(void)
* Description : �ֶ�ָʾ ;�ֶ�ָʾ( ָʾһ��Ŀ�� ) ;
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
				if(Laser_State==0)	// ������رռ����;
				{
						Laser_ScreenX = ValIN_Value[0];
						Laser_ScreenY = ValIN_Value[1];
						Deal_EnLaserData(Laser_ScreenX,Laser_ScreenY);
					Laser_OpenFlag=1;		// �Ȳ��������,�ȴ���̨����Ŀ�ĵ���;
					Laser_OpenTimeout=0;
					Loop_Runing=1;	Loop_RunHoldtime=0;	//�Զ�����,��ʱ;
				}
				else 
				{
					Laser_Off;// �رռ���;
					Laser_OpenFlag=0;
					Loop_Runing=0;	//�Զ�����,��ʱ;
					LCD_DealData();
				}
				break;
			default:	break;
		}
		Presskey_val=0;
		
		// �ȴ���̨����Ŀ��λ��;
		if((Run_Xmotor == Run_Xnum)&&(Run_Ymotor == Run_Ynum) )
			{
				if(Laser_OpenFlag==1)// �����ڵȴ�����;�򿪼���;��������;
				{
					Laser_On;
					Laser_OpenFlag=0;
					BUZZER_flag=3;	Sound_HandSet();
					LCD_DealData();
				}
			}
			
		// ������������;ע��˴�����,Matrixkey_valδ��0;
		Keyset_ValueInput();
		if(Matrixkey_val == KEY_VALD)// D ;�˻��ֶ�ģʽѡ�����(��������);�رռ���;
		{
				Laser_Off;Laser_OpenFlag=0;Loop_Runing=0;	//�Զ�����,��ʱ;
				Set_flag=2;LCD_flag=2;
				LCD_DealData();
		}
		Matrixkey_val = 0;
			
}


/**********************************************************************************************************
* Function    ��void ManualControl_Yaogan(void)
* Description : .�ֶ�ҡ��ָʾ ;
* Para        : none
* Return(s)   : none
* Example     : ManualControl_Yaogan();
**********************************************************************************************************/
void ManualControl_Yaogan(void)
{
		YGHand_flag=1;	// ����ҡ�˿���״̬;	
	
		// ҡ�˵�λ������;״̬���;
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2 )==0)	
		{
			while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2 )==0);
			AN1_flag=1;
		}
		else AN1_flag=0;
		
		// ҡ�˰���״̬;
		if(AN1_flag==1)	
		{
			if(Laser_State==0)
			{
				Laser_On;	// �򿪼���;
				Laser_OpenFlag=0;	Laser_OpenTimeout=0;
				Laser_ScreenX = XRealAll;
				Laser_ScreenY = YRealAll;// XRealAll, YRealAll, Ϊ��ǰʵʱ����ֵ;
					Sound_HandSet();// ��������;
				Loop_Runing=1;	Loop_RunHoldtime=0;	//�Զ�����,��ʱ;
			}
			else 
			{
				Laser_Off;	Laser_OpenFlag=0;// �رռ���;
				Loop_Runing=0;	//�Զ�����,��ʱ;
			}
		}
		switch(Presskey_val)
		{
			case KEY_F1:	MOT_SetSpeed_HI;	// ���������̨�˶��ٶȵ���;����;
				break;
			case KEY_F2:	MOT_SetSpeed_LI;	// ���������̨�˶��ٶȵ���;����;
				break;
			case KEY_F3:		break;
			case KEY_F4:		break;
			default:	break;
		}
		Presskey_val=0;

		switch(Matrixkey_val)
		{
			case KEY_VALD:// D ;�˻��ֶ�ģʽѡ�����(��������);�رռ���;
					Laser_Off;Laser_OpenFlag=0;
				YGHand_flag=0;Loop_Runing=0;	//�Զ�����,��ʱ;
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
* Function    ��void Keyset_ValueInit0(void)
* Description : ���̹��� - ��ֵ���뼰�趨���ܳ�ʼ��;�����״ν��뵱ǰҳ�������;
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
			case 3://ʱ������ҳ��;
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
			case 4://�ֶ�ָʾ-1;
					ValIN_Setwei = 0;
					ValIN_count = 2;
						if(PTZ_JiaoZhun_USE == USE_JiaoZhun_Motor)		// (USE_JiaoZhun_Motor)=1,���ò����������У׼��λ(16��);
						{
							ValIN_Value[0]=XReal_Motor;
							ValIN_Value[1]=YReal_Motor;
						}
						else		// (USE_JiaoZhun_Code)=0,Ĭ�����ôű���У׼��λ(16��);
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
			case 8://����У׼����;
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
* Function    ��void Keyset_ValueInput(void)
* Description : ���̹��� - ��ֵ���뼰�趨;ʹ��ʱע��Matrixkey_valδ��0;
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

		// ͳ�ƴ������ֵ�λ������;
		tmp_sum = 0;
		for(i=0;i<ValIN_count;i++)
		{
			tmp_sum = tmp_sum + ValIN_eachlen[i];
		}
		Len = tmp_sum;
		
		if((Len<40) && (Matrixkey_val != 0) && (Matrixkey_val != KEY_VALD) && (Matrixkey_val != KEY_VALE))// ��������;
		{
			// �������:0~9,up,dowm,left,right;������ֵ���빦��;
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
														else	ValIN_Setwei=Len;// �������;
						break;	// Left;
					case KEY_RIGHT:		if(ValIN_Setwei<Len)	ValIN_Setwei=ValIN_Setwei+1;
														else ValIN_Setwei=0;// �������;
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

			// ���ݼ�������ֵ�仯,���㵥����ֵ����Ϣ;
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
						if(ValIN_Setwei<Len)	ValIN_Setwei=ValIN_Setwei+1;// �������;
						else if(ValIN_Setwei==Len)	ValIN_Setwei=1;// �������;
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

