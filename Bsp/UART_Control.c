/*
 * @file          UART_Control.c
 * @brief         程序: 语音控制、红外遥控等部分函数;
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

// 语音部分;02
unsigned char M_Mode10[10] = {"静止模式"};
unsigned char M_Mode11[10] = {"低速模式"};
unsigned char M_Mode12[10] = {"高速模式"};
unsigned char M_Color11[10] ={"蓝色"};
unsigned char M_Color12[10] ={"黄色"};
unsigned char M_Color13[10] ={"紫红"};
unsigned char M_Color14[10] ={"青色"};
unsigned char M_Shape11[12] ={"平行四边形  "};
unsigned char M_Shape12[12] ={"六边形      "};
unsigned char M_Shape13[12] ={"五边形      "};
unsigned char M_Shape14[12] ={"正方形      "};
unsigned char M_Shape15[12] ={"三角形      "};


///**************芯片设置命令*********************/
//u8 SYN_StopCom[5] = {0xFD, 0X00, 0X02, 0X02, 0XFD}; //停止合成
//u8 SYN_SuspendCom[5] = {0XFD, 0X00, 0X02, 0X03, 0XFC}; //暂停合成
//u8 SYN_RecoverCom[5] = {0XFD, 0X00, 0X02, 0X04, 0XFB}; //恢复合成
//u8 SYN_ChackCom[5] = {0XFD, 0X00, 0X02, 0X21, 0XDE}; //状态查询
//u8 SYN_PowerDownCom[5] = {0XFD, 0X00, 0X02, 0X88, 0X77}; //进入POWER DOWN 状态命令



// 输入字符串数组缓存地址及待转换数值,由数值转成字符串数组输出,并反馈当前字符串字长;
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


// 输入字符串数组缓存地址&当前数组长度、增加数组及数组长度, 得到A=A+B,返回最终字符串数组A字长;
unsigned int String_add(unsigned char *Buffa,unsigned char *Buffb, unsigned int ADat_len,unsigned int BDat_len)
{
		int i;
		for(i=0; i<BDat_len; i++)
		{
			Buffa[i+ADat_len] = Buffb[i];		//语音内容增加;
		}
		ADat_len = ADat_len+i;						//增加内容后,当前固定字符串长度;1
		
		return ADat_len;
}

/**********************************************************************************************************
* Function    ：void Sound_Start(void)
* Description : 开机语音;
* Para        : none
* Return(s)   : none
* Example     : Sound_Start();
**********************************************************************************************************/
void Sound_Start(void)
{
		SYN_FrameInfo(2,"[v1][m1][t5]工业传送带已就绪");
}


/**********************************************************************************************************
* Function    ：void Sound_HandSet(void)
* Description : 手动指向时语音;
* Para        : none
* Return(s)   : none
* Example     : Sound_HandSet();
**********************************************************************************************************/
void Sound_HandSet(void)
{
	unsigned char Str_length=0;
	unsigned char Str_Cnt=0;
	unsigned char Sound_Massage[40]={"[v1][m0][t5]当前位置"};
	unsigned char Buf_Massage[30]={"                    "};
	
		///////////////////////////////////////////////////////////////
		//选择背景音乐2。(0：无背景音乐  1-15：背景音乐可选)
		//m[0~16]:0背景音乐为静音，16背景音乐音量最大
		//v[0~16]:0朗读音量为静音，16朗读音量最大
		//t[0~5]:0朗读语速最慢，5朗读语速最快
		//其他不常用功能请参考数据手册
		
		Str_length = 20;						//当前固定字符串长度;后续语音内容后面接上;
		
		Str_Cnt = Num_TO_String(Buf_Massage,Laser_ScreenX);	//X坐标转字符串;
		Str_length = String_add(Sound_Massage, Buf_Massage, Str_length, Str_Cnt);// A+B 字符串;返回最终A长度;
		
		Sound_Massage[Str_length] = ',';
		Str_length = Str_length+1;		//增加内容后,当前固定字符串长度;2
	
		Str_Cnt = Num_TO_String(Buf_Massage,Laser_ScreenY);	//Y坐标转字符串;
		Str_length = String_add(Sound_Massage, Buf_Massage, Str_length, Str_Cnt);// A+B 字符串;返回最终A长度;

		SYN_FrameInfo(2,Sound_Massage);
}



