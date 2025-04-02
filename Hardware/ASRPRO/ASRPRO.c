#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "OLED.h"
#include "LED.h"

uint8_t ASRPRO_RxFlag=0;  //中断接收标志位

void ASRPRO_Init(void)        //串口2初始化
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;            //USART2_TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;            //USART2_RX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStructure);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//xx
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART2, ENABLE);
}

void USART2_IRQHandler(void)    //利用中断接收语音助手的数据改变标志位
{

	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)    //判断是否是USART1的接收事件触发的中断
	{
		uint8_t RxData = USART_ReceiveData(USART2);           //读取数据寄存器，存放在接收的数据变量
		
			if (RxData==0x01)      //0x01 由语音模块发来
			{
				ASRPRO_RxFlag = 1;
			}
			else if (RxData==0x02)  //
			{
				ASRPRO_RxFlag = 2;
			}
			else if (RxData==0x03)  //
			{
				ASRPRO_RxFlag = 3;
			}
			else if (RxData==0x04)  //
			{
				ASRPRO_RxFlag = 4;
			}
			else if (RxData==0x05)  //
			{
				ASRPRO_RxFlag = 5;
			}
			else if (RxData==0x06)  //
			{
				ASRPRO_RxFlag = 6;
			}
			else if (RxData==0x07)  //
			{
				ASRPRO_RxFlag = 7;
			}
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);     
	}
}

extern u8 mode;

u8 RxFlag_deal(void)
{
	 switch(ASRPRO_RxFlag)
			{
				case 1:
							mode=1;
							ASRPRO_RxFlag=0;
				      break;
				
				case 2:
							mode=2;
							ASRPRO_RxFlag=0;
				      break;
				
				case 3:
							mode=3;
							ASRPRO_RxFlag=0;
				      break;
				
				case 4:
							mode=4;
							ASRPRO_RxFlag=0;
				      break;
				
				case 5:
							mode=5;
							ASRPRO_RxFlag=0;
				      break;
				
				case 6:
							mode=6;
				      OLED_Clear();
							ASRPRO_RxFlag=0;
				      break;
				
				case 7:                            //关灯
							mode=0;
				      OLED_Clear();
							ASRPRO_RxFlag=0;
				      break;
				
				default:break;
			}
    return mode;
}

