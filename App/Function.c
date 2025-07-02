/*
 * @file          Function.c
 * @brief         ����: ��Ʒ����ѡ��У׼�Ȳ��ֺ���;��̨��ʼ��;
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

extern void StepMotor_Codeback(void);

/**********************************************************************************************************
* Function    ��void ScreenOrigin_PositionSet(void)
* Description : ��ʼ����Ļ�Ա�ԭ��λ��;
* Para        : none
* Return(s)   : none
* Example     : ScreenOrigin_PositionSet();
**********************************************************************************************************/
void ScreenOrigin_PositionSet(void)
{
	Screen_OriginPosition = 0;	// ���½�,0; ���Ͻ�,1; ���½�,2; ���Ͻ�,3;
	Screen_XSizeMax = 694;	// ��Ļʵ�ʳߴ�,X�᳤��,mm;
	Screen_YSizeMax = 392;	// ��Ļʵ�ʳߴ�,Y�᳤��,mm;
	if(Screen_OriginPosition == 0)// ���½�,0;
	{
		Screen_XRealOffset = 0;	// X����Ļʵ��λ�ü���ƫ����,mm;
		Screen_YRealOffset = 0;	// Y����Ļʵ��λ�ü���ƫ����,mm;
		Screen_XRealZero = Arg_ReadAdj.PointaX;
		Screen_YRealZero = Arg_ReadAdj.PointaY;
	}
	else if(Screen_OriginPosition == 1)// ���Ͻ�,1;
	{
		Screen_XRealOffset = 0;
		Screen_YRealOffset = Screen_YSizeMax;
		Screen_XRealZero = Arg_ReadAdj.PointaX;
		Screen_YRealZero = Screen_YSizeMax-Arg_ReadAdj.PointaY;
	}
	else if(Screen_OriginPosition == 2)// ���½�,2;
	{
		Screen_XRealOffset = Screen_XSizeMax;
		Screen_YRealOffset = 0;
		Screen_XRealZero = Screen_XSizeMax-Arg_ReadAdj.PointaX;
		Screen_YRealZero = Arg_ReadAdj.PointaY;
	}
	else if(Screen_OriginPosition == 3)// ���Ͻ�,3;
	{
		Screen_XRealOffset = Screen_XSizeMax;
		Screen_YRealOffset = Screen_YSizeMax;
		Screen_XRealZero = Screen_XSizeMax-Arg_ReadAdj.PointaX;
		Screen_YRealZero = Screen_YSizeMax-Arg_ReadAdj.PointaY;
	}
}

