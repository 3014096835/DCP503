/*
 * @file          Function.c
 * @brief         程序: 物品特征选择、校准等部分函数;云台初始化;
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

extern void StepMotor_Codeback(void);

/**********************************************************************************************************
* Function    ：void ScreenOrigin_PositionSet(void)
* Description : 初始化屏幕对标原点位置;
* Para        : none
* Return(s)   : none
* Example     : ScreenOrigin_PositionSet();
**********************************************************************************************************/
void ScreenOrigin_PositionSet(void)
{
	Screen_OriginPosition = 0;	// 左下角,0; 左上角,1; 右下角,2; 右上角,3;
	Screen_XSizeMax = 694;	// 屏幕实际尺寸,X轴长度,mm;
	Screen_YSizeMax = 392;	// 屏幕实际尺寸,Y轴长度,mm;
	if(Screen_OriginPosition == 0)// 左下角,0;
	{
		Screen_XRealOffset = 0;	// X轴屏幕实际位置计算偏移量,mm;
		Screen_YRealOffset = 0;	// Y轴屏幕实际位置计算偏移量,mm;
		Screen_XRealZero = Arg_ReadAdj.PointaX;
		Screen_YRealZero = Arg_ReadAdj.PointaY;
	}
	else if(Screen_OriginPosition == 1)// 左上角,1;
	{
		Screen_XRealOffset = 0;
		Screen_YRealOffset = Screen_YSizeMax;
		Screen_XRealZero = Arg_ReadAdj.PointaX;
		Screen_YRealZero = Screen_YSizeMax-Arg_ReadAdj.PointaY;
	}
	else if(Screen_OriginPosition == 2)// 右下角,2;
	{
		Screen_XRealOffset = Screen_XSizeMax;
		Screen_YRealOffset = 0;
		Screen_XRealZero = Screen_XSizeMax-Arg_ReadAdj.PointaX;
		Screen_YRealZero = Arg_ReadAdj.PointaY;
	}
	else if(Screen_OriginPosition == 3)// 右上角,3;
	{
		Screen_XRealOffset = Screen_XSizeMax;
		Screen_YRealOffset = Screen_YSizeMax;
		Screen_XRealZero = Screen_XSizeMax-Arg_ReadAdj.PointaX;
		Screen_YRealZero = Screen_YSizeMax-Arg_ReadAdj.PointaY;
	}
}

/**********************************************************************************************************
* Function    ：void Material_FeatureSel(void)
* Description : 物体特征选定界面 ;
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
					break;// "8";暂停播放视频;
				case KEY_VAL9:
					Video_HandControl=1;
					Video_Control(Video_Play);
					break;// "9";启动播放视频;
				case KEY_VAL5:
					Camera_TxRefresh(0);
					break;// "5";更改相机方案;
				case KEY_LEFT:	// 向左;
						if(Arg_ReadAdj.Color<=1)	Arg_ReadAdj.Color = 4;
						else Arg_ReadAdj.Color = Arg_ReadAdj.Color-1;	
					LCD_DealData();
				break;
				case KEY_RIGHT:		// 向右;
						if(Arg_ReadAdj.Color>=4)	Arg_ReadAdj.Color = 1;
						else Arg_ReadAdj.Color = Arg_ReadAdj.Color+1;
					LCD_DealData();
				break;
				case KEY_UP:		// 向上;
						if(Arg_ReadAdj.Shape<=1)	Arg_ReadAdj.Shape = 5;
						else Arg_ReadAdj.Shape = Arg_ReadAdj.Shape-1;	
					LCD_DealData();
				break;
				case KEY_DOWN:	// 向下;
						if(Arg_ReadAdj.Shape>=5)	Arg_ReadAdj.Shape = 1;
						else Arg_ReadAdj.Shape = Arg_ReadAdj.Shape+1;
					LCD_DealData();
				break;
				case KEY_VALE:	// "E",确认物品特征并更新相机方案;
					Camera_TxRefresh(0);	// 更改相机方案;
					Flash_eepromIn();		//EEPROM 模拟 FLASH 存入;
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
				if(HollowPage_flag==1)//返回欢迎界面;
					{ Set_flag = 1;	LCD_flag = 1; }	// 跳转开机界面;
				else
					{	Set_flag = 9;	LCD_flag = 9;	}	//返回功能选择界面;
				LCD_DealData();//	|| (Arg_ReadAdj.Type != *(uint16_t*)(0x807f820))
					if((Arg_ReadAdj.Shape != *(uint16_t*)(0x807f81c)) || (Arg_ReadAdj.Color != *(uint16_t*)(0x807f81e)))
					{
						Flash_eepromIn();//EEPROM 模拟 FLASH 存入;
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
* Function    ：void Material_Adjust(void)
* Description : 其他参数校准界面 ;
* Para        : none
* Return(s)   : none
* Example     : Material_Adjust();
**********************************************************************************************************/
void Material_Adjust(void)
{
			if(Presskey_val==KEY_F1)
			{
				Presskey_val=0;
				Flash_Init(0);//校准数据恢复初始值;
				ScreenOrigin_PositionSet();
				BUZZER_flag=5;
				Keyset_ValueInit0(Set_flag);// 有数值输入功能,数值初始化;
			}
			
		// 按键操作函数;注意此处函数,Matrixkey_val未清0;
		if(Keyset_ValueInput())//若反馈值为1,则有数值更新;
		{
		}
		if(Matrixkey_val == KEY_VALD)// D ;退回主界面;
		{
				if(HollowPage_flag==1)//返回欢迎界面;
					{	Set_flag = 1;	LCD_flag = 1;	}
				else
					{	Set_flag = 9;	LCD_flag = 9;	}//返回功能选择界面;
				LCD_DealData();
		}
		else if(Matrixkey_val == KEY_VALE)// E ;确认校准数据;
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
				Flash_eepromIn();	//EEPROM 模拟 FLASH 存入;
				ScreenOrigin_PositionSet();
				BUZZER_flag=3;
				LCD_DealData();
		}
		Matrixkey_val=0;
		
}



