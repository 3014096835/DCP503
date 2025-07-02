/*
 * @file          operate.c
 * @brief         程序: 自动检测界面等部分函数;
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

unsigned char Run_FollowFlag=0;


/**********************************************************************************************************
* Function    ：void AutoPick_ModeSel(void)
* Description : .难度选择 ;
* Para        : none
* Return(s)   : none
* Example     : AutoPick_ModeSel();
**********************************************************************************************************/
void AutoPick_ModeSel(void)
{
		if(Matrixkey_val==KEY_VALD)			// "D";退回主界面;
		{
			Matrixkey_val=0;
			Set_flag=2;LCD_flag=2;
			LCD_DealData();
		}
		
		switch(Presskey_val)
		{
			case KEY_F1:// "F1";启动静止模式,并进入信息统计界面;
				// Set_flag/LCD_flag = 6. 自动检测( 识别到暂停视频且激光不跟随运动 ) ;
					delay_ms(50);
					Video_Mode = 0;	Video_State = Video_Play;
					Set_flag=6;		LCD_flag=6;	//3.Auto Run Details;
					BUZZER_flag=3;	Auto_Running=1;Auto_RunTimeflag=1;Test_SpeedEn=0;
					Data_init();
					LCD_AutoEnter=1;	LCD_DealData();
				break;
			case KEY_F2:// "F2";启动低速模式,并进入信息统计界面;
				// Set_flag/LCD_flag = 6. 自动检测( 识别到暂停视频且激光不跟随运动 ) ;
					delay_ms(50);
					Video_Mode = 1;	Video_State = Video_Play;
					Set_flag=6;		LCD_flag=6;	//3.Auto Run Details;
					BUZZER_flag=3;	Auto_Running=1;Auto_RunTimeflag=1;Test_SpeedEn=0;
					Data_init();
					LCD_AutoEnter=1;	LCD_DealData();
				break;
			case KEY_F3:// "F3";启动高速模式,并进入信息统计界面;
				// Set_flag/LCD_flag = 6. 自动检测( 识别到暂停视频且激光不跟随运动 ) ;
					delay_ms(50);
					Video_Mode = 2;	Video_State = Video_Play;
					Set_flag=6;		LCD_flag=6;	//3.Auto Run Details;
					BUZZER_flag=3;	Auto_Running=1;Auto_RunTimeflag=1;Test_SpeedEn=0;
					Data_init();
					LCD_AutoEnter=1;	LCD_DealData();
				break;
			case KEY_F4:// "F4";
				break;
			default:		break;
		}
		Presskey_val=0;
}



/**********************************************************************************************************
* Function    ：void Keyset_AutoPick(void)
* Description : 自动检测功能下的按键功能使用;
* Para        : none
* Return(s)   : none
* Example     : Keyset_AutoPick();
**********************************************************************************************************/
void Keyset_AutoPick01(void)
{
		// "F1","F2","F3",分别对应静止/低速/高速模式下,系统检测的启动与停止;
		switch(Presskey_val)
		{
			case KEY_F1:
				if(Video_Mode ==0)
				{
					if(Auto_Running==0)
					{
						BUZZER_flag=3;	Auto_Running=1;Auto_RunTimeflag=1;		Data_init();	
						delay_ms(50);
					}
					else
					{
						BUZZER_flag=5;	Auto_Running=0;Auto_RunTimeflag=0;		Massage_PostEnd();
						delay_ms(50);
					}
				}
				break;
			case KEY_F2:
				if(Video_Mode == 1)
				{
					if(Auto_Running==0)
					{
						BUZZER_flag=3;	Auto_Running=1;Auto_RunTimeflag=1;		Data_init();
						delay_ms(50);
					}
					else
					{
						BUZZER_flag=5;	Auto_Running=0;Auto_RunTimeflag=0;		Massage_PostEnd();
						delay_ms(50);
					}
				}
				break;
			case KEY_F3:
				if(Video_Mode == 2)
				{
					if(Auto_Running==0)
					{
						BUZZER_flag=3;	Auto_Running=1;Auto_RunTimeflag=1;		Data_init();
						delay_ms(50);
					}
					else
					{
						BUZZER_flag=5;	Auto_Running=0;Auto_RunTimeflag=0;		Massage_PostEnd();
						delay_ms(50);
					}
				}
				break;
			case KEY_F4:	break;
			default:	break;
		}
		Presskey_val=0;

		switch(Matrixkey_val)	// 其他按键功能;
		{
			case KEY_UP:
				Page_Up=1;		LCD_DealData();
				break;// "Up";向上翻页;
			case KEY_DOWN:
				Page_Down=1;	LCD_DealData();
				break;// "Down";向下翻页;
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
			case KEY_VALD:
				Auto_Running=0;	Auto_RunTimeflag=0;
				Set_flag=5;	LCD_flag=5;		// "D";退回模式界面;
				LCD_DealData();
				Pic_Success=0;Laser_OpenFlag=0;Video_timeflag=0;
				break;// "D";// 退出界面;
			case KEY_VAL0:
					Laser_ScreenX = Screen_XRealZero;
					Laser_ScreenY = Screen_YRealZero;
				Deal_EnLaserData(Laser_ScreenX,Laser_ScreenY);
				break;// "0";// 转到(0,0)点;
			case KEY_VALE:
				break;// "E";
		}
		Matrixkey_val=0;
}