/**********************************************************************************************************
* Function    ��void Material_FeatureSel(void)
* Description : ��������ѡ������ ;
* Para        : none
* Return(s)   : none
* Example     : Material_FeatureSel();
**********************************************************************************************************/
void Material_FeatureSel(void)
{
			switch(Matrixkey_val)
			{
				case KEY_VAL8:
					Video_HandControl=1;
					Video_Control(Video_Pause);
					break;// "8";��ͣ������Ƶ;
				case KEY_VAL9:
					Video_HandControl=1;
					Video_Control(Video_Play);
					break;// "9";����������Ƶ;
				case KEY_VAL5:
					Camera_TxRefresh(0);
					break;// "5";�����������;
				case KEY_LEFT:	// ����;
						if(Arg_ReadAdj.Color<=1)	Arg_ReadAdj.Color = 4;
						else Arg_ReadAdj.Color = Arg_ReadAdj.Color-1;	
					LCD_DealData();
				break;
				case KEY_RIGHT:		// ����;
						if(Arg_ReadAdj.Color>=4)	Arg_ReadAdj.Color = 1;
						else Arg_ReadAdj.Color = Arg_ReadAdj.Color+1;
					LCD_DealData();
				break;
				case KEY_UP:		// ����;
						if(Arg_ReadAdj.Shape<=1)	Arg_ReadAdj.Shape = 5;
						else Arg_ReadAdj.Shape = Arg_ReadAdj.Shape-1;	
					LCD_DealData();
				break;
				case KEY_DOWN:	// ����;
						if(Arg_ReadAdj.Shape>=5)	Arg_ReadAdj.Shape = 1;
						else Arg_ReadAdj.Shape = Arg_ReadAdj.Shape+1;
					LCD_DealData();
				break;
				case KEY_VALE:	// "E",ȷ����Ʒ�����������������;
					Camera_TxRefresh(0);	// �����������;
					Flash_eepromIn();		//EEPROM ģ�� FLASH ����;
						if((Select_ItemMode==1) || (Select_ItemMode==2) || (Select_ItemMode==3))
						{
							Select_Shape[Select_ItemMode-1] = Arg_ReadAdj.Shape;
							Select_Color[Select_ItemMode-1] = Arg_ReadAdj.Color;
						}
						else
						{
							Select_Shape[0] = Arg_ReadAdj.Shape;
							Select_Shape[1] = Arg_ReadAdj.Shape;
							Select_Shape[2] = Arg_ReadAdj.Shape;
							Select_Color[0] = Arg_ReadAdj.Color;
							Select_Color[1] = Arg_ReadAdj.Color;
							Select_Color[2] = Arg_ReadAdj.Color;
						}
				break;
				default:break;
			}
			if(Matrixkey_val==KEY_VALD)
			{
				Matrixkey_val=0;
				if(HollowPage_flag==1)//���ػ�ӭ����;
					{ Set_flag = 1;	LCD_flag = 1; }	// ��ת��������;
				else
					{	Set_flag = 9;	LCD_flag = 9;	}	//���ع���ѡ�����;
				LCD_DealData();//	|| (Arg_ReadAdj.Type != *(uint16_t*)(0x807f820))
					if((Arg_ReadAdj.Shape != *(uint16_t*)(0x807f81c)) || (Arg_ReadAdj.Color != *(uint16_t*)(0x807f81e)))
					{
						Flash_eepromIn();//EEPROM ģ�� FLASH ����;
						if((Select_ItemMode==1) || (Select_ItemMode==2) || (Select_ItemMode==3))
						{
							Select_Shape[Select_ItemMode-1] = Arg_ReadAdj.Shape;
							Select_Color[Select_ItemMode-1] = Arg_ReadAdj.Color;
						}
						else
						{
							Select_Shape[0] = Arg_ReadAdj.Shape;
							Select_Shape[1] = Arg_ReadAdj.Shape;
							Select_Shape[2] = Arg_ReadAdj.Shape;
							Select_Color[0] = Arg_ReadAdj.Color;
							Select_Color[1] = Arg_ReadAdj.Color;
							Select_Color[2] = Arg_ReadAdj.Color;
						}
					}
					
				Select_SetHang=0;
			}
			Matrixkey_val=0;
}


/**********************************************************************************************************
* Function    ��void Material_Adjust(void)
* Description : ��������У׼���� ;
* Para        : none
* Return(s)   : none
* Example     : Material_Adjust();
**********************************************************************************************************/
void Material_Adjust(void)
{
			if(Presskey_val==KEY_F1)
			{
				Presskey_val=0;
				Flash_Init(0);//У׼���ݻָ���ʼֵ;
				ScreenOrigin_PositionSet();
				BUZZER_flag=5;
				Keyset_ValueInit0(Set_flag);// ����ֵ���빦��,��ֵ��ʼ��;
			}
			
		// ������������;ע��˴�����,Matrixkey_valδ��0;
		if(Keyset_ValueInput())//������ֵΪ1,������ֵ����;
		{
		}
		if(Matrixkey_val == KEY_VALD)// D ;�˻�������;
		{
				if(HollowPage_flag==1)//���ػ�ӭ����;
					{	Set_flag = 1;	LCD_flag = 1;	}
				else
					{	Set_flag = 9;	LCD_flag = 9;	}//���ع���ѡ�����;
				LCD_DealData();
		}
		else if(Matrixkey_val == KEY_VALE)// E ;ȷ��У׼����;
		{
				Arg_ReadAdj.PointaX = ValIN_Value[0];
				Arg_ReadAdj.PointaY = ValIN_Value[1];
				Arg_ReadAdj.LASER_Axis = ValIN_Value[2];
				Arg_ReadAdj.MatchY = ValIN_Value[3];
				Arg_ReadAdj.CBMzeroX = ValIN_Value[4];
				Arg_ReadAdj.CBMzeroY = ValIN_Value[5];
				Arg_ReadAdj.Volume = ValIN_Value[6];
				Arg_ReadAdj.LowSpeed = ValIN_Value[7];
				Arg_ReadAdj.HighSpeed = ValIN_Value[8];
				Arg_ReadAdj.RunDir = ValIN_Value[9];
				Flash_eepromIn();	//EEPROM ģ�� FLASH ����;
				ScreenOrigin_PositionSet();
				BUZZER_flag=3;
				LCD_DealData();
		}
		Matrixkey_val=0;
		
}