/**********************************************************************************************************
* Function    ：void Sound_POST(void)
* Description : 发现目标后语音播报;
* Para        : none
* Return(s)   : none
* Example     : Sound_POST();
**********************************************************************************************************/
void Sound_POST(void)
{
	unsigned char Str_length=0;
	unsigned char Str_Cnt=0;
	//unsigned char Yuyin_Massage[50]={"[v1][m0][t5]高速模式检测到红色国王"};
	unsigned char Sound_Massage[50]={"[v1][m0][t5]"};
	unsigned char Speech_Sound2[6]={"检测到"};

		///////////////////////////////////////////////////////////////
		//选择背景音乐2。(0：无背景音乐  1-15：背景音乐可选)
		//m[0~16]:0背景音乐为静音，16背景音乐音量最大
		//v[0~16]:0朗读音量为静音，16朗读音量最大
		//t[0~5]:0朗读语速最慢，5朗读语速最快
		//其他不常用功能请参考数据手册
	
		Str_length = 12;						//当前固定字符串长度;后续语音内容后面接上;
	
		Str_Cnt = 8;	// 工作模式;
		switch(Video_Mode)
		{
			case 0: Str_length = String_add(Sound_Massage, M_Mode10, Str_length, Str_Cnt);// A+B 字符串;返回最终A长度;
							break;
			case 1:	Str_length = String_add(Sound_Massage, M_Mode11, Str_length, Str_Cnt);// A+B 字符串;返回最终A长度;
							break;
			case 2:	Str_length = String_add(Sound_Massage, M_Mode12, Str_length, Str_Cnt);// A+B 字符串;返回最终A长度;
							break;
			default:Str_length = String_add(Sound_Massage, M_Mode10, Str_length, Str_Cnt);// A+B 字符串;返回最终A长度;
							break;
		}
		
		Str_Cnt = 6;	// “检测到”;
		Str_length = String_add(Sound_Massage, Speech_Sound2, Str_length, Str_Cnt);// A+B 字符串;返回最终A长度;
		
		Str_Cnt = 4;	// 颜色;
		switch(Material_Color)
		{
			case 1:	Str_length = String_add(Sound_Massage, M_Color11, Str_length, Str_Cnt);// A+B 字符串;返回最终A长度;
							break;
			case 2:	Str_length = String_add(Sound_Massage, M_Color12, Str_length, Str_Cnt);// A+B 字符串;返回最终A长度;
							break;
			case 3:	Str_length = String_add(Sound_Massage, M_Color13, Str_length, Str_Cnt);// A+B 字符串;返回最终A长度;
							break;
			case 4:	Str_length = String_add(Sound_Massage, M_Color14, Str_length, Str_Cnt);// A+B 字符串;返回最终A长度;
							break;
			default:Str_length = String_add(Sound_Massage, M_Color11, Str_length, Str_Cnt);// A+B 字符串;返回最终A长度;	
							break;
		}
		
		Str_Cnt = 12;	// 形状;
		switch(Material_Shape)
		{
			case 1:	Str_length = String_add(Sound_Massage, M_Shape11, Str_length, Str_Cnt);// A+B 字符串;返回最终A长度;
							break;
			case 2:	Str_length = String_add(Sound_Massage, M_Shape12, Str_length, Str_Cnt);// A+B 字符串;返回最终A长度;
							break;
			case 3:	Str_length = String_add(Sound_Massage, M_Shape13, Str_length, Str_Cnt);// A+B 字符串;返回最终A长度;		
							break;
			case 4:	Str_length = String_add(Sound_Massage, M_Shape14, Str_length, Str_Cnt);// A+B 字符串;返回最终A长度;	
							break;
			case 5:	Str_length = String_add(Sound_Massage, M_Shape15, Str_length, Str_Cnt);// A+B 字符串;返回最终A长度;	
							break;
			default:Str_length = String_add(Sound_Massage, M_Shape11, Str_length, Str_Cnt);// A+B 字符串;返回最终A长度;	
							break;
		}
		
		SYN_FrameInfo(2,Sound_Massage);
}



/**********************************************************************************************************
* Function    ：void Video_Control(void)
* Description : 视频播放控制;
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
			USART_Send_Data02(4,&Video_TxSend[0],3);// 发送控制红外遥控器指令;Pause/0x01;Play/0x02;
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
				USART_Send_Data02(4,&Video_TxSend[0],3);// 发送控制红外遥控器指令;Pause/0x01;Play/0x02;
		}

	}
}


