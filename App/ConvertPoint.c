/*
 * @file          operate.c
 * @brief         ����: �����桢ʱ�����ý��桢�ֶ��������桢�Զ������桢Һ����ʾ���ݷ��ͺ����Ȳ��ֺ���;
 * @ucos-ii       V2.86 
 * @function      QSS-ICBS1 
 * @compiler      RealView MDK-ARM Compiler  V5.14
 * @peripheral    V3.5.0
 * @author  	  	
 * @date   	      2024/05/01
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

// ��Ʒ������ݷ���;
const unsigned char Res_PosInit[5000]={0};	//�˿ڽ������ݻ����� 
int Res_PosDeal[60][20]={0};				// ��Ʒ�����,��ʷĿ�����껺��.��Ϣ;
unsigned int Res_Dealcnt=0;					// ��Ʒ�����,��ʷĿ�����껺��.����;
unsigned char Res_Dealover=0;				// ��Ʒ�����,��ʷĿ�����껺������ղ����ı��,�ǵ�һ������;
int Res_PosPre[60][20]={0};					// ��Ʒ�����,��ͬʱ����յ��������Ϣ����.��Ϣ;
unsigned int  Res_Precnt=0;					// ��Ʒ�����,��ͬʱ����յ��������Ϣ����.����;
int Res_PosTrack[60][3]={0};				// ��Ʒ�����,��Ʒ׷�ٵ㻺��.��Ϣ;
unsigned int Res_Trackcnt=0;				// ��Ʒ�����,��Ʒ׷�ٵ㻺��.����;
unsigned char Res_TrackACK[60]={0};	// ��Ʒ�����,��Ʒ׷�ٵ㻺��.״̬;�˵�����ɼ�����,��Ϊ1;����Ϊ0,��ʵ�м�����;
int Res_PosFollow[20][2]={0};				// ��Ʒ�����,��ǰ��⵽�Ŀ�������������Ʒ,����;
unsigned int Res_FollowSum=0;				// ��Ʒ�����,��ǰ��⵽�Ŀ�������������Ʒ,������;
unsigned int Res_FollowNum=0;				// ��Ʒ�����,��ǰ��⵽�Ŀ�������������Ʒ,��ǰ�Ѳ�������;

unsigned char Test_SpeedEn=0;			// ����Ƶ�ٶ�;ʹ�ܲ��ٹ���;
unsigned int Res_Speedcnt=0;			// ����Ƶ�ٶ�;�ٶ�ֵ�������;
int Res_SpeedBuf[10]={0};					// ����Ƶ�ٶ�;�ٶ�ֵ����ֵ;

int Tmp_DeBuffer[60][20]={0};			// ��Ʒ�����,�˴ν��յ�Ŀ�����껺��;
int Tmp_Track01[60][3]={0};				// �����׷�ٵ㵱ǰʱ���Ԥ��������Ϣ;
int Tmp_Track02[60][3]={0};				// ����׷�ٵ���и����ĸ�����Ϣ;
int Tmp_Track03[60][3]={0};				// ����׷�ٵ���޸�����������Ϣ;
int Tmp_TrackSort[60]={0};				// ��������Ԥ�������X�����������ֵ;
int Tmp_CheckX[20]={0};						// ���浱ǰ�ɼ�������;X����;
int Tmp_CheckY[20]={0};						// ���浱ǰ�ɼ�������;Y����;
int Tmp_PicX[20]={0};							// ���浱ǰ��ָʾ�����;Y����;
int Tmp_PicY[20]={0};							// ���浱ǰ��ָʾ�����;Y����;
unsigned int Pixel_range=10;	// ���ص�ƫ�Ʒ�Χ;�˷�Χ������ͬһ��Ʒ;//��Ϊmm����,��ȡ����10mm������Ϊ��ֹ��Ʒ;

int Tmp_CheckSource[20]={0};						// ���浱ǰ�ɼ�������;Y����;
int Tmp_PicSource[20]={0};							// ���浱ǰ��ָʾ�����;Y����;
int Res_FollowSource[20]={0};							// ��Ʒ�����,��ǰ��⵽�Ŀ�������������Ʒ,����;

////////// ���� /////////////////
int Run_FollowData[3];
unsigned char Run_FollowAddr;
unsigned char Run_FollowUpdate;


void Data_init(void)
{
	Res_Dealcnt=0;
	Res_Dealover=0;// ����Res_PosInitΪunsigned char����,int�ͱ�����0ʱ�����4��;
	memcpy(Res_PosDeal, Res_PosInit, sizeof(Res_PosDeal));
	Res_Precnt=0;
	memcpy(Res_PosPre, Res_PosInit, sizeof(Res_PosPre));
	Res_Trackcnt=0;
	memcpy(Res_PosTrack, Res_PosInit, sizeof(Res_PosTrack));
	memcpy(Res_TrackACK, Res_PosInit, sizeof(Res_TrackACK));
	Res_FollowSum=0;
	Res_FollowNum=0;
	memcpy(Res_PosFollow, Res_PosInit, sizeof(Res_PosFollow));
	Res_Speedcnt=0;
	memcpy(Res_SpeedBuf, Res_PosInit, sizeof(Res_SpeedBuf));
}

void Data_InitToAuto(void)
{
	memcpy(Tmp_Track01, Res_PosInit, sizeof(Tmp_Track01));
	memcpy(Tmp_Track02, Res_PosInit, sizeof(Tmp_Track02));
	memcpy(Tmp_Track03, Res_PosInit, sizeof(Tmp_Track03));
	memcpy(Tmp_CheckX, Res_PosInit, sizeof(Tmp_CheckX));
	memcpy(Tmp_CheckY, Res_PosInit, sizeof(Tmp_CheckY));
	memcpy(Tmp_PicX, Res_PosInit, sizeof(Tmp_PicX));
	memcpy(Tmp_PicY, Res_PosInit, sizeof(Tmp_PicY));
}	

////////////////////////////////////////////////////////

/**********************************************************************************************************
* Function    ��unsigned int Data_AsciiToDec(unsigned char DataTmp)  
* Description : ��ASCIIת����һ��ʮ��������;
* Para        : none
* Return(s)   : none
* Example     : Data_AsciiToDec(DataTmp);
**********************************************************************************************************/
unsigned int Data_AsciiToDec(unsigned char DataTmp)
{
     uint16_t Result;
        if((DataTmp>0x30)&&(DataTmp<=0x39))  Result = DataTmp - 0x30;
        else   Result = 0x00;       // ���ݴ������ 
     return  Result;
}





