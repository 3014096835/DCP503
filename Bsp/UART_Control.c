/*
 * @file          UART_Control.c
 * @brief         ����: �������ơ�����ң�صȲ��ֺ���;
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

// ��������;02
unsigned char M_Mode10[10] = {"��ֹģʽ"};
unsigned char M_Mode11[10] = {"����ģʽ"};
unsigned char M_Mode12[10] = {"����ģʽ"};
unsigned char M_Color11[10] ={"��ɫ"};
unsigned char M_Color12[10] ={"��ɫ"};
unsigned char M_Color13[10] ={"�Ϻ�"};
unsigned char M_Color14[10] ={"��ɫ"};
unsigned char M_Shape11[12] ={"ƽ���ı���  "};
unsigned char M_Shape12[12] ={"������      "};
unsigned char M_Shape13[12] ={"�����      "};
unsigned char M_Shape14[12] ={"������      "};
unsigned char M_Shape15[12] ={"������      "};


///**************оƬ��������*********************/
//u8 SYN_StopCom[5] = {0xFD, 0X00, 0X02, 0X02, 0XFD}; //ֹͣ�ϳ�
//u8 SYN_SuspendCom[5] = {0XFD, 0X00, 0X02, 0X03, 0XFC}; //��ͣ�ϳ�
//u8 SYN_RecoverCom[5] = {0XFD, 0X00, 0X02, 0X04, 0XFB}; //�ָ��ϳ�
//u8 SYN_ChackCom[5] = {0XFD, 0X00, 0X02, 0X21, 0XDE}; //״̬��ѯ
//u8 SYN_PowerDownCom[5] = {0XFD, 0X00, 0X02, 0X88, 0X77}; //����POWER DOWN ״̬����



// �����ַ������黺���ַ����ת����ֵ,����ֵת���ַ����������,��������ǰ�ַ����ֳ�;
unsigned int Num_TO_String(unsigned char *Buff, unsigned int Datnum)
{
		unsigned int t;
	
		if(Datnum>999)
		{
			t=4;
			Buff[0] = 0x30+Datnum/1000%10;
			Buff[1] = 0x30+Datnum/100%10;
			Buff[2] = 0x30+Datnum/10%10;
			Buff[3] = 0x30+Datnum/1%10;
		}
		else if(Datnum>99)
		{
			t=3;
			Buff[0] = 0x30+Datnum/100%10;
			Buff[1] = 0x30+Datnum/10%10;
			Buff[2] = 0x30+Datnum/1%10;
		}
		else if(Datnum>9)
		{
			t=2;
			Buff[0] = 0x30+Datnum/10%10;
			Buff[1] = 0x30+Datnum/1%10;
		}
		else
		{
			t=1;
			Buff[0] = 0x30+Datnum/1%10;
		}
		
		return t;
}


// �����ַ������黺���ַ&��ǰ���鳤�ȡ��������鼰���鳤��, �õ�A=A+B,���������ַ�������A�ֳ�;
unsigned int String_add(unsigned char *Buffa,unsigned char *Buffb, unsigned int ADat_len,unsigned int BDat_len)
{
		int i;
		for(i=0; i<BDat_len; i++)
		{
			Buffa[i+ADat_len] = Buffb[i];		//������������;
		}
		ADat_len = ADat_len+i;						//�������ݺ�,��ǰ�̶��ַ�������;1
		
		return ADat_len;
}

/**********************************************************************************************************
* Function    ��void Sound_Start(void)
* Description : ��������;
* Para        : none
* Return(s)   : none
* Example     : Sound_Start();
**********************************************************************************************************/
void Sound_Start(void)
{
		SYN_FrameInfo(2,"[v1][m1][t5]��ҵ���ʹ��Ѿ���");
}


/**********************************************************************************************************
* Function    ��void Sound_HandSet(void)
* Description : �ֶ�ָ��ʱ����;
* Para        : none
* Return(s)   : none
* Example     : Sound_HandSet();
**********************************************************************************************************/
void Sound_HandSet(void)
{
	unsigned char Str_length=0;
	unsigned char Str_Cnt=0;
	unsigned char Sound_Massage[40]={"[v1][m0][t5]��ǰλ��"};
	unsigned char Buf_Massage[30]={"                    "};
	
		///////////////////////////////////////////////////////////////
		//ѡ�񱳾�����2��(0���ޱ�������  1-15���������ֿ�ѡ)
		//m[0~16]:0��������Ϊ������16���������������
		//v[0~16]:0�ʶ�����Ϊ������16�ʶ��������
		//t[0~5]:0�ʶ�����������5�ʶ��������
		//���������ù�����ο������ֲ�
		
		Str_length = 20;						//��ǰ�̶��ַ�������;�����������ݺ������;
		
		Str_Cnt = Num_TO_String(Buf_Massage,Laser_ScreenX);	//X����ת�ַ���;
		Str_length = String_add(Sound_Massage, Buf_Massage, Str_length, Str_Cnt);// A+B �ַ���;��������A����;
		
		Sound_Massage[Str_length] = ',';
		Str_length = Str_length+1;		//�������ݺ�,��ǰ�̶��ַ�������;2
	
		Str_Cnt = Num_TO_String(Buf_Massage,Laser_ScreenY);	//Y����ת�ַ���;
		Str_length = String_add(Sound_Massage, Buf_Massage, Str_length, Str_Cnt);// A+B �ַ���;��������A����;

		SYN_FrameInfo(2,Sound_Massage);
}