/**********************************************************************************************************
* Function    ：void ManualControl_Select(void)
* Description : 设备参数调整使用功能选择;
* Para        : none
* Return(s)   : none
* Example     : ManualControl_Select();
**********************************************************************************************************/
void Material_Select(void)
{
		switch(Presskey_val)
		{
			case KEY_F2:// Set_flag/LCD_flag = 10.云台监测界面;
							Presskey_val=0;
					Set_flag=10;LCD_flag = 10;	// 云台监测界面;Material_PTZposition();
					LCD_DealData();
				break;
			case KEY_F3:// Set_flag/LCD_flag = 8.校准界面;
							Presskey_val=0;
					Set_flag=8;	LCD_flag = 8;		// 跳转校准界面;Material_Adjust();
					Keyset_ValueInit0(Set_flag);// 有数值输入功能,数值初始化;
					LCD_DealData();
				break;
			case KEY_F4:// Set_flag/LCD_flag = 7.物体特征选择界面;
							Presskey_val=0;
					Set_flag=7;	LCD_flag = 7;		// 跳转物体特征选择界面;Material_FeatureSel();
					LCD_DealData();
				break;
			default:break;
		}
		if(Matrixkey_val == KEY_VALD)
		{
			// D ;退回主界面;
			BUZZER_flag=3;
			Set_flag=2;LCD_flag=2;
			LCD_DealData();
		}
		Presskey_val=0;
		Matrixkey_val=0;
}