/**********************************************************************************************************
* Function    ��void Auto_DataProcess(uint8_t M_VideoDir,uint8_t M_VideoMode,uint8_t M_SpeedView)
* Description : ��������,���ݸ��洦����;
* Para        : none
* Return(s)   : none
* Example     : void Auto_DataProcess(uint8_t M_VideoDir,uint8_t M_VideoMode,uint8_t M_SpeedView)
**********************************************************************************************************/
void Auto_DataProcess(uint8_t M_VideoDir,uint8_t M_VideoMode,uint8_t M_SpeedView)
{
	static unsigned char Pulse_Flag=0;// �״���ͣ����Ϊ1,��ͣ������Ϊ1�򲻽������ݽ���,�ȴ���0�����;
		unsigned int i1,j1,k1;//
		// Tmp_De;�ֽ�˴ν��յ���������Ϣʱʹ��;
		unsigned int Tmp_Des01,Tmp_Dex,Tmp_Dey;
		unsigned int Tmp_Desum,Tmp_Decnt;	// Tmp_Desum��¼�˴���Ϣ����������;Tmp_Decnt��¼���һ����Ϣ�ڵ���Ʒ����;
		// Tmp_Track;����׷�ٵ����ʱʹ��;
		unsigned char loop_nA; 	// �������ʱ����Ϣ����Ʒ����;
		unsigned char Inside_flagA,Inside_flagB; 	// ��Ʒ�ڶ�Ӧ����������ʹ��;
		int	Tmp_ax,Tmp_ay,Tmp_bx,Tmp_by,Tmp_cx;		// �ֱ𻺴�������/������/Ԥ������;
		int Sort_datatmp;						// ��������ʱ,��������������;
		unsigned int Sort_addrtmp;								// ��������ʱ,�������������ݵ�ַ;
		unsigned char Tmp_TrackSortAddr[10]={0};	// ���������,��������������������ַ;
		unsigned char Tmp_Track02CH[10]={0};			// Tmp_Track02�����������,ͬ����ǵ�ǰλ���Ƿ񻺴�������;
		unsigned int Tmp_Tracknum3=0;				// ����׷�ٵ���޸��������ӵ�����;
		unsigned char Tmp_TrackEN[10]={0};	// ����׷�ٵ��,�Ƿ��ʹ�õı��;
		unsigned char Tmp_TrackOver[10]={0};	// ����׷�ٵ��,�Ƿ��ʹ�õı��;
		unsigned int Tmp_Tracknum4=0;				// ����׷�ٵ��,ɾ������׷�ٺ��׷�ٵ�����;
		long int Tmp_Timold,Tmp_Timnew,Tmp_Timstep,Tmp_Timdelay;		// ����Ϣʱ��,����Ϣʱ��;// �¾���Ϣʱ���;
		// Tmp_Check;������Ʒ���ʱʹ��;
		unsigned int Tmp_Checknum=0;
		// Tmp_Pic;������Ʒ���ʱʹ��;
		unsigned char Tmp_Picflag=0;
		unsigned char Tmp_Picflagcnt=0;
// 		unsigned char Tmp_PicEN[10]={0};
		unsigned int Tmp_Picnum=0;
		// Tmp_Spd;�����˶��ٶ�ʱʹ��;
		int Tmp_SpdBuf[10]={0};
		long int Tmp_SpdTrackold,Tmp_SpdTracknew,Tmp_SpdTrackstep;
		long int Tmp_SpdTimold,Tmp_SpdTimnew,Tmp_SpdTimstep;
		
		if((Screen_OriginPosition == 2) ||(Screen_OriginPosition == 2))// �Ҳ�Ϊ���ʱ,��������ת;
		{
			if(M_VideoDir == Dir_Leftdown)	M_VideoDir = Dir_Rightup;
				else	M_VideoDir = Dir_Leftdown;
		}
		
		if(Pulse_Flag==0)
		{
				// Step_01;
				// ����������Ϣ,��������㼰����ʱ�����Res_PosBuf[][];
				// ͬʱ,�õ���Ϣ���� Tmp_Desum,����ǰ���һ����Ϣ��Ʒ����Tmp_Decnt;
				// ASCII����ת��Ϊʮ����������,����ֵ(Tmp_Dex,Tmp_Dey);
				Tmp_Des01=0;	Tmp_Dex=0;	Tmp_Dey=0;
				Tmp_Desum=0;	Tmp_Decnt=0;
				memcpy(Tmp_DeBuffer, Res_PosInit, sizeof(Tmp_DeBuffer));
			
			// AI�߼�ʶ��ģʽ��ͨѶЭ���޸�/������⵽����Ʒ���м���������¼;//20241020�޸�;
			if(Cam_RxBuffer[0] == 'T')
			{
				Camera_RxType.Flag=1;Camera_RxType.Cnt=0;Camera_RxType.Typelist[Camera_RxType.Cnt]=0;
				for(i1=2;i1<2048;i1++)
				{
					if(Cam_RxBuffer[i1] == ';')
					{
						if(Camera_RxType.Flag==0)
						{
							if(Tmp_Des01==0)	Tmp_Des01=1;
							else
							{
								Tmp_Des01=0;
								Tmp_Decnt++;
								Tmp_DeBuffer[Tmp_Desum][0] = Tmp_Decnt;
								Tmp_DeBuffer[Tmp_Desum][2*(Tmp_Decnt-1)+1] = Tmp_Dex;
								Tmp_DeBuffer[Tmp_Desum][2*(Tmp_Decnt-1)+2] = Tmp_Dey;
								Camera_RxType.pointx[Camera_RxType.Cnt] = Tmp_Dex;
								Camera_RxType.pointy[Camera_RxType.Cnt] = Tmp_Dey;
								Camera_RxType.Flag=1;
								Camera_RxType.Cnt++;
								Camera_RxType.Typelist[Camera_RxType.Cnt]=0;
								Tmp_Dex=0;	Tmp_Dey=0;
								if((Cam_RxBuffer[i1+1] == 0x0D) && (Cam_RxBuffer[i1+2] == 0X0A))
								{
									if(Cam_RxBuffer[i1+3] != 0)
									{
										Tmp_DeBuffer[Tmp_Desum][2*(Tmp_Decnt-1)+3] = Auto_RunTimemisec;
										if((Cam_RxBuffer[i1+3] == 'N') && (Cam_RxBuffer[i1+4] == 'G'))
										{
											Tmp_Desum++;	
											break;
										}
										if(Tmp_Desum<19)
										{
											//Tmp_Desum++;	Tmp_Decnt=0;
											//i1 = i1+6;
											if((Cam_RxBuffer[i1+3] == 'T') && (Cam_RxBuffer[i1+4] == ';') && (Cam_RxBuffer[i1+5] != 0))
											{
												Tmp_Desum++;	Tmp_Decnt=0;
												Camera_RxType.Flag=1;Camera_RxType.Cnt++;Camera_RxType.Typelist[Camera_RxType.Cnt]=0;
												i1 = i1+5;
											}
											else {Tmp_Desum++;break;}
										}
										else {Tmp_Desum++;break;}
									}
									else
									{
										Tmp_DeBuffer[Tmp_Desum][2*(Tmp_Decnt-1)+3] = Auto_RunTimemisec;
											Tmp_Desum++;
										break;
									}
								}
							}
						}
						else	Camera_RxType.Flag=0;

					}
					else if((Cam_RxBuffer[i1] == 0x0D) || (Cam_RxBuffer[i1] == 0X0A)){}
					else
					{
						if(Camera_RxType.Flag==1)
						{
							Camera_RxType.Typelist[Camera_RxType.Cnt] = Camera_RxType.Typelist[Camera_RxType.Cnt]*10 + Data_AsciiToDec(Cam_RxBuffer[i1]);
						}
						else
						{
						if(Tmp_Des01==0)				{	Tmp_Dex = Tmp_Dex*10 + Data_AsciiToDec(Cam_RxBuffer[i1]);	}
						else if(Tmp_Des01==1)		{	Tmp_Dey = Tmp_Dey*10 + Data_AsciiToDec(Cam_RxBuffer[i1]);	}
						}
					}
				}
			}
			else
			{
				// ��ģ��ʶ��ģʽ��������⵽����Ʒ��״��ɫ��¼;//20241020�޸�;
				Camera_RxType.Shape = Data_AsciiToDec(Cam_RxBuffer[0]);
				Camera_RxType.Color = Data_AsciiToDec(Cam_RxBuffer[2]);
				
				for(i1=4;i1<2048;i1++)
				{
					if(Cam_RxBuffer[i1] == ';')
					{
						if(Tmp_Des01==0)	Tmp_Des01=1;
						else
						{
							Tmp_Des01=0;
							Tmp_Decnt++;
							Tmp_DeBuffer[Tmp_Desum][0] = Tmp_Decnt;
							Tmp_DeBuffer[Tmp_Desum][2*(Tmp_Decnt-1)+1] = Tmp_Dex;
							Tmp_DeBuffer[Tmp_Desum][2*(Tmp_Decnt-1)+2] = Tmp_Dey;
							Tmp_Dex=0;	Tmp_Dey=0;
							if((Cam_RxBuffer[i1+1] == 0x0D) && (Cam_RxBuffer[i1+2] == 0X0A))
							{
								if(Cam_RxBuffer[i1+3] != 0)
								{
									Tmp_DeBuffer[Tmp_Desum][2*(Tmp_Decnt-1)+3] = Auto_RunTimemisec;
									if((Cam_RxBuffer[i1+3] == 'N') && (Cam_RxBuffer[i1+4] == 'G'))
									{
										Tmp_Desum++;	
										break;
									}
									if(Tmp_Desum<19)
									{
										//Tmp_Desum++;	Tmp_Decnt=0;
										//i1 = i1+6;
										if((Cam_RxBuffer[i1+4] == ';') && (Cam_RxBuffer[i1+6] == ';') && (Cam_RxBuffer[i1+7] != 0))
										{
											Tmp_Desum++;	Tmp_Decnt=0;
											i1 = i1+6;
										}
										else {Tmp_Desum++;break;}
									}
									else {Tmp_Desum++;break;}
								}
								else
								{
									Tmp_DeBuffer[Tmp_Desum][2*(Tmp_Decnt-1)+3] = Auto_RunTimemisec;
										Tmp_Desum++;
									break;
								}
							}
						}
					}
					else if((Cam_RxBuffer[i1] == 0x0D) || (Cam_RxBuffer[i1] == 0X0A)){}
					else
					{
						if(Tmp_Des01==0)				{	Tmp_Dex = Tmp_Dex*10 + Data_AsciiToDec(Cam_RxBuffer[i1]);	}
						else if(Tmp_Des01==1)		{	Tmp_Dey = Tmp_Dey*10 + Data_AsciiToDec(Cam_RxBuffer[i1]);	}
					}
				}
			}
				// Step_02;
				// ������������Ϣ������Ϣ�������� Res_PosDeal[][],����Ϣ������ Res_Dealcnt;
				// ������Ϣ��������ﵽ�������,ȡ���2����Ϣ������λ,����ռ����;�������������Ӵ˴���Ϣ��;
				if(Res_Dealcnt>=50)
				{
					for(j1=0;j1<20;j1++)
					{
						Res_PosDeal[0][j1] = Res_PosDeal[Res_Dealcnt-2][j1];
					}
					for(j1=0;j1<20;j1++)
					{
						Res_PosDeal[1][j1] = Res_PosDeal[Res_Dealcnt-1][j1];
					}
					Res_Dealcnt=2;
					Res_Dealover=1;
					for(i1=Res_Dealcnt;i1<60;i1++)
					{
						for(j1=0;j1<20;j1++)
						{
							Res_PosDeal[i1][j1] = 0;
						}
					}
				}
				for(i1=0;i1<Tmp_Desum;i1++)
				{
					for(j1=0;j1<20;j1++)
					{
						Res_PosDeal[Res_Dealcnt][j1] = Tmp_DeBuffer[i1][j1];
					}
					Res_Dealcnt++;
				}
				// Step_03;
				// ���������Ϣ����Ʒ��ǰʱ�䵱ǰλ��,�������ų�׷�ٵ���Ʒ;
				// ������Ϣ��������ﵽ�������,ȡ���1����Ϣ������λ,����ռ����;�������������Ӵ˴���Ϣ��;
				if(Res_Precnt>=50)
				{
					for(j1=0;j1<20;j1++)
					{
						Res_PosPre[0][j1] = Res_PosPre[Res_Precnt-1][j1];
					}
					Res_Precnt=1;
					for(i1=Res_Precnt;i1<60;i1++)
					{
						for(j1=0;j1<20;j1++)
						{
							Res_PosPre[i1][j1] = 0;
						}
					}
				}
				for(j1=0;j1<20;j1++)
				{
					Res_PosPre[Res_Precnt][j1]=Res_PosDeal[Res_Dealcnt-1][j1];
				}
				Res_Precnt++;
				
				Data_InitToAuto();// �����������������ݻ���;
				
				// Step_04;
				// ������Ʒ׷��;Res_PosPre�����õ�׷����ϢRes_PosTrack;
				if(Res_Trackcnt == 0)
				{
					// Step_04A;
					// ����Ʒ�޶Ա�����ʱ,��Ʒ׷�ٵ�ֱ�����Ӽ�¼;
					loop_nA=Res_PosDeal[Res_Dealcnt-1][0];
					for(i1=0;i1<loop_nA;i1++)
					{
						Res_PosTrack[i1][0]=Res_PosDeal[Res_Dealcnt-1][2*i1+1];
						Res_PosTrack[i1][1]=Res_PosDeal[Res_Dealcnt-1][2*i1+2];
						Res_PosTrack[i1][2]=Res_PosDeal[Res_Dealcnt-1][2*loop_nA+1];
						Tmp_TrackEN[i1]=1;
					}
					Res_Trackcnt=loop_nA;
					// Step_04A end;
				}
				else
				{
					// Step_04B-1;
					// ����Ʒ�жԱ�����ʱ,��Ʒ׷�ٵ��ж�ʶ��;
					// �����׷�ٵ㵱ǰʱ���Ԥ������;
					loop_nA=Res_PosDeal[Res_Dealcnt-1][0];
					Tmp_Timnew=Res_PosDeal[Res_Dealcnt-1][2*loop_nA+1];
					
					// Step_04B-4;
					// Ԥ�����곬��һ��ʱ���,�����׷�������,�˵�׷�ٽ���;����ֹ״̬;
						if(M_VideoMode==0)
						{
							for(i1=0;i1<Res_Trackcnt;i1++)
							{
								Tmp_Timold=Res_PosTrack[i1][2];
								if(Tmp_Timold>Tmp_Timnew)
								{
									Tmp_Timstep=65536+Tmp_Timnew-Tmp_Timold;
								}
								else
								{
									Tmp_Timstep=Tmp_Timnew-Tmp_Timold;
								}
								if(Tmp_Timstep>15000)
								{
									Tmp_TrackOver[i1]=1;
								}
							}
						}
					for(i1=0;i1<Res_Trackcnt;i1++)
					{
						if(M_VideoMode==0)
						{
							Tmp_Track01[i1][0]=Res_PosTrack[i1][0];
							Tmp_Track01[i1][1]=Res_PosTrack[i1][1];
							Tmp_Track01[i1][2]=Tmp_Timnew;
						}
						else
						{
							Tmp_Timold=Res_PosTrack[i1][2];
							if(Tmp_Timold>Tmp_Timnew)
							{
								Tmp_Timstep=65536+Tmp_Timnew-Tmp_Timold;
							}
							else
							{
								Tmp_Timstep=Tmp_Timnew-Tmp_Timold;
							}
							Tmp_Timdelay = Tmp_Timstep/1000.0*(Arg_ReadAdj.HighSpeed*10);
							if(M_VideoDir == Dir_Rightup)
							{
								Tmp_Track01[i1][0]=Res_PosTrack[i1][0]+Tmp_Timdelay;
							}
							else
							{
								Tmp_Track01[i1][0]=Res_PosTrack[i1][0]-Tmp_Timdelay;
							}
							Tmp_Track01[i1][1]=Res_PosTrack[i1][1];
							Tmp_Track01[i1][2]=Tmp_Timnew;
						}
					}
					// Ԥ��������X������ֵ����;
					for(i1=0;i1<Res_Trackcnt;i1++)
					{
						Tmp_TrackSortAddr[i1]=i1;
						Tmp_TrackSort[i1]=Tmp_Track01[i1][0];
					}
					if(Res_Trackcnt>1)
					{
						for(i1=0;i1<Res_Trackcnt-1;i1++)
						{
							for(j1=0;j1<Res_Trackcnt-1-i1;j1++)
							{
								if(M_VideoDir == Dir_Rightup)
								{
									if(Tmp_TrackSort[j1] < Tmp_TrackSort[j1+1])
									{
										Sort_datatmp=Tmp_TrackSort[j1];
										Tmp_TrackSort[j1]=Tmp_TrackSort[j1+1];
										Tmp_TrackSort[j1+1]=Sort_datatmp;
										
										Sort_addrtmp=Tmp_TrackSortAddr[j1];
										Tmp_TrackSortAddr[j1]=Tmp_TrackSortAddr[j1+1];
										Tmp_TrackSortAddr[j1+1]=Sort_addrtmp;
									}
								}
								else
								{
									if(Tmp_TrackSort[j1] > Tmp_TrackSort[j1+1])
									{
										Sort_datatmp=Tmp_TrackSort[j1];
										Tmp_TrackSort[j1]=Tmp_TrackSort[j1+1];
										Tmp_TrackSort[j1+1]=Sort_datatmp;
										
										Sort_addrtmp=Tmp_TrackSortAddr[j1];
										Tmp_TrackSortAddr[j1]=Tmp_TrackSortAddr[j1+1];
										Tmp_TrackSortAddr[j1+1]=Sort_addrtmp;
									}
								}
							}
						}
					}
					
					// Step_04B-2;
					// �ж���Ʒ����״̬;
					Tmp_Tracknum3=0;
					for(i1=0;i1<loop_nA;i1++)
					{
						Inside_flagA=0;
						Tmp_ax = Res_PosDeal[Res_Dealcnt-1][2*i1+1];
						Tmp_ay = Res_PosDeal[Res_Dealcnt-1][2*i1+2];
						for(j1=0;j1<Res_Trackcnt;j1++)
						{
							Inside_flagB=0;
							Sort_addrtmp=Tmp_TrackSortAddr[j1];
							Tmp_bx = Res_PosTrack[Sort_addrtmp][0];
							Tmp_by = Res_PosTrack[Sort_addrtmp][1];
							Tmp_cx = Tmp_Track01[Sort_addrtmp][0];
							if((fabs(Tmp_ax-Tmp_bx)<Pixel_range) && (fabs(Tmp_ay-Tmp_by)<Pixel_range))		{	Inside_flagB++;	}
							else if((fabs(Tmp_ay-Tmp_by)<Pixel_range) &&(M_VideoMode!=0))
							{
								if(M_VideoDir == Dir_Rightup)
								{
									if((Tmp_ax>(Tmp_cx-40)))		{	Inside_flagB++;	}
									else if((Tmp_ax>Tmp_bx) && (Tmp_ax<Tmp_cx))			{	Inside_flagB++;	}
								}
								else
								{
									if((Tmp_ax<(Tmp_cx+40)))		{	Inside_flagB++;	}
									else if((Tmp_ax>Tmp_cx) && (Tmp_ax<Tmp_bx))			{	Inside_flagB++;	}
								}
							}
							if((Inside_flagA==0)&&(Tmp_Track02CH[Sort_addrtmp]==0)&&(Inside_flagB!=0))
							{
								if(Tmp_TrackOver[Sort_addrtmp]==0)
								{
								Inside_flagA++;
								Tmp_Track02[Sort_addrtmp][0]=Tmp_ax;
								Tmp_Track02[Sort_addrtmp][1]=Tmp_ay;
								Tmp_Track02[Sort_addrtmp][2]=Tmp_Timnew;
								Tmp_Track02CH[Sort_addrtmp]=1;
								}
							}
							if(Inside_flagA!=0) break;
						}
						if(Inside_flagA==0)
						{
							Tmp_Track03[Tmp_Tracknum3][0]=Tmp_ax;
							Tmp_Track03[Tmp_Tracknum3][1]=Tmp_ay;
							Tmp_Track03[Tmp_Tracknum3][2]=Tmp_Timnew;
							Tmp_Tracknum3++;
						}
					}
					
					// Step_04B-3;
					// �жϸ�׷�ٵ㵱ǰ�Ƿ����Ϊ����Ʒ����ֹ�ж�EN;
					for(i1=0;i1<Res_Trackcnt;i1++)
					{
						if((Tmp_Track02[i1][0]!=0) && (Tmp_Track02[i1][1]!=0))
						{
							// �����ٶȼ���;
							//M_SpeedView;
							if((i1==0)&&(M_SpeedView==1))
							{
								Tmp_SpdTrackold = Res_PosTrack[i1][0];
								Tmp_SpdTracknew = Tmp_Track02[i1][0];
								if(M_VideoDir == Dir_Rightup)
									Tmp_SpdTrackstep = Tmp_SpdTracknew-Tmp_SpdTrackold;
								else
									Tmp_SpdTrackstep = Tmp_SpdTrackold-Tmp_SpdTracknew;
								
								Tmp_SpdTimold = Res_PosTrack[i1][2];
								Tmp_SpdTimnew = Tmp_Track02[i1][2];
								if(Tmp_SpdTimnew>=Tmp_SpdTimold)
									Tmp_SpdTimstep = Tmp_SpdTimnew-Tmp_SpdTimold;
								else
									Tmp_SpdTimstep = 65536+Tmp_SpdTimnew-Tmp_SpdTimold;
								
								Tmp_SpdBuf[i1] = Tmp_SpdTrackstep*1000.0/Tmp_SpdTimstep;
								Res_SpeedBuf[Res_Speedcnt] = Tmp_SpdBuf[0];
								Res_Speedcnt++;
								if(Res_Speedcnt>=10)	Res_Speedcnt=0;
							}
							
							// ��Ϣ����;
							Res_PosTrack[i1][0]=Tmp_Track02[i1][0];
							Res_PosTrack[i1][1]=Tmp_Track02[i1][1];
							Res_PosTrack[i1][2]=Tmp_Track02[i1][2];
							if(Res_TrackACK[i1]==0)
							{
								Tmp_TrackEN[i1]=1;
							}
							
							if((Run_FollowAddr != 0) && (i1 == (Run_FollowAddr-1)))
							{
								////////// ���� /////////////////
								Run_FollowData[0] = Res_PosTrack[Run_FollowAddr-1][0];
								Run_FollowData[1] = Res_PosTrack[Run_FollowAddr-1][1];
								Run_FollowData[2] = Res_PosTrack[Run_FollowAddr-1][2];
								Run_FollowUpdate=1;
							}
						}
					}
					
					// Step_04B-4;
					// Ԥ�����곬����Ļ��Χ��,�����׷�������,�˵�׷�ٽ���;���˶�״̬;
					if(M_VideoMode!=0)
					{
						Tmp_Tracknum4=Res_Trackcnt;
						for(i1=0;i1<Res_Trackcnt;i1++)
						{
							if((Tmp_Track02[i1][0]==0) && (Tmp_Track02[i1][1]==0))
							{
// 							if(((M_VideoDir==Dir_Rightup)&&(Tmp_Track01[i1][0]>3900))||((M_VideoDir==Dir_Leftdown)&&(Tmp_Track01[i1][0]<-600)))
							if(((M_VideoDir==Dir_Rightup)&&(Tmp_Track01[i1][0]>2000))||((M_VideoDir==Dir_Leftdown)&&(Tmp_Track01[i1][0]<-1500)))
							{
								for(j1=i1-(Res_Trackcnt-Tmp_Tracknum4);j1<(Tmp_Tracknum4-1);j1++)
								{
									Res_PosTrack[j1][0]=Res_PosTrack[j1+1][0];
									Res_PosTrack[j1][1]=Res_PosTrack[j1+1][1];
									Res_PosTrack[j1][2]=Res_PosTrack[j1+1][2];
									Res_TrackACK[j1]=Res_TrackACK[j1+1];
									Tmp_TrackEN[j1]=Tmp_TrackEN[j1+1];
								}
								Res_PosTrack[Tmp_Tracknum4-1][0]=0;
								Res_PosTrack[Tmp_Tracknum4-1][1]=0;
								Res_PosTrack[Tmp_Tracknum4-1][2]=0;
								Res_TrackACK[Tmp_Tracknum4-1]=0;
								Tmp_TrackEN[Tmp_Tracknum4-1]=0;
								Tmp_Tracknum4--;
								
								if((Run_FollowAddr != 0) && (i1 == (Run_FollowAddr-1)))
								{
									////////// ���� /////////////////
									Run_FollowData[0] = 0;
									Run_FollowData[1] = 0;
									Run_FollowData[2] = 0;
									Run_FollowAddr=0;
									Run_FollowUpdate=0;
								}
								else if((Run_FollowAddr != 0) && (i1 < (Run_FollowAddr-1)))
								{
									Run_FollowAddr=Run_FollowAddr-1;
								}
							}
							}
						}
						Res_Trackcnt=Tmp_Tracknum4;
					}
					// Step_04B-4;
					// Ԥ�����곬��һ��ʱ���,�����׷�������,�˵�׷�ٽ���;����ֹ״̬;
					else if(M_VideoMode==0)
					{
						Tmp_Tracknum4=Res_Trackcnt;
						for(i1=0;i1<Res_Trackcnt;i1++)
						{
							Tmp_Timold=Res_PosTrack[i1-(Res_Trackcnt-Tmp_Tracknum4)][2];
							if(Tmp_Timold>Tmp_Timnew)
							{
								Tmp_Timstep=65536+Tmp_Timnew-Tmp_Timold;
							}
							else
							{
								Tmp_Timstep=Tmp_Timnew-Tmp_Timold;
							}
							if(Tmp_Timstep>15000)
							{
								for(j1=i1-(Res_Trackcnt-Tmp_Tracknum4);j1<(Tmp_Tracknum4-1);j1++)
								{
									Res_PosTrack[j1][0]=Res_PosTrack[j1+1][0];
									Res_PosTrack[j1][1]=Res_PosTrack[j1+1][1];
									Res_PosTrack[j1][2]=Res_PosTrack[j1+1][2];
									Res_TrackACK[j1]=Res_TrackACK[j1+1];
									Tmp_TrackEN[j1]=Tmp_TrackEN[j1+1];
								}
								Res_PosTrack[Tmp_Tracknum4-1][0]=0;
								Res_PosTrack[Tmp_Tracknum4-1][1]=0;
								Res_PosTrack[Tmp_Tracknum4-1][2]=0;
								Res_TrackACK[Tmp_Tracknum4-1]=0;
								Tmp_TrackEN[Tmp_Tracknum4-1]=0;
								Tmp_Tracknum4--;
								
								if((Run_FollowAddr != 0) && (i1 == (Run_FollowAddr-1)))
								{
									////////// ���� /////////////////
									Run_FollowData[0] = 0;
									Run_FollowData[1] = 0;
									Run_FollowData[2] = 0;
									Run_FollowAddr=0;
									Run_FollowUpdate=0;
								}
								else if((Run_FollowAddr != 0) && (i1 < (Run_FollowAddr-1)))
								{
									Run_FollowAddr=Run_FollowAddr-1;
								}
							}
						}
						Res_Trackcnt=Tmp_Tracknum4;
					}
					
					// Step_04B-5;
					// ����׷����Ʒ;��������Ʒ������������Ϊ������Ʒ;
					if((Tmp_Tracknum3!=0)&&((Res_Trackcnt+Tmp_Tracknum3)<=60))
					{
						for(i1=0;i1<Tmp_Tracknum3;i1++)
						{
							Res_PosTrack[Res_Trackcnt+i1][0] = Tmp_Track03[i1][0];
							Res_PosTrack[Res_Trackcnt+i1][1] = Tmp_Track03[i1][1];
							Res_PosTrack[Res_Trackcnt+i1][2] = Tmp_Track03[i1][2];
						}
						for(i1=Res_Trackcnt;i1<(Res_Trackcnt+Tmp_Tracknum3);i1++)
						{
							Tmp_TrackEN[i1]=1;
						}
						Res_Trackcnt = Res_Trackcnt+Tmp_Tracknum3;
					}
					
					
					// Step_04B end;
				}
				
				// Step_05;
				// ȷ���ɼ����֤����Ʒ;
				Tmp_Checknum=0;
				for(i1=0;i1<Res_Trackcnt;i1++)
				{
					if(Tmp_TrackEN[i1]==1)
					{
						Tmp_CheckX[Tmp_Checknum]=Res_PosTrack[i1][0];
						Tmp_CheckY[Tmp_Checknum]=Res_PosTrack[i1][1];
						Tmp_CheckSource[Tmp_Checknum]=i1;
						Tmp_Checknum++;
					}
				}
				
				// Step_06;
				// ������Ʒ��ⶨλ;
				if((Auto_Running==1)&&(Tmp_Checknum!=0))
				{
					if(GS_StartFlag==1)//(Video_Mode==1)// ��������ʱ��ָʾ����ȷ��;
					{
						// Step_06-1;
						// ����Ϣ����,���������ָʾ;
						{
							Tmp_Picnum=0;
							for(i1=0;i1<Tmp_Checknum;i1++)
							{
									Tmp_PicX[Tmp_Picnum] = Tmp_CheckX[i1];
									Tmp_PicY[Tmp_Picnum] = Tmp_CheckY[i1];
									Tmp_PicSource[Tmp_Picnum] = Tmp_CheckSource[i1];
									Tmp_Picnum++;
							}
							if(Tmp_Picnum>0)	
							{
								Pic_Success=1;
								Res_FollowSum=Tmp_Picnum;
								Res_FollowNum=0;
								for(i1=0;i1<Res_FollowSum;i1++)
								{
									Res_PosFollow[i1][0]=Tmp_PicX[i1];
									Res_PosFollow[i1][1]=Tmp_PicY[i1];
									Res_FollowSource[i1] = Tmp_PicSource[i1];
								}
							}
						}
					}
					else// ����ָʾ�Ĳ���ȷ��;
					{
						if(M_SpeedView==0)
						{
							if(Video_State != Video_Pause)
							{
								Pulse_Flag=1;
								Res_Dealcnt=0;	Res_Dealover=0;
								memcpy(Res_PosDeal, Res_PosInit, sizeof(Res_PosDeal));// ����Res_PosInitΪunsigned char����,int�ͱ�����0ʱ�����4��;
								delay_ms(50);
							}
							
							Video_Control(Video_Pause);
						}
						
						// Step_06-1;
						// ����Ϣ�������ڵ���3,���ж���Ʒ�Ƿ�ֹ״̬,��Ϊ��ֹ,���������ָʾ;
						// ����Ƶ�ս�����жϵ�����,���ڸ��������ʱ���ų�;��Ƶ�������޷��ų�(��ʹ�ü����ָͣ������);
						if(Res_Dealcnt>=10)		// ��ģ��/��ģ��AIʶ�����-�ֲ������޸�;//20241020�޸�;
						{
							Tmp_Picnum=0;
							for(i1=0;i1<Tmp_Checknum;i1++)
							{
								Tmp_Picflagcnt=0;
								for(j1=0;j1<5;j1++)// �ӳ��Լ�(N-1)������˳��(N-2)��ȡ5��ֵ�Ƚ�,������6��ֵ�ж�;
								{
									Tmp_Picflag=0;
									for(k1=0;k1<Res_PosDeal[Res_Dealcnt-2-j1][0];k1++)
									{
										// ��ģ��/��ģ��AIʶ�����-�ֲ������޸�;//20241020�޸�;
										if( (fabs(Tmp_CheckX[i1]-Res_PosDeal[Res_Dealcnt-2-j1][2*k1+1])<3) && (fabs(Tmp_CheckY[i1]-Res_PosDeal[Res_Dealcnt-2-j1][2*k1+2])<3) )
										{
											Tmp_Picflag=1;
										}
									}
									
									if(Tmp_Picflag==1)	
										Tmp_Picflagcnt++;
									if(Tmp_Picflagcnt>=2) 
										break;
									if((Res_Dealcnt-2-j1)==0) 
										break;
								}
								if(Tmp_Picflagcnt>=2)
								{
									Tmp_PicX[Tmp_Picnum] = Tmp_CheckX[i1];
									Tmp_PicY[Tmp_Picnum] = Tmp_CheckY[i1];
									Tmp_Picnum++;
								}
							}
							if(Tmp_Picnum>0)	
							{
								Pic_Success=1;
								Res_FollowSum=Tmp_Picnum;
								Res_FollowNum=0;
								for(i1=0;i1<Res_FollowSum;i1++)
								{
									Res_PosFollow[i1][0]=Tmp_PicX[i1];
									Res_PosFollow[i1][1]=Tmp_PicY[i1];
								}
							}
						}
					}
				}
				
		}
		else
		{
			Pulse_Flag++;
			if(Pulse_Flag>3)Pulse_Flag=0;
		}
}




