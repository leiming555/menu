#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "stepmotor.h"

void LED_Init(void)
{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;   //ºì»ÆÀ¶ÂÌ
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB,&GPIO_InitStructure);

	 
}
extern u8 mode;
void Red_off(void)
{
	 mode=0;
		GPIO_ResetBits(GPIOB,GPIO_Pin_11);
}

extern u8 mode;
void Red_on(void)
{
	
	  mode=6;
		GPIO_SetBits(GPIOB,GPIO_Pin_11);
}


extern u8 mode;

void mode_deal(void)
{
	switch(mode)
	{
		case 0:
					 Red_off();     //¹ØµÆ
					 break;
		
		case 1:
					     MOTOR_Direction_Angle(1,0,90,1); //´ò¿ª´°Á±
					 break;
		
		case 2:
		          MOTOR_Direction_Angle(0,0,90,1);//¹Ø´°Á±
					 break;
		
		case 3:
					      //À¶µÆ
					 break;
		
		case 4:
					     //ÂÌµÆ
					 break;
		
		case 5:
					     //°×µÆ
					 break;
		
		case 6:
					Red_on();    //¿ªµÆ
					 break;
		
		case 7:
			   MOTOR_STOP();
		       break;
		
		
		
		default:break;
	}
}



