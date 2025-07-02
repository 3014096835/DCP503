#ifndef __AS5600__
#define __AS5600__
#include "stm32f10x.h"

#define Slave_Addr                0x36 //�豸�ӵ�ַ
#define Write_Bit                 0	   //д���	
#define Read_Bit                  1    //�����
#define Angle_Hight_Register_Addr 0x0C //�Ĵ�����λ��ַ
#define Angle_Low_Register_Addr   0x0D //�Ĵ�����λ��ַ

void AS5600_Init ( void );  //��ʼ��
u16 AS5600_Read_Len ( u8 addr, u8 reg, u8 len, u8 *buf );//��AS5600��ȡһ������
void Get_Ini_Val(void); //�õ��ϵ��Ƕȳ�ʼֵ
void Get_Num_sign(unsigned char Select); //��ȡ��ǰ�Ƕ�ֵ����;


#endif