/////////////////////////////////////////////////////////////////////////////////



long	Plot_Xset[16] ={40,240,440,640, 640,440,240,40, 40,240,440,640, 640,440,240,40};//
long	Plot_Xreal[16]={40,240,440,640, 640,440,240,40, 40,240,440,640, 640,440,240,40};// Y��У׼��;

long	Plot_Yset[16] ={0,0,0,0, 120,120,120,120, 240,240,240,240, 360,360,360,360};//
long	Plot_Yreal[16]={0,0,0,0, 120,120,120,120, 240,240,240,240, 360,360,360,360};// Y��У׼��;

long	Plot_XsetA[4] ={0};	//
long	Plot_XrealA[4]={0};//
long	Plot_YsetA[4] ={0};	//
long	Plot_YrealA[4]={0};//

long	Plot_XrealReturn =0;//
long	Plot_YrealReturn=0;// Y��У׼��;

////////////////////////////////////////////////////////////////////////////////////
void  Position( long    x, long    y, long *  p, long *  q,
	              long *  XX, long *  YY)
{
   float  Qi_0=0,Pi_0=0,Qi_1=0,Pi_1=0;
	 float  Q_Real_Position;
	 float  P_Real_Position;
	 int   X_Temp_0,X_Temp_1,X_Temp_2,Y_Temp_0,Y_Temp_1;
	 //uint32_t   Value;
   X_Temp_0  = XX[1]-XX[0];
	 X_Temp_1  = x-XX[0];

   Pi_0      = ((p[1]-p[0])*1.0/X_Temp_0)*X_Temp_1 + p[0];
   Qi_0      = ((q[1]-q[0])*1.0/X_Temp_0)*X_Temp_1 + q[0];
   X_Temp_2  = XX[3]-XX[2];
   Pi_1      = ((p[3]-p[2])*1.0/X_Temp_2)*X_Temp_1 + p[2];
   Qi_1      = ((q[3]-q[2])*1.0/X_Temp_2)*X_Temp_1 + q[2];
	 
	 Y_Temp_0  = y - YY[0];
	 Y_Temp_1  = YY[2] - YY[0];
	 P_Real_Position  = ((Pi_1-Pi_0)*1.0/Y_Temp_1)*Y_Temp_0 + Pi_0;
   Q_Real_Position  = ((Qi_1-Qi_0)*1.0/Y_Temp_1)*Y_Temp_0 + Qi_0;
		
	Plot_XrealReturn = P_Real_Position;
	Plot_YrealReturn = Q_Real_Position;
   //Value = ((uint16_t)P_Real_Position << 16)   +  (uint16_t)Q_Real_Position;
	//return Value;
}

