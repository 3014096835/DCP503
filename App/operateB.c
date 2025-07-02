/*
 * @file          operate.c
 * @brief         ����: �Զ�������Ȳ��ֺ���;
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

unsigned char Run_FollowFlag=0;


/**********************************************************************************************************
* Function    ��void AutoPick_ModeSel(void)
* Description : .�Ѷ�ѡ�� ;
* Para        : none
* Return(s)   : none
* Example     : AutoPick_ModeSel();
**********************************************************************************************************/
void AutoPick_ModeSel(void)
{
		if(Matrixkey_val==KEY_VALD)			// "D";�˻�������;
		{
			Matrixkey_val=0;
			Set_flag=2;LCD_flag=2;
			LCD_DealData();
		}
		
		switch(Presskey_val)
		{
			case KEY_F1:// "F1";������ֹģʽ,��������Ϣͳ�ƽ���;
				// Set_flag/LCD_flag = 6. �Զ����( ʶ����ͣ��Ƶ�Ҽ��ⲻ�����˶� ) ;
					delay_ms(50);
					Video_Mode = 0;	Video_State = Video_Play;
					Set_flag=6;		LCD_flag=6;	//3.Auto Run Details;
					BUZZER_flag=3;	Auto_Running=1;Auto_RunTimeflag=1;Test_SpeedEn=0;
					Data_init();
					LCD_AutoEnter=1;	LCD_DealData();
				break;
			case KEY_F2:// "F2";��������ģʽ,��������Ϣͳ�ƽ���;
				// Set_flag/LCD_flag = 6. �Զ����( ʶ����ͣ��Ƶ�Ҽ��ⲻ�����˶� ) ;
					delay_ms(50);
					Video_Mode = 1;	Video_State = Video_Play;
					Set_flag=6;		LCD_flag=6;	//3.Auto Run Details;
					BUZZER_flag=3;	Auto_Running=1;Auto_RunTimeflag=1;Test_SpeedEn=0;
					Data_init();
					LCD_AutoEnter=1;	LCD_DealData();
				break;
			case KEY_F3:// "F3";��������ģʽ,��������Ϣͳ�ƽ���;
				// Set_flag/LCD_flag = 6. �Զ����( ʶ����ͣ��Ƶ�Ҽ��ⲻ�����˶� ) ;
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
* Function    ��void Keyset_AutoPick(void)
* Description : �Զ���⹦���µİ�������ʹ��;
* Para        : none
* Return(s)   : none
* Example     : Keyset_AutoPick();
**********************************************************************************************************/
void Keyset_AutoPick01(void)
{
		// "F1","F2","F3",�ֱ��Ӧ��ֹ/����/����ģʽ��,ϵͳ����������ֹͣ;
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

		switch(Matrixkey_val)	// ������������;
		{
			case KEY_UP:
				Page_Up=1;		LCD_DealData();
				break;// "Up";���Ϸ�ҳ;
			case KEY_DOWN:
				Page_Down=1;	LCD_DealData();
				break;// "Down";���·�ҳ;
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
			case KEY_VALD:
				Auto_Running=0;	Auto_RunTimeflag=0;
				Set_flag=5;	LCD_flag=5;		// "D";�˻�ģʽ����;
				LCD_DealData();
				Pic_Success=0;Laser_OpenFlag=0;Video_timeflag=0;
				break;// "D";// �˳�����;
			case KEY_VAL0:
					Laser_ScreenX = Screen_XRealZero;
					Laser_ScreenY = Screen_YRealZero;
				Deal_EnLaserData(Laser_ScreenX,Laser_ScreenY);
				break;// "0";// ת��(0,0)��;
			case KEY_VALE:
				break;// "E";
		}
		Matrixkey_val=0;
}


// ��ǰָʾ��Ʒ��Ϣ��¼;//20241020����;
void Deal_Materialmassage(unsigned int Followcnt)
{
	unsigned int addrcam=0;
	unsigned int addrmass=0;
	
		Material_Xinxi++;
		Material_TimeOut = Auto_RunTimesec;
		Material_PicX = Res_PosFollow[Followcnt][0];
		Material_PicY = Res_PosFollow[Followcnt][1];
		Material_Number = 1;
	
		if(Camera_mode==0)// =0,Ĭ�ϵ�ģ��ģʽ;=1,�߼�ʶ��ģʽ;
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
* Function    ��void AutoPick_ModeRun(void)
* Description : 4.Auto Run Details;
* Para        : none
* Return(s)   : none
* Example     : AutoPick_ModeRun();
**********************************************************************************************************/
void AutoPick_ModeRun(void)
{
		unsigned int ix1,iy1;//,iz1
	int Tmp_timeA,Tmp_timeB,Tmp_timeC;
	
		GS_StartFlag=0;				// �˶���Ƶ����;���������־λ;=1,����;
		Keyset_AutoPick01();
		
		////////// ���Ųɼ���������� ���ݴ���; //////////////
		if((Cam_Rxflag == 1)&&(Pic_Success==0))
		{
				Run_FollowUpdate=0;
				if(Auto_Running==1)	Auto_DataProcess(Arg_ReadAdj.RunDir,Video_Mode,0);
				
				// ȷ����Ŀ��,Ŀ��ȷ��;��¼��ǰĿ����Ϣ,��λĿ��λ��,������Ϣ����////����������;
				if(Pic_Success==1)
				{
					Video_Control(Video_Pause);
					Laser_OpenFlag=0;
					Laser_Off;
					
					// ��ǰָʾ��Ʒ��Ϣ��¼;//20241020�޸�;
					Deal_Materialmassage(Res_FollowNum);//ָʾĿ�����Ϣ�ֽ�;
					
						Laser_ScreenX = Material_PicX;
						Laser_ScreenY = Material_PicY;
					Deal_EnLaserData(Laser_ScreenX,Laser_ScreenY);
					
					Massage_Post();
				}
		}
		
		/////////// ����Ŀ������ //////////////
		if(Pic_Success==1)
		{
			// ������Ŀ������,���ƶ���Ŀ��λ�ú�,�򿪼����;// ��������;
			// ����ʹ�߳��� 5s ��,�رռ����,������Ϣ������Ƶ����,�ع鵽���λ��;
		
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
						
						// ��ǰָʾ��Ʒ��Ϣ��¼;//20241020�޸�;
					Deal_Materialmassage(Res_FollowNum);//ָʾĿ�����Ϣ�ֽ�;
						
						
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
						
						
						////////// ���� /////////////////
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
		
		////////////// �ų���Ƶ��ͣ��,һ��ʱ����û���ٽ��յ�Ŀ����Ϣ,����ϵͳ��Բ�������bug; ///////////////
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