/**********************************************************************************************************
* Function    ��void Sound_POST(void)
* Description : ����Ŀ�����������;
* Para        : none
* Return(s)   : none
* Example     : Sound_POST();
**********************************************************************************************************/
void Sound_POST(void)
{
	unsigned char Str_length=0;
	unsigned char Str_Cnt=0;
	//unsigned char Yuyin_Massage[50]={"[v1][m0][t5]����ģʽ��⵽��ɫ����"};
	unsigned char Sound_Massage[50]={"[v1][m0][t5]"};
	unsigned char Speech_Sound2[6]={"��⵽"};

		///////////////////////////////////////////////////////////////
		//ѡ�񱳾�����2��(0���ޱ�������  1-15���������ֿ�ѡ)
		//m[0~16]:0��������Ϊ������16���������������
		//v[0~16]:0�ʶ�����Ϊ������16�ʶ��������
		//t[0~5]:0�ʶ�����������5�ʶ��������
		//���������ù�����ο������ֲ�
	
		Str_length = 12;						//��ǰ�̶��ַ�������;�����������ݺ������;
	
		Str_Cnt = 8;	// ����ģʽ;
		switch(Video_Mode)
		{
			case 0: Str_length = String_add(Sound_Massage, M_Mode10, Str_length, Str_Cnt);// A+B �ַ���;��������A����;
							break;
			case 1:	Str_length = String_add(Sound_Massage, M_Mode11, Str_length, Str_Cnt);// A+B �ַ���;��������A����;
							break;
			case 2:	Str_length = String_add(Sound_Massage, M_Mode12, Str_length, Str_Cnt);// A+B �ַ���;��������A����;
							break;
			default:Str_length = String_add(Sound_Massage, M_Mode10, Str_length, Str_Cnt);// A+B �ַ���;��������A����;
							break;
		}
		
		Str_Cnt = 6;	// ����⵽��;
		Str_length = String_add(Sound_Massage, Speech_Sound2, Str_length, Str_Cnt);// A+B �ַ���;��������A����;
		
		Str_Cnt = 4;	// ��ɫ;
		switch(Material_Color)
		{
			case 1:	Str_length = String_add(Sound_Massage, M_Color11, Str_length, Str_Cnt);// A+B �ַ���;��������A����;
							break;
			case 2:	Str_length = String_add(Sound_Massage, M_Color12, Str_length, Str_Cnt);// A+B �ַ���;��������A����;
							break;
			case 3:	Str_length = String_add(Sound_Massage, M_Color13, Str_length, Str_Cnt);// A+B �ַ���;��������A����;
							break;
			case 4:	Str_length = String_add(Sound_Massage, M_Color14, Str_length, Str_Cnt);// A+B �ַ���;��������A����;
							break;
			default:Str_length = String_add(Sound_Massage, M_Color11, Str_length, Str_Cnt);// A+B �ַ���;��������A����;	
							break;
		}
		
		Str_Cnt = 12;	// ��״;
		switch(Material_Shape)
		{
			case 1:	Str_length = String_add(Sound_Massage, M_Shape11, Str_length, Str_Cnt);// A+B �ַ���;��������A����;
							break;
			case 2:	Str_length = String_add(Sound_Massage, M_Shape12, Str_length, Str_Cnt);// A+B �ַ���;��������A����;
							break;
			case 3:	Str_length = String_add(Sound_Massage, M_Shape13, Str_length, Str_Cnt);// A+B �ַ���;��������A����;		
							break;
			case 4:	Str_length = String_add(Sound_Massage, M_Shape14, Str_length, Str_Cnt);// A+B �ַ���;��������A����;	
							break;
			case 5:	Str_length = String_add(Sound_Massage, M_Shape15, Str_length, Str_Cnt);// A+B �ַ���;��������A����;	
							break;
			default:Str_length = String_add(Sound_Massage, M_Shape11, Str_length, Str_Cnt);// A+B �ַ���;��������A����;	
							break;
		}
		
		SYN_FrameInfo(2,Sound_Massage);
}



/**********************************************************************************************************
* Function    ��void Video_Control(void)
* Description : ��Ƶ���ſ���;
* Para        : none
* Return(s)   : none
* Example     : Video_Control();
**********************************************************************************************************/
void Video_Control(unsigned char M_State)
{
	unsigned int i;
	if(Video_HandControl==1)
	{
		Video_HandControl=0;
			for(i=0;i<3;i++)		{	Video_TxSend[i] = M_State;	}
			USART_Send_Data02(4,&Video_TxSend[0],3);// ���Ϳ��ƺ���ң����ָ��;Pause/0x01;Play/0x02;
	}
	else
	{
		if(M_State == Video_Pause)// 
		{
			Video_timeflag=1;
			Video_timesec=0;
			Video_timemisec=0;
		}
		else
		{
			Video_timeflag=0;
			Video_timesec=0;
			Video_timemisec=0;
		}
		
		if(Video_State != M_State)
		{
				Video_State = M_State;
			
				for(i=0;i<3;i++)		{	Video_TxSend[i] = M_State;	}
				USART_Send_Data02(4,&Video_TxSend[0],3);// ���Ϳ��ƺ���ң����ָ��;Pause/0x01;Play/0x02;
		}

	}
}