/**********************************************************************************************************
* Function    ��void Deal_EnLaserData(void)
* Description : (Ŀ����Ļ����) ת���� (����Ƕ�->�������) ;
* Para        : none
* Return(s)   : none
* Example     : Deal_EnLaserData();
**********************************************************************************************************/
void Deal_EnLaserData(long nDataX, long nDataY)
{
		double Laser_cy1;
		double Laser_AngleXpi,Laser_AngleYpi;		// ����ʿ��ƽǶ�λ��;(360��=2��)
		float Laser_mul,Laser_mcode;
	long int Tmp_Xmotor,Tmp_Ymotor;			// �������A,B;
	//uint32_t Tmp_Data;
	unsigned char N1,N2,N3,N4;
	unsigned char i;
	
	
			if(Screen_XRealOffset != 0)	nDataX = Screen_XRealOffset - nDataX;
			if(Screen_YRealOffset != 0)	nDataY = Screen_YRealOffset - nDataY;

	//if((TC1_SNumX==0)&&(TC1_SNumY==0) )
	{
		if((Laser_ScrnSaveX!=nDataX) || (Laser_ScrnSaveY!=nDataY))
		{
			Laser_ScrnSaveX = nDataX;
			Laser_ScrnSaveY = nDataY;	
			
			if(C_shiftflag!=0)
			{
				if(PTZ_JiaoZhun_USE == USE_JiaoZhun_Motor)		// (USE_JiaoZhun_Motor)=1,���ò����������У׼��λ(16��);
				{
					for(i=0;i<16;i++)
					{
						Plot_Xreal[i]=Arg_ReadAdj.MotorReal[i]-300;
						Plot_Yreal[i]=Arg_ReadAdj.MotorReal[i+16]-300;
					}
				}
				else		// (USE_JiaoZhun_Code)=0,Ĭ�����ôű���У׼��λ(16��);
				{
					for(i=0;i<16;i++)
					{
						Plot_Xreal[i]=Arg_ReadAdj.PlotReal[i]-100;
						Plot_Yreal[i]=Arg_ReadAdj.PlotReal[i+16]-100;
					}
				}
				////////////////����,����;///////////////
				if((nDataX<=Plot_Xset[1])&&(nDataY<=Plot_Yset[7]))
				{
					N1=0;	N2=1;	N3=7;	N4=6;	
				}
				else if((nDataX<=Plot_Xset[1])&&(nDataY>=Plot_Yset[7])&&(nDataY<=Plot_Yset[8]))
				{
					N1=7;	N2=6;	N3=8;	N4=9;	
				}
				else if((nDataX<=Plot_Xset[1])&&(nDataY>=Plot_Yset[8]))
				{
					N1=8;	N2=9;	N3=15;	N4=14;	
				}
				////////////////����,����;///////////////
				else if((nDataX>=Plot_Xset[1])&&(nDataX<=Plot_Xset[2])&&(nDataY<=Plot_Yset[7]))
				{
					N1=1;	N2=2;	N3=6;	N4=5;	
				}
				else if((nDataX>=Plot_Xset[1])&&(nDataX<=Plot_Xset[2])&&(nDataY>=Plot_Yset[7])&&(nDataY<=Plot_Yset[8]))
				{
					N1=6;	N2=5;	N3=9;	N4=10;	
				}
				else if((nDataX>=Plot_Xset[1])&&(nDataX<=Plot_Xset[2])&&(nDataY>=Plot_Yset[8]))
				{
					N1=9;	N2=10;	N3=14;	N4=13;	
				}
				////////////////����,����;///////////////
				else if((nDataX>=Plot_Xset[2])&&(nDataY<=Plot_Yset[7]))
				{
					N1=2;	N2=3;	N3=5;	N4=4;	
				}
				else if((nDataX>=Plot_Xset[2])&&(nDataY>=Plot_Yset[7])&&(nDataY<=Plot_Yset[8]))
				{
					N1=5;	N2=4;	N3=10;	N4=11;	
				}
				else if((nDataX>=Plot_Xset[2])&&(nDataY>=Plot_Yset[8]))
				{
					N1=10;	N2=11;	N3=13;	N4=12;	
				}
				////////////////����,����У��ֵ;///////////////
					Plot_XsetA[0] = Plot_Xset[N1];			Plot_YsetA[0] = Plot_Yset[N1];
					Plot_XsetA[1] = Plot_Xset[N2];			Plot_YsetA[1] = Plot_Yset[N2];
					Plot_XsetA[2] = Plot_Xset[N3];			Plot_YsetA[2] = Plot_Yset[N3];
					Plot_XsetA[3] = Plot_Xset[N4];			Plot_YsetA[3] = Plot_Yset[N4];
					
					Plot_XrealA[0] = Plot_Xreal[N1];		Plot_YrealA[0] = Plot_Yreal[N1];
					Plot_XrealA[1] = Plot_Xreal[N2];		Plot_YrealA[1] = Plot_Yreal[N2];
					Plot_XrealA[2] = Plot_Xreal[N3];		Plot_YrealA[2] = Plot_Yreal[N3];
					Plot_XrealA[3] = Plot_Xreal[N4];		Plot_YrealA[3] = Plot_Yreal[N4];
				
				Position(nDataX,nDataY,Plot_XrealA,Plot_YrealA,Plot_XsetA,Plot_YsetA);
				
				nDataX = Plot_XrealReturn;
				nDataY = Plot_YrealReturn;
			}
			
			nDataX = nDataX-Arg_ReadAdj.PointaX;
			nDataY = nDataY-Arg_ReadAdj.PointaY;//+5
			
			// y=sin(x)����ʱ,xȡֵ��ͬ�ڽǶ�(��)��λ,��2�е���360��; 
			// ����,��Ҫ�Ƚ��Ƕ�(��)ת������ֵ����;	// ��ֵ = �Ƕ� * �� / 180; 
			// ��֮,��x=asin(y)����ʱ,x��ֵ��ת���ɽǶ�(��)ʹ��;	// �Ƕ� = ��ֵ * 180 / ��; 
			// ���ϸ��6400,��6400������ת��һȦ,��360��; // ת��N������ʱ,�Ƕ�=N*360/6400 = N*9/160;
				//(180/��)*(6400/360) = (180/��)*(160/9) = 1019.14;
				//(180/��)*((2500-500)/180) = (180/��)*(100/9) = 636.94 = 637;
				//(180/��)*(4096/360) = (180/��)*(512/45) = 652.23;
		
			Laser_mul=1019.14;
			Laser_mcode=652.23;
			
			if((nDataX==0)&&(nDataY==0))
			{
				Tmp_Xmotor = 0;
				Tmp_Ymotor = 0;
				CodeX.AddSet=0;
				CodeY.AddSet=0;
			}
			else if((nDataX!=0)&&(nDataY==0))
			{
				Laser_AngleXpi = atan(nDataX*1.0/(Arg_ReadAdj.LASER_Axis));//+CB_L1
				if(Laser_AngleXpi>0)
				Tmp_Xmotor = Laser_AngleXpi*Laser_mul + 0.5;
				else
				Tmp_Xmotor = Laser_AngleXpi*Laser_mul - 0.5;
				CodeX.AddSet=Laser_AngleXpi*Laser_mcode;
				
				Tmp_Ymotor = 0;
				CodeY.AddSet=0;
				//Laser_cy1 = sqrt( Arg_ReadAdj.LASER_Axis*Arg_ReadAdj.LASER_Axis + nDataX*nDataX);
				//Laser_AngleYpi = atan(nDataY/Laser_cy1);// atan(0)=0;
				//if(Laser_AngleYpi>0)
				//Tmp_Ymotor = Laser_AngleYpi*Laser_mul + 0.5;
				//else
				//Tmp_Ymotor = Laser_AngleYpi*Laser_mul - 0.5;
			}
			else if((nDataX==0)&&(nDataY!=0))
			{
				//Laser_AngleXpi = atan(nDataX*1.0/Arg_ReadAdj.LASER_Axis);// atan(0)=0;
				//if(Laser_AngleXpi>0)
				//Tmp_Xmotor = Laser_AngleXpi*Laser_mul + 0.5;
				//else
				//Tmp_Xmotor = Laser_AngleXpi*Laser_mul - 0.5;
				Tmp_Xmotor = 0;
				CodeX.AddSet=0;
				
				Laser_cy1 = sqrt( (Arg_ReadAdj.LASER_Axis)*(Arg_ReadAdj.LASER_Axis) + nDataX*nDataX);//+CB_L1+CB_L1
				Laser_AngleYpi = atan(nDataY/Laser_cy1);
				if(Laser_AngleYpi>0)
				Tmp_Ymotor = Laser_AngleYpi*Laser_mul + 0.5;
				else
				Tmp_Ymotor = Laser_AngleYpi*Laser_mul - 0.5;
				CodeY.AddSet=Laser_AngleYpi*Laser_mcode;
			}
			else
			{
				Laser_AngleXpi = atan(nDataX*1.0/(Arg_ReadAdj.LASER_Axis));//+CB_L1
				if(Laser_AngleXpi>0)
				Tmp_Xmotor = Laser_AngleXpi*Laser_mul + 0.5;
				else
				Tmp_Xmotor = Laser_AngleXpi*Laser_mul - 0.5;
				CodeX.AddSet=Laser_AngleXpi*Laser_mcode;
				
				Laser_cy1 = sqrt( (Arg_ReadAdj.LASER_Axis)*(Arg_ReadAdj.LASER_Axis) + nDataX*nDataX);//+CB_L1+CB_L1
				Laser_AngleYpi = atan(nDataY/Laser_cy1);
				if(Laser_AngleYpi>0)
				Tmp_Ymotor = Laser_AngleYpi*Laser_mul + 0.5;
				else
				Tmp_Ymotor = Laser_AngleYpi*Laser_mul - 0.5;
				CodeY.AddSet=Laser_AngleYpi*Laser_mcode;
			}
			
				Run_Xmotor = Tmp_Xmotor;
				Run_Ymotor = Tmp_Ymotor;
				TC1_SNumX=1;
				TC1_SNumY=1;
			CodeX.RealSet = Arg_ReadAdj.CBMzeroX+CodeX.AddSet;
			CodeY.RealSet = Arg_ReadAdj.CBMzeroY+CodeY.AddSet;
			CodeX.Compenflag=1;CodeY.Compenflag=1;
			
		}
	}
		
}

