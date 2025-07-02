#ifndef __AS5600__
#define __AS5600__
#include "stm32f10x.h"

#define Slave_Addr                0x36 //设备从地址
#define Write_Bit                 0	   //写标记	
#define Read_Bit                  1    //读标记
#define Angle_Hight_Register_Addr 0x0C //寄存器高位地址
#define Angle_Low_Register_Addr   0x0D //寄存器低位地址

void AS5600_Init ( void );  //初始化
u16 AS5600_Read_Len ( u8 addr, u8 reg, u8 len, u8 *buf );//从AS5600读取一次数据
void Get_Ini_Val(void); //得到上电后角度初始值
void Get_Num_sign(unsigned char Select); //获取当前角度值数据;


#endif
