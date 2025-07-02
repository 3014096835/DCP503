#ifndef __I2C_H
#define __I2C_H
#include "stm32f10x.h"

// #define IIC_SCL   PAout(11)
// #define IIC_SDA   PAout(12)
// #define READ_SDA  PAin(12)

#define GPIO_SCL01 			GPIO_Pin_6			//SCL01 = PG3 => PA6;
#define GPIO_SDA01 			GPIO_Pin_7			//SDA01 = PG4 => PA7;
#define IIC_SCL01_L            GPIO_ResetBits(GPIOA, GPIO_SCL01)                  //SCL01 ;
#define IIC_SCL01_H            GPIO_SetBits(GPIOA, GPIO_SCL01)
#define IIC_SDA01_L            GPIO_ResetBits(GPIOA, GPIO_SDA01)                   //SDA01 ;
#define IIC_SDA01_H            GPIO_SetBits(GPIOA, GPIO_SDA01) 
#define READ_SDA01           GPIO_ReadInputDataBit(GPIOA, GPIO_SDA01 ) 

#define GPIO_SCL02 			GPIO_Pin_0			//SCL02 = PG5 => PA0;
#define GPIO_SDA02 			GPIO_Pin_1			//SDA02 = PG6 => PA1;
#define IIC_SCL02_L            GPIO_ResetBits(GPIOA, GPIO_SCL02)                  //SCL02 ;
#define IIC_SCL02_H            GPIO_SetBits(GPIOA, GPIO_SCL02)
#define IIC_SDA02_L            GPIO_ResetBits(GPIOA, GPIO_SDA02)                   //SDA02 ;
#define IIC_SDA02_H            GPIO_SetBits(GPIOA, GPIO_SDA02) 
#define READ_SDA02           GPIO_ReadInputDataBit(GPIOA, GPIO_SDA02 ) 

extern unsigned char IIC_Select;


void IIC_Init ( void );
void SDA_IN ( void );
void SDA_OUT ( void );
void IIC_Start ( void );
void IIC_Stop ( void );
u8 IIC_Wait_Ack ( void );
void IIC_Ack ( void );
void IIC_NAck ( void );
void IIC_Send_Byte ( u8 txd );
u8 IIC_Read_Byte ( u8 ack );
void Delay_us(unsigned int Count);

#endif