long int XReal_Code,YReal_Code;
long int XReal_Motor,YReal_Motor;

long int XRealAll=0;
long int YRealAll=0;// �ɵ�ǰ�Ƕȼ������ǰ��Ļ����λ��(������У׼��,���׼ȷ��ﵱǰʵ������ֵ);

long	Plot_XsetReturn =0;//
long	Plot_YsetReturn=0;// Y��У׼��;
////////////////////////////////////////////////////////////////////////////////////
/**********************************************************************************************************
* Function    ��Position( long    x, uint16_t    y, long *  p, long *  q,
	              long *  XX, long *  YY)
* Description : Ŀ����Ļ����ͨ����̨�����㷨����µ�Ŀ����Ļ����;
* Para        : none
* Return(s)   : none
* Example     : Position( long    x, uint16_t    y, long *  p, long *  q,
	              long *  XX, long *  YY)
**********************************************************************************************************/
void  Position_Tran( long    Xp, long    Yq, long *  p, long *  q,
	              long *  XX, long *  YY)
{
	long int PP_tmp1,PP_tmp2,PP_tmp3,PP_tmp4;
	long int QQ_tmp1,QQ_tmp2,QQ_tmp3,QQ_tmp4;
	float PX_tmp0,PX_tmp1;
	float QY_tmp0,QY_tmp1;
	float Ki1,Wi1,Wi2;
	
	PP_tmp1 = Xp - p[0];	//0
	PP_tmp2 = p[1] - p[0];	
	PP_tmp3 = p[2] - p[0];	//0
	PP_tmp4 = p[3] - p[2];
	PX_tmp1 = XX[1]-XX[0];
	QQ_tmp1 = Yq - q[0];	//0
	QQ_tmp2 = q[1] - q[0];	
	QQ_tmp3 = q[2] - q[0];	
	QQ_tmp4 = q[3] - q[2];	
	QY_tmp1 = YY[2]-YY[0];
	
	Ki1 = QQ_tmp2*(PP_tmp4-PP_tmp2) - PP_tmp2*(QQ_tmp4-QQ_tmp2);
	Wi1 = PP_tmp1*(QQ_tmp4-QQ_tmp2) - PP_tmp2*QQ_tmp3 - QQ_tmp1*(PP_tmp4-PP_tmp2) + QQ_tmp2*PP_tmp3;
	Wi2 = PP_tmp1*QQ_tmp3 - QQ_tmp1*PP_tmp3;
	if(Ki1>0)
	{
		if(PX_tmp1>0)
		{
			PX_tmp0 = (sqrt(Wi1*Wi1+4*Wi2*Ki1) + Wi1)*abs(PX_tmp1)/(2*abs(Ki1)) + 0.5;
		}
		else
		{
			PX_tmp0 = (sqrt(Wi1*Wi1+4*Wi2*Ki1) - Wi1)*abs(PX_tmp1)/(2*abs(Ki1)) + 0.5;
		}
	}
	else if(Ki1<0)
	{
		if(PX_tmp1>0)
		{
			PX_tmp0 = (sqrt(Wi1*Wi1-4*Wi2*Ki1) + Wi1)*abs(PX_tmp1)/(2*abs(Ki1)) + 0.5;
		}
		else
		{
			PX_tmp0 = (sqrt(Wi1*Wi1-4*Wi2*Ki1) - Wi1)*abs(PX_tmp1)/(2*abs(Ki1)) + 0.5;
		}
	}
	else	//PX_tmp0=0;
	{
		if((QQ_tmp2==0)&&(QQ_tmp4==QQ_tmp2))
		{
			PX_tmp0 = PX_tmp1*(PP_tmp1*QQ_tmp3-QQ_tmp1*PP_tmp3)/(PP_tmp2*(QQ_tmp3-QQ_tmp1)+QQ_tmp1*PP_tmp4);
		}
		else if((PP_tmp4==PP_tmp2)&&(QQ_tmp4==QQ_tmp2))
		{
			PX_tmp0 = PX_tmp1*(PP_tmp1*QQ_tmp3-QQ_tmp1*PP_tmp3)/(PP_tmp2*QQ_tmp3+QQ_tmp2*PP_tmp3);
		}
		else	PX_tmp0=0;
	}
	
	QY_tmp0 = QY_tmp1*(QQ_tmp1*PX_tmp1 - QQ_tmp2*PX_tmp0)/(PX_tmp1*QQ_tmp3 + (QQ_tmp4-QQ_tmp2)*PX_tmp0)+0.5;
	
	Plot_XsetReturn = PX_tmp0+XX[0];
	Plot_YsetReturn = QY_tmp0+YY[0];
	
}