/**********************************************************************************************************
* Function    ��void ManualControl_Select(void)
* Description : �豸��������ʹ�ù���ѡ��;
* Para        : none
* Return(s)   : none
* Example     : ManualControl_Select();
**********************************************************************************************************/
void Material_Select(void)
{
		switch(Presskey_val)
		{
			case KEY_F2:// Set_flag/LCD_flag = 10.��̨������;
							Presskey_val=0;
					Set_flag=10;LCD_flag = 10;	// ��̨������;Material_PTZposition();
					LCD_DealData();
				break;
			case KEY_F3:// Set_flag/LCD_flag = 8.У׼����;
							Presskey_val=0;
					Set_flag=8;	LCD_flag = 8;		// ��תУ׼����;Material_Adjust();
					Keyset_ValueInit0(Set_flag);// ����ֵ���빦��,��ֵ��ʼ��;
					LCD_DealData();
				break;
			case KEY_F4:// Set_flag/LCD_flag = 7.��������ѡ�����;
							Presskey_val=0;
					Set_flag=7;	LCD_flag = 7;		// ��ת��������ѡ�����;Material_FeatureSel();
					LCD_DealData();
				break;
			default:break;
		}
		if(Matrixkey_val == KEY_VALD)
		{
			// D ;�˻�������;
			BUZZER_flag=3;
			Set_flag=2;LCD_flag=2;
			LCD_DealData();
		}
		Presskey_val=0;
		Matrixkey_val=0;
}


