#include "Menu.h"
#include "LED.h"
#include "stepmotor.h"

void Menu_RunMainMenu(void)
{
	static struct Option_Class Menu_StartOptionList[] = {
		{"<<<"},
		{"Tools", Menu_RunToolsMenu},	   // 工具
		{"Curtain", Menu_RunCurtainMenu},
		{"LED", Menu_RunLEDMenu},  
		{"Setting", Menu_RunSettingMenu},  // 设置
		{"Information", Menu_Information}, // 信息
		{".."}};

	Menu_RunMenu(Menu_StartOptionList);
}
void Menu_RunLEDMenu(void)
{
	LED_Init();
	static struct Option_Class Menu_ToolsOptionList[] = {
		{"<<<"},
		{"ON",Red_on},	
    {"OFF",Red_off},		
		{".."}};

	Menu_RunMenu(Menu_ToolsOptionList);
}

void Menu_RunCurtainMenu(void)
{
	static struct Option_Class Menu_ToolsOptionList[] = {
		{"<<<"},
		{"ON",Curtain_on},	
    {"OFF",Curtain_off},		
		{".."}};

	Menu_RunMenu(Menu_ToolsOptionList);
}

extern u8 mode;
void Curtain_on(void)
{
  MOTOR_Direction_Angle(1,0,90,1); //打开窗帘
	mode=1;
	MOTOR_STOP();
}
void Curtain_off(void)
{
  MOTOR_Direction_Angle(0,0,90,1); //关闭窗帘
	mode=2;
	MOTOR_STOP();
}



void Menu_RunToolsMenu(void)
{
	static struct Option_Class Menu_ToolsOptionList[] = {
		{"<<<"},
		{"定时器"},					   // 6-1 定时器定时中断
		{"输入捕获"},				   // 6-6 输入捕获模式测频率
		{"PWM输出", Tools_PWM_Output}, // 6-3 PWM驱动LED呼吸灯
		{"PWM输入"},				   // 6-7 PWMI模式测频率占空比
		{"编码器"},					   // 6-8 编码器接口测速
		{"串口"},					   // 9-3 串口收发HEX数据包
		{"示波器"},					   // 示波器
		{"ADC"},					   // 8-2 DMA+AD多通道
		{".."}};

	Menu_RunMenu(Menu_ToolsOptionList);
}


void Menu_Information(void)
{
	uint8_t Angle = 233;
	while (1)
	{
		OLED_Clear();

		OLED_ShowImage(88, 8, 32, 32, goutou);
		OLED_Rotation_Block(88 + 16, 8 + 16, 16, Angle * 360 / 256);

		Angle += Menu_RollEvent() * 8;
		Angle += 2;

		OLED_ShowString(2, 0, "Menu V1.0", OLED_8X16);
		OLED_ShowString(2, 32, "By:LEIMING", OLED_8X16);
		

		OLED_Update();
		if (Menu_EnterEvent())
		{
			return;
		}
		if (Menu_BackEvent())
		{
			return;
		}
	}
}

/**********************************************************/