/**********************************************************************************************************
* Function    ��void Deal_TranLaserData(void)
* Description : (�������->����Ƕ�) ת���� (��ǰ��Ļ����) ;
* Para        : none
* Return(s)   : none
* Example     : Deal_TranLaserData();
**********************************************************************************************************/
void Deal_TranLaserDataCode(void)// �������ǰʵ������(����λ�ñ�������λ);
{
		double Laser_Dy1;
		double Laser_bnx1,Laser_bny1;
		double Laser_AngleXpi,Laser_AngleYpi;		// ����ʿ��ƽǶ�λ��;(360��=2��)
		long mDataX, mDataY;
		float Laser_mcode;
		//int Laser_mul;
		long int Tmp_XData,Tmp_YData;
	unsigned char N1,N2,N3,N4;
	unsigned char i;
	
				mDataX = CodeX.AddRead;
				mDataY = CodeY.AddRead;
				//Laser_mul=160;
			Laser_mcode=102.4;

			// y=sin(x)����ʱ,xȡֵ��ͬ�ڽǶ�(��)��λ,��2�е���360��; 
			// ����,��Ҫ�Ƚ��Ƕ�(��)ת������ֵ����;	// ��ֵ = �Ƕ� * �� / 180; 
			// ��֮,��x=asin(y)����ʱ,x��ֵ��ת���ɽǶ�(��)ʹ��;	// �Ƕ� = ��ֵ * 180 / ��; 
			// ���ϸ��6400,��6400������ת��һȦ,��360��; // ת��N������ʱ,�Ƕ�=N*360/6400 = N*9/160;
			// �������Χ 500~2500usec,�Ƕ� 180 ��; // ת��N������ʱ,�Ƕ�=N*180/2000 = N*9/100;
			// �ű��뷶Χ 4096,�Ƕ� 360 ��; // ת��N������ʱ,�Ƕ�=N*360/4096 = N*9/102.4;
				if((mDataX==0)&&(mDataY==0))
				{
					Tmp_XData = 0;
					Tmp_YData = 0;
				}
				else if((mDataX!=0)&&(mDataY==0))
				{
					Laser_AngleXpi = (mDataX*9*1.0/Laser_mcode) *3.14/180;
					Laser_bnx1 = tan( Laser_AngleXpi );
					Tmp_XData = Arg_ReadAdj.LASER_Axis*Laser_bnx1;
					
					Tmp_YData = 0;
					//Laser_AngleYpi = (mDataY*9*1.0/Laser_mul) *3.14/180;
					//Laser_bny1 = tan( Laser_AngleYpi );//tan(0)=0;
					//Laser_Dy1 = sqrt( Arg_ReadAdj.LASER_Axis*Arg_ReadAdj.LASER_Axis + Tmp_XData*Tmp_XData);
					//Tmp_YData = Laser_Dy1*Laser_bny1;
				}
				else if((mDataX==0)&&(mDataY!=0))
				{
					Tmp_XData = 0;
					//Laser_AngleXpi = (mDataX*9*1.0/Laser_mul) *3.14/180;
					//Laser_bnx1 = tan( Laser_AngleXpi );//tan(0)=0;
					//Tmp_XData = Arg_ReadAdj.LASER_Axis*Laser_bnx1;
					
					Laser_AngleYpi = (mDataY*9*1.0/Laser_mcode) *3.14/180;
					Laser_bny1 = tan( Laser_AngleYpi );
					Laser_Dy1 = sqrt( Arg_ReadAdj.LASER_Axis*Arg_ReadAdj.LASER_Axis + Tmp_XData*Tmp_XData);
					Tmp_YData = Laser_Dy1*Laser_bny1;
				}
				else
				{
					Laser_AngleXpi = (mDataX*9*1.0/Laser_mcode) *3.14/180;
					Laser_bnx1 = tan( Laser_AngleXpi );
					Tmp_XData = Arg_ReadAdj.LASER_Axis*Laser_bnx1;
					
					Laser_AngleYpi = (mDataY*9*1.0/Laser_mcode) *3.14/180;
					Laser_bny1 = tan( Laser_AngleYpi );
					Laser_Dy1 = sqrt( Arg_ReadAdj.LASER_Axis*Arg_ReadAdj.LASER_Axis + Tmp_XData*Tmp_XData);
					Tmp_YData = Laser_Dy1*Laser_bny1;
				}
			
			if(Screen_XRealOffset != 0)				XReal_Code = Screen_XRealOffset - (Tmp_XData+Arg_ReadAdj.PointaX);
				else	XReal_Code = Tmp_XData+Arg_ReadAdj.PointaX;
			if(Screen_YRealOffset != 0)				YReal_Code = Screen_YRealOffset - (Tmp_YData+Arg_ReadAdj.PointaY);
				else	YReal_Code = Tmp_YData+Arg_ReadAdj.PointaY;
			
		if(PTZ_JiaoZhun_USE == USE_JiaoZhun_Code)
		{
			Tmp_XData = Tmp_XData+Arg_ReadAdj.PointaX;
			Tmp_YData = Tmp_YData+Arg_ReadAdj.PointaY;
			
			if(C_shiftflag!=0)
			{
				if(PTZ_JiaoZhun_USE == USE_JiaoZhun_Motor)		// (USE_JiaoZhun_Motor)=1,���ò����������У׼��λ(16��);
				{
					for(i=0;i<16;i++)
					{
						Plot_Xreal[i]=Arg_ReadAdj.MotorReal[i]-300;
						Plot_Yreal[i]=Arg_ReadAdj.MotorReal[i+16]-300;
					}
				}
				else		// (USE_JiaoZhun_Code)=0,Ĭ�����ôű���У׼��λ(16��);
				{
					for(i=0;i<16;i++)
					{
						Plot_Xreal[i]=Arg_ReadAdj.PlotReal[i]-100;
						Plot_Yreal[i]=Arg_ReadAdj.PlotReal[i+16]-100;
					}
				}
				////////////////����,����;///////////////
				if((Tmp_XData<=Plot_Xreal[1])&&(Tmp_YData<=Plot_Yreal[7]))
				{
					N1=0;	N2=1;	N3=7;	N4=6;	
				}
				else if((Tmp_XData<=Plot_Xreal[1])&&(Tmp_YData>=Plot_Yreal[7])&&(Tmp_YData<=Plot_Yreal[8]))
				{
					N1=7;	N2=6;	N3=8;	N4=9;	
				}
				else if((Tmp_XData<=Plot_Xreal[1])&&(Tmp_YData>=Plot_Yreal[8]))
				{
					N1=8;	N2=9;	N3=15;	N4=14;	
				}
				////////////////����,����;///////////////
				else if((Tmp_XData>=Plot_Xreal[1])&&(Tmp_XData<=Plot_Xreal[2])&&(Tmp_YData<=Plot_Yreal[7]))
				{
					N1=1;	N2=2;	N3=6;	N4=5;	
				}
				else if((Tmp_XData>=Plot_Xreal[1])&&(Tmp_XData<=Plot_Xreal[2])&&(Tmp_YData>=Plot_Yreal[7])&&(Tmp_YData<=Plot_Yreal[8]))
				{
					N1=6;	N2=5;	N3=9;	N4=10;	
				}
				else if((Tmp_XData>=Plot_Xreal[1])&&(Tmp_XData<=Plot_Xreal[2])&&(Tmp_YData>=Plot_Yreal[8]))
				{
					N1=9;	N2=10;	N3=14;	N4=13;	
				}
				////////////////����,����;///////////////
				else if((Tmp_XData>=Plot_Xreal[2])&&(Tmp_YData<=Plot_Yreal[7]))
				{
					N1=2;	N2=3;	N3=5;	N4=4;	
				}
				else if((Tmp_XData>=Plot_Xreal[2])&&(Tmp_YData>=Plot_Yreal[7])&&(Tmp_YData<=Plot_Yreal[8]))
				{
					N1=5;	N2=4;	N3=10;	N4=11;	
				}
				else if((Tmp_XData>=Plot_Xreal[2])&&(Tmp_YData>=Plot_Yreal[8]))
				{
					N1=10;	N2=11;	N3=13;	N4=12;	
				}
				
				////////////////����,����У��ֵ;///////////////
					Plot_XsetA[0] = Plot_Xset[N1];			Plot_YsetA[0] = Plot_Yset[N1];
					Plot_XsetA[1] = Plot_Xset[N2];			Plot_YsetA[1] = Plot_Yset[N2];
					Plot_XsetA[2] = Plot_Xset[N3];			Plot_YsetA[2] = Plot_Yset[N3];
					Plot_XsetA[3] = Plot_Xset[N4];			Plot_YsetA[3] = Plot_Yset[N4];
					
					Plot_XrealA[0] = Plot_Xreal[N1];		Plot_YrealA[0] = Plot_Yreal[N1];
					Plot_XrealA[1] = Plot_Xreal[N2];		Plot_YrealA[1] = Plot_Yreal[N2];
					Plot_XrealA[2] = Plot_Xreal[N3];		Plot_YrealA[2] = Plot_Yreal[N3];
					Plot_XrealA[3] = Plot_Xreal[N4];		Plot_YrealA[3] = Plot_Yreal[N4];
				
				Position_Tran(Tmp_XData,Tmp_YData,Plot_XrealA,Plot_YrealA,Plot_XsetA,Plot_YsetA);
				
				Tmp_XData = Plot_XsetReturn;
				Tmp_YData = Plot_YsetReturn;
			}
				
			if(Screen_XRealOffset != 0)				XRealAll = Screen_XRealOffset - Tmp_XData;
				else	XRealAll = Tmp_XData;
			if(Screen_YRealOffset != 0)				YRealAll = Screen_YRealOffset - Tmp_YData;
				else	YRealAll = Tmp_YData;
		}
		
}
/////////////////////////////////////////////////////////////////////////////////