// 当前指示物品信息记录;//20241020新增;
void Deal_Materialmassage(unsigned int Followcnt)
{
	unsigned int addrcam=0;
	unsigned int addrmass=0;
	
		Material_Xinxi++;
		Material_TimeOut = Auto_RunTimesec;
		Material_PicX = Res_PosFollow[Followcnt][0];
		Material_PicY = Res_PosFollow[Followcnt][1];
		Material_Number = 1;
	
		if(Camera_mode==0)// =0,默认单模型模式;=1,高级识别模式;
		{
			Material_Shape = Camera_RxType.Shape;//Arg_ReadAdj.Shape
			Material_Color = Camera_RxType.Color;//Arg_ReadAdj.Color
		}
		else
		{
			for(addrcam=0;addrcam<Camera_RxType.Cnt;addrcam++)
			{
				if((Material_PicX == Camera_RxType.pointx[addrcam])&&(Material_PicY == Camera_RxType.pointy[addrcam]))
					break;
			}
			switch(Camera_RxType.Typelist[addrcam])
			{
				case 0:	Material_Shape = 1;	Material_Color = 1;break;
				case 1:	Material_Shape = 2;	Material_Color = 1;break;
				case 2:	Material_Shape = 3;	Material_Color = 1;break;
				case 3:	Material_Shape = 4;	Material_Color = 1;break;
				case 4:	Material_Shape = 5;	Material_Color = 1;break;
				case 5:	Material_Shape = 1;	Material_Color = 3;break;
				case 6:	Material_Shape = 2;	Material_Color = 3;break;
				case 7:	Material_Shape = 3;	Material_Color = 3;break;
				case 8:	Material_Shape = 4;	Material_Color = 3;break;
				case 9:	Material_Shape = 5;	Material_Color = 3;break;
				case 10:	Material_Shape = 1;	Material_Color = 4;break;
				case 11:	Material_Shape = 2;	Material_Color = 4;break;
				case 12:	Material_Shape = 3;	Material_Color = 4;break;
				case 13:	Material_Shape = 4;	Material_Color = 4;break;
				case 14:	Material_Shape = 5;	Material_Color = 4;break;
				case 15:	Material_Shape = 1;	Material_Color = 2;break;
				case 16:	Material_Shape = 2;	Material_Color = 2;break;
				case 17:	Material_Shape = 3;	Material_Color = 2;break;
				case 18:	Material_Shape = 4;	Material_Color = 2;break;
				case 19:	Material_Shape = 5;	Material_Color = 2;break;
			}
		}
		
		if(Video_Mode==0)
		{
				for(addrmass=0;addrmass<Collect_Static.SUM;addrmass++)
				{
					if((Collect_Static.Color[addrmass] == Material_Color)&&(Collect_Static.Shape[addrmass] == Material_Shape))
						break;
				}
				if(addrmass >= Collect_Static.SUM)
				{
					Collect_Static.Listcnt[addrmass] = 1;
					Collect_Static.List[addrmass] = Camera_RxType.Typelist[addrcam];
					Collect_Static.Shape[addrmass] = Material_Shape;
					Collect_Static.Color[addrmass] = Material_Color;
					Collect_Static.SUM++;
				}
				else
				{
					Collect_Static.Listcnt[addrmass] ++;
				}
		}
		else if(Video_Mode==1)
		{
				for(addrmass=0;addrmass<Collect_Low.SUM;addrmass++)
				{
					if((Collect_Low.Color[addrmass] == Material_Color)&&(Collect_Low.Shape[addrmass] == Material_Shape))
						break;
				}
				if(addrmass >= Collect_Low.SUM)
				{
					Collect_Low.Listcnt[addrmass] = 1;
					Collect_Low.List[addrmass] = Camera_RxType.Typelist[addrcam];
					Collect_Low.Shape[addrmass] = Material_Shape;
					Collect_Low.Color[addrmass] = Material_Color;
					Collect_Low.SUM++;
				}
				else
				{
					Collect_Low.Listcnt[addrmass] ++;
				}
		}
		else if(Video_Mode==2)
		{
				for(addrmass=0;addrmass<Collect_High.SUM;addrmass++)
				{
					if((Collect_High.Color[addrmass] == Material_Color)&&(Collect_High.Shape[addrmass] == Material_Shape))
						break;
				}
				if(addrmass >= Collect_High.SUM)
				{
					Collect_High.Listcnt[addrmass] = 1;
					Collect_High.List[addrmass] = Camera_RxType.Typelist[addrcam];
					Collect_High.Shape[addrmass] = Material_Shape;
					Collect_High.Color[addrmass] = Material_Color;
					Collect_High.SUM++;
				}
				else
				{
					Collect_High.Listcnt[addrmass] ++;
				}
		}
}