/**********************************************************************************************************
* Function    ：void Material_PTZposition(void)
* Description : 云台校准调整;
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
		if(PTZ_JiaoZhun_USE == USE_JiaoZhun_Motor)		// (USE_JiaoZhun_Motor)=1,利用步进电机步距校准定位(16点);
		{
			Obj_DataX=XReal_Motor;
			Obj_DataY=YReal_Motor;
		}
		else		// (USE_JiaoZhun_Code)=0,默认利用磁编码校准定位(16点);
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
						case KEY_UP:// 返回上一点;
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
							PTZ_JiaoZhun_USE = USE_JiaoZhun_Code;// (USE_JiaoZhun_Code)=0,默认利用磁编码校准定位(16点);
							BUZZER_flag=3;
							break;	// Left;
						case KEY_RIGHT:	
							PTZ_JiaoZhun_USE = USE_JiaoZhun_Motor;// (USE_JiaoZhun_Motor)=1,利用步进电机步距校准定位(16点);
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
							if(Laser_State==0)	{	Laser_On;		}// 打开激光;
							else 	{	Laser_Off;}// 关闭激光;
						break;
						case KEY_VALD:		break;// D ;退回主界面;关闭激光;
						case KEY_VALE:
							Hand_LoopRun=0;
								Arg_ReadAdj.PlotReal[Hand_Loopcnt] = XReal_Code+100;
								Arg_ReadAdj.PlotReal[Hand_Loopcnt+16] = YReal_Code+100;
								Arg_ReadAdj.MotorReal[Hand_Loopcnt] = XReal_Motor+300;
								Arg_ReadAdj.MotorReal[Hand_Loopcnt+16] = YReal_Motor+300;
								Flash_eepromIn();	//EEPROM 模拟 FLASH 存入;
								BUZZER_flag=5;
							break;// E ;确认移动;
						default:	break;
				}
				if(Matrixkey_val==KEY_VALD)
				{
					Matrixkey_val=0;
					C_shiftflag=1;
						Laser_Off;Laser_OpenFlag=0;
						if(HollowPage_flag==1)//返回欢迎界面;
							{	Set_flag = 1;	LCD_flag = 1;	}
						else
							{	Set_flag = 9;	LCD_flag = 9;	}//返回功能选择界面;
						LCD_DealData();
						break;// D ;退回主界面;
				}
				Matrixkey_val=0;
				
			StepMotor_Codeback();				// 获取云台初始角度;云台磁编码二次调整定位;
			Deal_TranLaserDataMotor();	// 计算出当前实际坐标(步进电机步距定位);
			Deal_TranLaserDataCode();		// 计算出当前实际坐标(绝对位置编码器定位);
			
				LCD_DealData();
				
				
		}
		
}


unsigned char PTZ_PID_num=0;


/**********************************************************************************************************
* Function    ：void Init_PTZ_Position(void)
* Description : 云台初始化;
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
	
		Get_Num_sign(0);// 获取角度值;
		CodeX.RealSet = Dat_SetX;
		CodeX.RealOld = CodeX.RealRead;
		ABAE = 	CBM_D_Value(CodeX.RealSet,CodeX.RealOld,CodeX.Error);
		if(abs(ABAE)>=3)
		{
			if(ABAE>512)				ABAE = 300;
				else if(ABAE<-512)	ABAE = -300;
			Run_Xmotor = Run_Xmotor + ABAE*6400/4096;
			while( !(Run_Xmotor == Run_Xnum) ) ;
			delay_ms(20);    // 延时一段时间
			Get_Num_sign(0);// 获取角度值;
			if((CodeX.RealRead-2)<0)	Dat_Min = 0;
				else	Dat_Min = CodeX.RealRead-2;
			if((CodeX.RealRead+2)>4095)	Dat_Max = 4095;
				else	Dat_Max = CodeX.RealRead+2;
			if((abs(ABAE)>=10)&&((CodeX.RealOld >= Dat_Min) && (CodeX.RealOld <= Dat_Max)))	CodeX.Error=Err;
		}
		Get_Num_sign(1);// 获取角度值;
		CodeY.RealSet = Dat_SetY;
		CodeY.RealOld = CodeY.RealRead;
		ABAE = CBM_D_Value(CodeY.RealSet,CodeY.RealOld,CodeY.Error);
		if(abs(ABAE)>=3)
		{
			if(ABAE>512)				ABAE = 300;
				else if(ABAE<-512)	ABAE = -300;
			Run_Ymotor = Run_Ymotor + ABAE*6400/4096;
			while( !(Run_Ymotor == Run_Ynum) ) ;
			delay_ms(20);    // 延时一段时间
			Get_Num_sign(1);// 获取角度值;
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
			Get_Num_sign(0);// 获取角度值;
			Get_Num_sign(1);// 获取角度值;
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
				//delay_ms(20);    // 延时一段时间
				PTZ_PID_num++;
				if(PTZ_PID_num>=3)
				break;
			}
			else
			{
				PTZ_PID_num=0;
			}
			while( ! ( (Run_Xmotor == Run_Xnum)&&(Run_Ymotor == Run_Ynum) ) )	;
			delay_ms(80);    // 延时一段时间
			Get_Num_sign(0);// 获取角度值;
			Get_Num_sign(1);// 获取角度值;
			
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
		IIC_GetInit=1;	Get_Ini_Val();// 获取初始角度;
		
		if((CodeX.Error==NoErr) && (CodeY.Error==NoErr))	BUZZER_flag=5;
		else
		{
			BUZZER_flag=3;
		}
}


/**********************************************************************************************************
* Function    ：void CBM_D_Value(long set_val,long read_val,unsigned char error_val);
* Description : 磁编码差值计算;
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
* Function    ：void Init_PTZ_PID(void)
* Description : 云台闭环控制;
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
		
			delay_ms(20);    // 延时一段时间
	
	PTZ_PID_num=0;
	
		for(i=0;i<25;i++)
		{
			Get_Num_sign(0);// 获取角度值;
			Get_Num_sign(1);// 获取角度值;
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
				//delay_ms(20);    // 延时一段时间
				PTZ_PID_num++;
				if(PTZ_PID_num>=3)
				break;
			}
			else
			{
				PTZ_PID_num=0;
			}
			while( ! ( (Run_Xmotor == Run_Xnum)&&(Run_Ymotor == Run_Ynum) ) )	;
			delay_ms(80);    // 延时一段时间
			Get_Num_sign(0);// 获取角度值;
			Get_Num_sign(1);// 获取角度值;
			
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