/**********************************************************************************************************
* Function    ��void Material_PTZposition(void)
* Description : ��̨У׼����;
* Para        : none
* Return(s)   : none
* Example     : Material_PTZposition();
**********************************************************************************************************/
void Material_PTZposition(void)
{
		unsigned int Hand_LoopRun=0;
		unsigned int Hand_Loopcnt=0;
		long int Set_PosX[16]={40,240,440,640, 640,440,240,40,  40,240,440,640,   640,440,240,40};
		long int Set_PosY[16]={0,0,0,0,        120,120,120,120, 240,240,240,240,  360,360,360,360};
		unsigned int Hand_Loopsum=16;
		
		Hand_MotorAdd=1;
		C_shiftflag=0;
		Hand_Loopcnt=16;
		if(PTZ_JiaoZhun_USE == USE_JiaoZhun_Motor)		// (USE_JiaoZhun_Motor)=1,���ò����������У׼��λ(16��);
		{
			Obj_DataX=XReal_Motor;
			Obj_DataY=YReal_Motor;
		}
		else		// (USE_JiaoZhun_Code)=0,Ĭ�����ôű���У׼��λ(16��);
		{
			Obj_DataX=XReal_Code;
			Obj_DataY=YReal_Code;
		}

		while(1)
		{
			
			switch(Presskey_val)
			{
				case KEY_F1:// Motor Speed; TIM2;	// Motor Speed; TIM2;
						if(Hand_MotorAdd==1)	Hand_MotorAdd=2;
						else if(Hand_MotorAdd==2)	Hand_MotorAdd=4;
						else	Hand_MotorAdd=1;
					break;
				case KEY_F2:
						if(C_shiftflag==0)	{C_shiftflag=1;BUZZER_flag=5;	}
						else			{C_shiftflag=0;BUZZER_flag=3;	}
					break;
				case KEY_F3:
						if(Hand_LoopRun==0)
						{
							Hand_LoopRun=0;
									delay_ms(500);
							if(Hand_Loopcnt<Hand_Loopsum-1)	Hand_Loopcnt++;
							else	Hand_Loopcnt=0;
								Laser_ScreenX = Set_PosX[Hand_Loopcnt];
								Laser_ScreenY = Set_PosY[Hand_Loopcnt];
								Deal_EnLaserData(Laser_ScreenX,Laser_ScreenY);
								Obj_DataX=Laser_ScreenX;
								Obj_DataY=Laser_ScreenY;
						}
						else	Hand_LoopRun=0;
					break;
				case KEY_F4:
						if(Hand_LoopRun==0)	Hand_LoopRun=1;
						else	Hand_LoopRun=0;
					break;
				default:		break;
			}
			Presskey_val=0;
			
			if((Run_Xmotor == Run_Xnum)&&(Run_Ymotor == Run_Ynum)&&(CodeX.Compenflag==0)&&(CodeY.Compenflag==0) )
				{
					if(Hand_LoopRun==1)
					{
						delay_ms(500);
						if(Hand_Loopcnt<Hand_Loopsum-1)	Hand_Loopcnt++;
						else	Hand_Loopcnt=0;
						Laser_ScreenX = Set_PosX[Hand_Loopcnt];
						Laser_ScreenY = Set_PosY[Hand_Loopcnt];
						Deal_EnLaserData(Laser_ScreenX,Laser_ScreenY);
						Obj_DataX=Laser_ScreenX;
						Obj_DataY=Laser_ScreenY;
					}
				}


				switch(Matrixkey_val)
				{
						case KEY_UP:// ������һ��;
							if(Hand_LoopRun==0)
							{
								Hand_LoopRun=0;
										delay_ms(500);
								if(Hand_Loopcnt>0)	Hand_Loopcnt--;
								else	Hand_Loopcnt=Hand_Loopsum-1;
									Laser_ScreenX = Set_PosX[Hand_Loopcnt];
									Laser_ScreenY = Set_PosY[Hand_Loopcnt];
									Deal_EnLaserData(Laser_ScreenX,Laser_ScreenY);
									Obj_DataX=Laser_ScreenX;
									Obj_DataY=Laser_ScreenY;
							}
							else	Hand_LoopRun=0;
							break;	// Up;
						case KEY_DOWN:					break;	// Down;
						case KEY_LEFT:
							PTZ_JiaoZhun_USE = USE_JiaoZhun_Code;// (USE_JiaoZhun_Code)=0,Ĭ�����ôű���У׼��λ(16��);
							BUZZER_flag=3;
							break;	// Left;
						case KEY_RIGHT:	
							PTZ_JiaoZhun_USE = USE_JiaoZhun_Motor;// (USE_JiaoZhun_Motor)=1,���ò����������У׼��λ(16��);
							BUZZER_flag=5;
							break;	// Right;
						case KEY_VAL0:		break;	// "0";ReInitialize Zero Point;
						case KEY_VAL1:		break;	// "1";To Point(0,0);
						case KEY_VAL2:		Init_PTZ_Position();			break;	// "2";To Point_Init;
						case KEY_VAL5:		break;// "5";
						case KEY_VAL6:		Run_Ymotor = Run_Ymotor+Hand_MotorAdd;				break;	// 8,"6";
						case KEY_VAL3:		Run_Ymotor = Run_Ymotor-Hand_MotorAdd;				break;	// 12,"3";
						case KEY_VAL7:		Run_Xmotor = Run_Xmotor+Hand_MotorAdd;				break;	// 2,"7";
						case KEY_VAL4:		Run_Xmotor = Run_Xmotor-Hand_MotorAdd;				break;	// 6,"4";
						case KEY_VAL8:		break;// 8 ;
						case KEY_VAL9:
							if(Laser_State==0)	{	Laser_On;		}// �򿪼���;
							else 	{	Laser_Off;}// �رռ���;
						break;
						case KEY_VALD:		break;// D ;�˻�������;�رռ���;
						case KEY_VALE:
							Hand_LoopRun=0;
								Arg_ReadAdj.PlotReal[Hand_Loopcnt] = XReal_Code+100;
								Arg_ReadAdj.PlotReal[Hand_Loopcnt+16] = YReal_Code+100;
								Arg_ReadAdj.MotorReal[Hand_Loopcnt] = XReal_Motor+300;
								Arg_ReadAdj.MotorReal[Hand_Loopcnt+16] = YReal_Motor+300;
								Flash_eepromIn();	//EEPROM ģ�� FLASH ����;
								BUZZER_flag=5;
							break;// E ;ȷ���ƶ�;
						default:	break;
				}
				if(Matrixkey_val==KEY_VALD)
				{
					Matrixkey_val=0;
					C_shiftflag=1;
						Laser_Off;Laser_OpenFlag=0;
						if(HollowPage_flag==1)//���ػ�ӭ����;
							{	Set_flag = 1;	LCD_flag = 1;	}
						else
							{	Set_flag = 9;	LCD_flag = 9;	}//���ع���ѡ�����;
						LCD_DealData();
						break;// D ;�˻�������;
				}
				Matrixkey_val=0;
				
			StepMotor_Codeback();				// ��ȡ��̨��ʼ�Ƕ�;��̨�ű�����ε�����λ;
			Deal_TranLaserDataMotor();	// �������ǰʵ������(����������ඨλ);
			Deal_TranLaserDataCode();		// �������ǰʵ������(����λ�ñ�������λ);
			
				LCD_DealData();
				
				
		}
		
}