/**********************************************************************************************************
* Function    ��void Deal_TranLaserData(void)
* Description : (�������->����Ƕ�) ת���� (��ǰ��Ļ����) ;
* Para        : none
* Return(s)   : none
* Example     : Deal_TranLaserData();
**********************************************************************************************************/
void Deal_TranLaserDataMotor(void)// �������ǰʵ������(����������ඨλ);
{
		double Laser_Dy1;
		double Laser_bnx1,Laser_bny1;
		double Laser_AngleXpi,Laser_AngleYpi;		// ����ʿ��ƽǶ�λ��;(360��=2��)
		long mDataX, mDataY;
		//float Laser_mcode;
		int Laser_mul;
		long int Tmp_XData,Tmp_YData;
	unsigned char N1,N2,N3,N4;
	unsigned char i;
	
				//mDataX = CodeX.AddRead;
				//mDataY = CodeY.AddRead;
			//Laser_mcode=102.4;
	
			mDataX = Run_Xnum;
			mDataY = Run_Ynum;
				Laser_mul=160;

			// y=sin(x)����ʱ,xȡֵ��ͬ�ڽǶ�(��)��λ,��2�е���360��; 
			// ����,��Ҫ�Ƚ��Ƕ�(��)ת������ֵ����;	// ��ֵ = �Ƕ� * �� / 180; 
			// ��֮,��x=asin(y)����ʱ,x��ֵ��ת���ɽǶ�(��)ʹ��;	// �Ƕ� = ��ֵ * 180 / ��; 
			// ���ϸ��6400,��6400������ת��һȦ,��360��; // ת��N������ʱ,�Ƕ�=N*360/6400 = N*9/160;
			// �������Χ 500~2500usec,�Ƕ� 180 ��; // ת��N������ʱ,�Ƕ�=N*180/2000 = N*9/100;
			// �ű��뷶Χ 4096,�Ƕ� 360 ��; // ת��N������ʱ,�Ƕ�=N*360/4096 = N*9/102.4;
				if((mDataX==0)&&(mDataY==0))
				{
					Tmp_XData = 0;
					Tmp_YData = 0;
				}
				else if((mDataX!=0)&&(mDataY==0))
				{
					Laser_AngleXpi = (mDataX*9*1.0/Laser_mul) *3.14/180;
					Laser_bnx1 = tan( Laser_AngleXpi );
					Tmp_XData = Arg_ReadAdj.LASER_Axis*Laser_bnx1;
					
					Tmp_YData = 0;
					//Laser_AngleYpi = (mDataY*9*1.0/Laser_mul) *3.14/180;
					//Laser_bny1 = tan( Laser_AngleYpi );//tan(0)=0;
					//Laser_Dy1 = sqrt( Arg_ReadAdj.LASER_Axis*Arg_ReadAdj.LASER_Axis + Tmp_XData*Tmp_XData);
					//Tmp_YData = Laser_Dy1*Laser_bny1;
				}
				else if((mDataX==0)&&(mDataY!=0))
				{
					Tmp_XData = 0;
					//Laser_AngleXpi = (mDataX*9*1.0/Laser_mul) *3.14/180;
					//Laser_bnx1 = tan( Laser_AngleXpi );//tan(0)=0;
					//Tmp_XData = Arg_ReadAdj.LASER_Axis*Laser_bnx1;
					
					Laser_AngleYpi = (mDataY*9*1.0/Laser_mul) *3.14/180;
					Laser_bny1 = tan( Laser_AngleYpi );
					Laser_Dy1 = sqrt( Arg_ReadAdj.LASER_Axis*Arg_ReadAdj.LASER_Axis + Tmp_XData*Tmp_XData);
					Tmp_YData = Laser_Dy1*Laser_bny1;
				}
				else
				{
					Laser_AngleXpi = (mDataX*9*1.0/Laser_mul) *3.14/180;
					Laser_bnx1 = tan( Laser_AngleXpi );
					Tmp_XData = Arg_ReadAdj.LASER_Axis*Laser_bnx1;
					
					Laser_AngleYpi = (mDataY*9*1.0/Laser_mul) *3.14/180;
					Laser_bny1 = tan( Laser_AngleYpi );
					Laser_Dy1 = sqrt( Arg_ReadAdj.LASER_Axis*Arg_ReadAdj.LASER_Axis + Tmp_XData*Tmp_XData);
					Tmp_YData = Laser_Dy1*Laser_bny1;
				}
			
			if(Screen_XRealOffset != 0)				XReal_Motor = Screen_XRealOffset - (Tmp_XData+Arg_ReadAdj.PointaX);// �������ǰʵ������(����������ඨλ);
				else	XReal_Motor = Tmp_XData+Arg_ReadAdj.PointaX;
			if(Screen_YRealOffset != 0)				YReal_Motor = Screen_YRealOffset - (Tmp_YData+Arg_ReadAdj.PointaY);
				else	YReal_Motor = Tmp_YData+Arg_ReadAdj.PointaY;
			
				
		if(PTZ_JiaoZhun_USE == USE_JiaoZhun_Motor)
		{
			Tmp_XData = Tmp_XData+Arg_ReadAdj.PointaX;
			Tmp_YData = Tmp_YData+Arg_ReadAdj.PointaY;
				
			if(C_shiftflag!=0)
			{
				if(PTZ_JiaoZhun_USE == USE_JiaoZhun_Motor)		// (USE_JiaoZhun_Motor)=1,���ò����������У׼��λ(16��);
				{
					for(i=0;i<16;i++)
					{
						Plot_Xreal[i]=Arg_ReadAdj.MotorReal[i]-300;
						Plot_Yreal[i]=Arg_ReadAdj.MotorReal[i+16]-300;
					}
				}
				else		// (USE_JiaoZhun_Code)=0,Ĭ�����ôű���У׼��λ(16��);
				{
					for(i=0;i<16;i++)
					{
						Plot_Xreal[i]=Arg_ReadAdj.PlotReal[i]-100;
						Plot_Yreal[i]=Arg_ReadAdj.PlotReal[i+16]-100;
					}
				}
				////////////////����,����;///////////////
				if((Tmp_XData<=Plot_Xreal[1])&&(Tmp_YData<=Plot_Yreal[7]))
				{
					N1=0;	N2=1;	N3=7;	N4=6;	
				}
				else if((Tmp_XData<=Plot_Xreal[1])&&(Tmp_YData>=Plot_Yreal[7])&&(Tmp_YData<=Plot_Yreal[8]))
				{
					N1=7;	N2=6;	N3=8;	N4=9;	
				}
				else if((Tmp_XData<=Plot_Xreal[1])&&(Tmp_YData>=Plot_Yreal[8]))
				{
					N1=8;	N2=9;	N3=15;	N4=14;	
				}
				////////////////����,����;///////////////
				else if((Tmp_XData>=Plot_Xreal[1])&&(Tmp_XData<=Plot_Xreal[2])&&(Tmp_YData<=Plot_Yreal[7]))
				{
					N1=1;	N2=2;	N3=6;	N4=5;	
				}
				else if((Tmp_XData>=Plot_Xreal[1])&&(Tmp_XData<=Plot_Xreal[2])&&(Tmp_YData>=Plot_Yreal[7])&&(Tmp_YData<=Plot_Yreal[8]))
				{
					N1=6;	N2=5;	N3=9;	N4=10;	
				}
				else if((Tmp_XData>=Plot_Xreal[1])&&(Tmp_XData<=Plot_Xreal[2])&&(Tmp_YData>=Plot_Yreal[8]))
				{
					N1=9;	N2=10;	N3=14;	N4=13;	
				}
				////////////////����,����;///////////////
				else if((Tmp_XData>=Plot_Xreal[2])&&(Tmp_YData<=Plot_Yreal[7]))
				{
					N1=2;	N2=3;	N3=5;	N4=4;	
				}
				else if((Tmp_XData>=Plot_Xreal[2])&&(Tmp_YData>=Plot_Yreal[7])&&(Tmp_YData<=Plot_Yreal[8]))
				{
					N1=5;	N2=4;	N3=10;	N4=11;	
				}
				else if((Tmp_XData>=Plot_Xreal[2])&&(Tmp_YData>=Plot_Yreal[8]))
				{
					N1=10;	N2=11;	N3=13;	N4=12;	
				}
				
				////////////////����,����У��ֵ;///////////////
					Plot_XsetA[0] = Plot_Xset[N1];			Plot_YsetA[0] = Plot_Yset[N1];
					Plot_XsetA[1] = Plot_Xset[N2];			Plot_YsetA[1] = Plot_Yset[N2];
					Plot_XsetA[2] = Plot_Xset[N3];			Plot_YsetA[2] = Plot_Yset[N3];
					Plot_XsetA[3] = Plot_Xset[N4];			Plot_YsetA[3] = Plot_Yset[N4];
					
					Plot_XrealA[0] = Plot_Xreal[N1];		Plot_YrealA[0] = Plot_Yreal[N1];
					Plot_XrealA[1] = Plot_Xreal[N2];		Plot_YrealA[1] = Plot_Yreal[N2];
					Plot_XrealA[2] = Plot_Xreal[N3];		Plot_YrealA[2] = Plot_Yreal[N3];
					Plot_XrealA[3] = Plot_Xreal[N4];		Plot_YrealA[3] = Plot_Yreal[N4];
				
				Position_Tran(Tmp_XData,Tmp_YData,Plot_XrealA,Plot_YrealA,Plot_XsetA,Plot_YsetA);
				
				Tmp_XData = Plot_XsetReturn;
				Tmp_YData = Plot_YsetReturn;
			}
				
			if(Screen_XRealOffset != 0)				XRealAll = Screen_XRealOffset - Tmp_XData;
				else	XRealAll = Tmp_XData;
			if(Screen_YRealOffset != 0)				YRealAll = Screen_YRealOffset - Tmp_YData;
				else	YRealAll = Tmp_YData;
		}
			
}
/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////