/**********************************************************************************************************
* Function    ：void AutoPick_ModeRun(void)
* Description : 4.Auto Run Details;
* Para        : none
* Return(s)   : none
* Example     : AutoPick_ModeRun();
**********************************************************************************************************/
void AutoPick_ModeRun(void)
{
		unsigned int ix1,iy1;//,iz1
	int Tmp_timeA,Tmp_timeB,Tmp_timeC;
	
		GS_StartFlag=0;				// 运动视频跟随;启动跟随标志位;=1,跟随;
		Keyset_AutoPick01();
		
		////////// 开放采集到相机数据 数据处理; //////////////
		if((Cam_Rxflag == 1)&&(Pic_Success==0))
		{
				Run_FollowUpdate=0;
				if(Auto_Running==1)	Auto_DataProcess(Arg_ReadAdj.RunDir,Video_Mode,0);
				
				// 确定有目标,目标确定;记录当前目标信息,定位目标位置,发送信息更新////并语音播报;
				if(Pic_Success==1)
				{
					Video_Control(Video_Pause);
					Laser_OpenFlag=0;
					Laser_Off;
					
					// 当前指示物品信息记录;//20241020修改;
					Deal_Materialmassage(Res_FollowNum);//指示目标的信息分解;
					
						Laser_ScreenX = Material_PicX;
						Laser_ScreenY = Material_PicY;
					Deal_EnLaserData(Laser_ScreenX,Laser_ScreenY);
					
					Massage_Post();
				}
		}
		
		/////////// 发现目标物体 //////////////
		if(Pic_Success==1)
		{
			// 当发现目标物体,并移动到目标位置后,打开激光笔;// 语音播报;
			// 激光笔光斑持续 5s 后,关闭激光笔,发送信息启动视频播放,回归到零点位置;
		
			if((Run_Xmotor == Run_Xnum)&&(Run_Ymotor == Run_Ynum)&&(CodeX.Compenflag==0)&&(CodeY.Compenflag==0))
			{
				if(Laser_OpenFlag==0)
				{
					Laser_On;
					Sound_POST();
					Laser_OpenFlag=1;
					Laser_OpenTimeout=0;
				}
				if((Laser_OpenFlag==1) && (Laser_OpenTimeout>=5000))
				{
					Res_FollowNum++;
					if(Res_FollowNum<Res_FollowSum)
					{
						Laser_OpenFlag=0;
						Laser_Off;
						
						// 当前指示物品信息记录;//20241020修改;
					Deal_Materialmassage(Res_FollowNum);//指示目标的信息分解;
						
						
						Laser_ScreenX = Material_PicX;
						Laser_ScreenY = Material_PicY;
						Deal_EnLaserData(Laser_ScreenX,Laser_ScreenY);
						
						Massage_Post();
					}
					else
					{
						Laser_OpenFlag=0;
						Laser_Off;
						
						Pic_Success=0;
						Video_Control(Video_Play);
						
						Run_FollowAddr=0;
						for(ix1=0;ix1<Res_FollowSum;ix1++)
						{
							for(iy1=0;iy1<Res_Trackcnt;iy1++)
							{
								if((Res_PosTrack[iy1][0]==Res_PosFollow[ix1][0]) && (Res_PosTrack[iy1][1]==Res_PosFollow[ix1][1]))
								{
									Res_TrackACK[iy1]=1;
									Tmp_timeA=Res_PosTrack[iy1][2];
									Run_FollowAddr=iy1+1;
								}
							}
						}
						
						Tmp_timeB=Auto_RunTimemisec;
						if(Tmp_timeB<Tmp_timeA)
							Tmp_timeC=65536+Tmp_timeB-Tmp_timeA;
						else
							Tmp_timeC=Tmp_timeB-Tmp_timeA;
						for(ix1=0;ix1<Res_Trackcnt;ix1++)
						{
							Res_PosTrack[ix1][2] = Res_PosTrack[ix1][2]+Tmp_timeC;
						}
						
						
						////////// 跟随 /////////////////
						Run_FollowData[0] = Res_PosTrack[Run_FollowAddr-1][0];
						Run_FollowData[1] = Res_PosTrack[Run_FollowAddr-1][1];
						Run_FollowData[2] = Res_PosTrack[Run_FollowAddr-1][2];
						Run_FollowUpdate=1;
						
							Laser_ScreenX = Screen_XRealZero;
							Laser_ScreenY = Screen_YRealZero;
							Deal_EnLaserData(Laser_ScreenX,Laser_ScreenY);
						
						
					}
				}
			}
		}
		
		////////////// 排除视频暂停后,一段时间内没有再接收到目标信息,导致系统相对不动作的bug; ///////////////
		if((Video_timeflag==1)&&(Pic_Success==0))
		{
			if((Video_Mode==2) && (Video_timesec>=5))
			{
				Video_timeflag=0;
				Video_Control(Video_Play);
			}
			else if((Video_Mode==1) && (Video_timesec>=8))
			{
				Video_timeflag=0;
				Video_Control(Video_Play);
			}
			else if((Video_Mode==0) && (Video_timesec>=5))
			{
				Video_timeflag=0;
				Video_Control(Video_Play);
			}
		}
}

////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////


