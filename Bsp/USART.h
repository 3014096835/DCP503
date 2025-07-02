#ifndef __USART_H__
#define __USART_H__

#include  "stm32f10x.h"


// UART1~UART5 IO    
#define BSP_IO_TXD1     GPIO_Pin_9	 // PA9(IO101)
#define BSP_IO_RXD1     GPIO_Pin_10	 // PA10(IO102)
#define BSP_IO_DIR1     GPIO_Pin_8   // PA8(IO100)

#define BSP_IO_TXD2     GPIO_Pin_2	 // PA2(IO36)
#define BSP_IO_RXD2     GPIO_Pin_3	 // PA3(IO37)
#define BSP_IO_TXD3     GPIO_Pin_10	 // PB10(IO69)
#define BSP_IO_RXD3     GPIO_Pin_11	 // PB11(IO70)

#define BSP_IO_TXD4     GPIO_Pin_10	 // PC10(IO111)
#define BSP_IO_RXD4     GPIO_Pin_11	 // PC11(IO112)	
#define BSP_IO_TXD5     GPIO_Pin_12	 // PC12(IO113)
#define BSP_IO_RXD5     GPIO_Pin_2	 // PD2(IO116)


#define USART1_DR_Base  0x40013804
#define USART2_DR_Base  0x40004404
#define USART3_DR_Base  0x40004804
#define UART4_DR_Base   0x40004C04

#define RS_485_TX()		 GPIO_SetBits(GPIOA, BSP_IO_DIR1);		// 设置RS485-1发送状态;
#define RS_485_RX()		 GPIO_ResetBits(GPIOA, BSP_IO_DIR1);		// 设置RS485-1接收状态;

/* Function  -----------------------------------------------------------*/
// USRT.C
extern void UART_Init(void);
extern void UART_GPIO_Configuration(void);
extern void UART_NVIC_Configuration(void);
extern void USART_Configuration02(USART_TypeDef* USARTx, uint32_t bound);
extern void BSP_DMA_Configuration(uint8_t UsartNum,uint8_t *TxbufferAddr,uint16_t Txnum);
extern void BSP_USART_OUT02(USART_TypeDef* USARTx, uint8_t *Data,uint16_t Len);
extern void USART_Send_Data02(uint8_t Channel,uint8_t *Dat,uint8_t Counter);
extern uint8_t REC_CRC_CHECK(unsigned char *RxData,uint16_t num);

// USRT_Control.C
extern unsigned int Num_TO_String(unsigned char *Buff, unsigned int Datnum);
extern unsigned int String_add(unsigned char *Buffa,unsigned char *Buffb, unsigned int ADat_len,unsigned int BDat_len);
extern void Sound_Start(void);
extern void Sound_HandSet(void);
extern void Sound_POST(void);
extern void Video_Control(unsigned char M_State);

// USRT_Camera.C
extern void UART4_IRQHandler(void);
extern void UART4Rx_DMA_Init(void);
extern void UART4Rx_DMA_Enable(void);
extern void UART4Rx_DMA_Disable(void);
extern void Camera_RxRefresh(void);
extern void Camera_TxRefresh(unsigned int T_Beat);
void Camera_AIselect(uint16_t pic_num, uint16_t pic_list1, uint16_t pic_list2, uint16_t  pic_list3);

// USRT_485.C
extern void Massage_Post(void);
extern void Massage_PostEnd(void);

// MCGS触摸屏发送函数新修改;//20241020修改;
//extern void MCGS_Send(uint8_t M_Cnt,uint8_t M_State,uint8_t M_Mode,long M_Time,uint8_t M_Num,uint8_t M_Message);
void MCGS_Send(uint8_t M_Cnt,uint8_t M_State,uint8_t M_Mode,long M_Time,uint8_t M_Num,uint8_t M_Message
								,uint8_t M_Shape,uint8_t M_Color);

extern void Function_Modbus(void);

// USRT_TFTLCD.C
extern void LCD_DealData(void);

#endif 
                               