unsigned char PTZ_PID_num=0;


/**********************************************************************************************************
* Function    ��void Init_PTZ_Position(void)
* Description : ��̨��ʼ��;
* Para        : none
* Return(s)   : none
* Example     : Init_PTZ_Position();
**********************************************************************************************************/
void Init_PTZ_Position(void)
{
		unsigned char i;
		long ABAE=0;
		long Dat_SetX,Dat_SetY;
		long Dat_ChX,Dat_ChY;
		long Dat_Min,Dat_Max;
		
		CodeX.InitData = Arg_ReadAdj.CBMzeroX;
		CodeY.InitData = Arg_ReadAdj.CBMzeroY;
		Dat_SetX = CodeX.InitData;
		Dat_SetY = CodeY.InitData;
	
		Get_Num_sign(0);// ��ȡ�Ƕ�ֵ;
		CodeX.RealSet = Dat_SetX;
		CodeX.RealOld = CodeX.RealRead;
		ABAE = 	CBM_D_Value(CodeX.RealSet,CodeX.RealOld,CodeX.Error);
		if(abs(ABAE)>=3)
		{
			if(ABAE>512)				ABAE = 300;
				else if(ABAE<-512)	ABAE = -300;
			Run_Xmotor = Run_Xmotor + ABAE*6400/4096;
			while( !(Run_Xmotor == Run_Xnum) ) ;
			delay_ms(20);    // ��ʱһ��ʱ��
			Get_Num_sign(0);// ��ȡ�Ƕ�ֵ;
			if((CodeX.RealRead-2)<0)	Dat_Min = 0;
				else	Dat_Min = CodeX.RealRead-2;
			if((CodeX.RealRead+2)>4095)	Dat_Max = 4095;
				else	Dat_Max = CodeX.RealRead+2;
			if((abs(ABAE)>=10)&&((CodeX.RealOld >= Dat_Min) && (CodeX.RealOld <= Dat_Max)))	CodeX.Error=Err;
		}
		Get_Num_sign(1);// ��ȡ�Ƕ�ֵ;
		CodeY.RealSet = Dat_SetY;
		CodeY.RealOld = CodeY.RealRead;
		ABAE = CBM_D_Value(CodeY.RealSet,CodeY.RealOld,CodeY.Error);
		if(abs(ABAE)>=3)
		{
			if(ABAE>512)				ABAE = 300;
				else if(ABAE<-512)	ABAE = -300;
			Run_Ymotor = Run_Ymotor + ABAE*6400/4096;
			while( !(Run_Ymotor == Run_Ynum) ) ;
			delay_ms(20);    // ��ʱһ��ʱ��
			Get_Num_sign(1);// ��ȡ�Ƕ�ֵ;
			if((CodeY.RealRead-2)<0)	Dat_Min = 0;
				else	Dat_Min = CodeY.RealRead-2;
			if((CodeY.RealRead+2)>4095)	Dat_Max = 4095;
				else	Dat_Max = CodeY.RealRead+2;
			if((abs(ABAE)>=10)&&((CodeY.RealOld >= Dat_Min) && (CodeY.RealOld <= Dat_Max)))	CodeY.Error=Err;
		}

		for(i=0;i<25;i++)
		{
			if(i==1)	
			{
				CodeX.RealSet = Dat_SetX+300;
				CodeY.RealSet = Dat_SetY+300;
			}
			else	
			{
				CodeX.RealSet = Dat_SetX;
				CodeY.RealSet = Dat_SetY;
			}
			Get_Num_sign(0);// ��ȡ�Ƕ�ֵ;
			Get_Num_sign(1);// ��ȡ�Ƕ�ֵ;
			CodeX.RealOld = CodeX.RealRead;
			CodeY.RealOld = CodeY.RealRead;
			Dat_ChX=0;	Dat_ChY=0;
			ABAE = 	CBM_D_Value(CodeX.RealSet,CodeX.RealOld,CodeX.Error);
			if(abs(ABAE)>=1)
			{
				Dat_ChX=abs(ABAE);
				Run_Xmotor = Run_Xmotor + ABAE*6400/4096;
			}
			ABAE = CBM_D_Value(CodeY.RealSet,CodeY.RealOld,CodeY.Error);
			if(abs(ABAE)>=1)
			{
				Dat_ChY=abs(ABAE);
				Run_Ymotor = Run_Ymotor + ABAE*6400/4096;
			}
			if((i>1) && ((Dat_ChX==0)&&(Dat_ChY==0)))
			{
				//delay_ms(20);    // ��ʱһ��ʱ��
				PTZ_PID_num++;
				if(PTZ_PID_num>=3)
				break;
			}
			else
			{
				PTZ_PID_num=0;
			}
			while( ! ( (Run_Xmotor == Run_Xnum)&&(Run_Ymotor == Run_Ynum) ) )	;
			delay_ms(80);    // ��ʱһ��ʱ��
			Get_Num_sign(0);// ��ȡ�Ƕ�ֵ;
			Get_Num_sign(1);// ��ȡ�Ƕ�ֵ;
			
			if(Dat_ChX>10)
			{
				if((CodeX.RealRead-2)<0)	Dat_Min = 0;
					else	Dat_Min = CodeX.RealRead-2;
				if((CodeX.RealRead+2)>4095)	Dat_Max = 4095;
					else	Dat_Max = CodeX.RealRead+2;
				if((CodeX.RealOld >= Dat_Min) && (CodeX.RealOld <= Dat_Max))	CodeX.Error=Err;
			}
			if(Dat_ChY>10)
			{	
				if((CodeY.RealRead-2)<0)	Dat_Min = 0;
					else	Dat_Min = CodeY.RealRead-2;
				if((CodeY.RealRead+2)>4095)	Dat_Max = 4095;
					else	Dat_Max = CodeY.RealRead+2;
				if((CodeY.RealOld >= Dat_Min) && (CodeY.RealOld <= Dat_Max))	CodeY.Error=Err;
			}
		}
		
		Run_Xmotor = 0;	Run_Ymotor = 0;
		Run_Xnum = 0;		Run_Ynum = 0;
		IIC_GetInit=1;	Get_Ini_Val();// ��ȡ��ʼ�Ƕ�;
		
		if((CodeX.Error==NoErr) && (CodeY.Error==NoErr))	BUZZER_flag=5;
		else
		{
			BUZZER_flag=3;
		}
}


