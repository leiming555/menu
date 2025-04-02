#ifndef __MENU_H
#define __MENU_H
#include <stdio.h>
#include <stdarg.h>
#include "stm32f10x.h" // Device header
#include <string.h>
#include "OLED.h"
#include "Encoder.h"
#include "Key.h"

#include "Tools.h"

#include "Game_Snake.h"
#include "game_of_life.h"

struct Option_Class
{
	char *String;		// 选项名字
	void (*func)(void); // 函数指针
	float *Variable;	// 可选变量
	uint8_t StrLen;		// 由于中文占三个字节,用strlen计算名字长度不再准确,故需额外储存名字长度
};

enum CursorStyle
{
	reverse,
	mouse,
	frame,
};

struct MenuProperty
{
	float Cursor_Actual_X;		  // 当前光标位置X
	float Cursor_Actual_Y;		  // 当前光标位置Y
	float Cursor_Actual_W;		  // 当前光标尺寸宽
	float Cursor_Actual_H;		  // 当前光标尺寸高
	enum CursorStyle CursorStyle; // 光标风格;
	float Cursor_ActSpeed;		  // 光标动画速度系数;
	float Slide_ActSpeed;		  // 滚动动画速度系数;
	int8_t Font_Width;			  // 字体宽度
	int8_t Font_Height;			  // 字体宽度
	int8_t Line_Height;			  // 行高
	int8_t Layout_Margin;		  // 页边距

	int8_t Window_X;				// 窗口位置X
	int8_t Window_Y;                // 窗口位置Y
	uint8_t Window_W;               // 窗口宽度
	uint8_t Window_H;               // 窗口高度
};
extern struct MenuProperty Menu_Global;

void Menu_RunMenu(struct Option_Class *MU);
/**********************************************************/
void Menu_RunMainMenu(void);
void Menu_RunToolsMenu(void);
void Menu_RunGamesMenu(void);
void Menu_RunSettingMenu(void);
void Menu_RunLEDMenu(void);
void Menu_RunCurtainMenu(void);
void Menu_Information(void);
/**********************************************************/
int8_t Menu_RollEvent(void);
int8_t Menu_EnterEvent(void);
int8_t Menu_BackEvent(void);
/**********************************************************/
uint8_t Menu_GetOptionStrLen(char *String);
uint8_t Menu_PrintfOptionStr(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, uint8_t FontSize, char *format, ...);
/**********************************************************/
float Menu_CurveMigration(float Actual_Value, float Target_Value, float Act_Speed);
void Menu_ShowCursor(float Target_Cur_X, float Target_Cur_Y, float Target_Cur_W, float Target_Cur_H, enum CursorStyle CurStyle, float Cur_Act_Speed);
/**********************************************************/
void Menu_ShowWallpaper(const uint8_t *Wallpaper);
void Curtain_on(void);
void Curtain_off(void);

#endif
