#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

#define USART_REC_LEN  			3  	//定义最大接收字节数为 3
extern u8  USART_RX_BUF[USART_REC_LEN]; 
extern u8 USART_RX_STA;

extern uint8_t Serial_TxPacket[];
extern uint8_t Serial_RxPacket[];
extern uint8_t Serial_RxFlag;

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);

void Serial_SendPacket(void);
uint8_t Serial_GetRxData(void);
uint8_t Serial_GetRxFlag(void);

#endif