/**********************************************************************************************************
* Function    ��void CBM_D_Value(long set_val,long read_val,unsigned char error_val);
* Description : �ű����ֵ����;
* Para        : none
* Return(s)   : none
* Example     : CBM_D_Value(long set_val,long read_val,unsigned char error_val);
**********************************************************************************************************/
long CBM_D_Value(long set_val,long read_val,unsigned char error_val)
{
		long D_Val=0;
	
		if(error_val == NoErr)
		{
		
			if((set_val+2048) < 4096)
			{
				if((read_val>=set_val) && (read_val<=(set_val+2048)))
				{
					D_Val = set_val - read_val;
				}
				else
				{
					if(read_val<=set_val)
					{
						D_Val = set_val - read_val;
					}
					else
					{
						D_Val = set_val - read_val + 4096;
					}
				}
			}
			else
			{
				if((read_val>=(set_val-2048)) && (read_val<=set_val))
				{
					D_Val = set_val - read_val;
				}
				else
				{
					if(read_val<=(set_val-2048))
					{
						D_Val = set_val - read_val - 4096;
					}
					else
					{
						D_Val = set_val - read_val;
					}
				}
			}
		}
			
			
		return D_Val;
		
}



/**********************************************************************************************************
* Function    ��void Init_PTZ_PID(void)
* Description : ��̨�ջ�����;
* Para        : none
* Return(s)   : none
* Example     : Init_PTZ_PID();
**********************************************************************************************************/
void Init_PTZ_PID(long set_valx,long set_valy)
{
		unsigned char i;
		long ABAE=0;
		long Dat_ChX,Dat_ChY;
		long Dat_Min,Dat_Max;
		
			delay_ms(20);    // ��ʱһ��ʱ��
	
	PTZ_PID_num=0;
	
		for(i=0;i<25;i++)
		{
			Get_Num_sign(0);// ��ȡ�Ƕ�ֵ;
			Get_Num_sign(1);// ��ȡ�Ƕ�ֵ;
			CodeX.RealOld = CodeX.RealRead;
			CodeY.RealOld = CodeY.RealRead;
			Dat_ChX=0;	Dat_ChY=0;
			ABAE = 	CBM_D_Value(CodeX.RealSet,CodeX.RealOld,CodeX.Error);
			if(abs(ABAE)>=2)
			{
				Dat_ChX=abs(ABAE);
				Run_Xmotor = Run_Xmotor + ABAE*6400/4096;
			}
			ABAE = CBM_D_Value(CodeY.RealSet,CodeY.RealOld,CodeY.Error);
			if(abs(ABAE)>=2)
			{
				Dat_ChY=abs(ABAE);
				Run_Ymotor = Run_Ymotor + ABAE*6400/4096;
			}
			if((i>1) && ((Dat_ChX==0)&&(Dat_ChY==0)))	
			{
				//delay_ms(20);    // ��ʱһ��ʱ��
				PTZ_PID_num++;
				if(PTZ_PID_num>=3)
				break;
			}
			else
			{
				PTZ_PID_num=0;
			}
			while( ! ( (Run_Xmotor == Run_Xnum)&&(Run_Ymotor == Run_Ynum) ) )	;
			delay_ms(80);    // ��ʱһ��ʱ��
			Get_Num_sign(0);// ��ȡ�Ƕ�ֵ;
			Get_Num_sign(1);// ��ȡ�Ƕ�ֵ;
			
			if(Dat_ChX>10)
			{
				if((CodeX.RealRead-2)<0)	Dat_Min = 0;
					else	Dat_Min = CodeX.RealRead-2;
				if((CodeX.RealRead+2)>4095)	Dat_Max = 4095;
					else	Dat_Max = CodeX.RealRead+2;
				if((CodeX.RealOld >= Dat_Min) && (CodeX.RealOld <= Dat_Max))	CodeX.Error=Err;
			}
			if(Dat_ChY>10)
			{	
				if((CodeY.RealRead-2)<0)	Dat_Min = 0;
					else	Dat_Min = CodeY.RealRead-2;
				if((CodeY.RealRead+2)>4095)	Dat_Max = 4095;
					else	Dat_Max = CodeY.RealRead+2;
				if((CodeY.RealOld >= Dat_Min) && (CodeY.RealOld <= Dat_Max))	CodeY.Error=Err;
			}
		}
		
		CodeX.Compenflag=0;CodeY.Compenflag=0;
}
