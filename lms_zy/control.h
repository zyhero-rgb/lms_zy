#ifndef _CONTROL_H
#define _CONTROL_H

#include <graphics.h>

// 主窗口的宽高
#define WIN_WIDTH 1024
#define WIN_HEIGHT 768

// 控件的类型
#define BUTTON 	 1
#define EDIT	 2
#define EDIT_PWD 3
#define LABEL	 4 

extern IMAGE g_background;

// 控件结构体
typedef struct control_T
{	
	int x;		// 控件的坐标 
	int y;				
	int width;	// 控件的宽高
	int height;
	int type;	// 控件类型 
	int state;	// 选择状态 1：被选择 0：为选择
	char text[40];
	COLORREF defaultColor;
	COLORREF focusColor;
	COLORREF textColor;
	int fontSize;	// 0 表示使用默认字号 26
}CONTROL_T;

// 窗口结构体，背景色和控件组成
typedef struct window_t
{
	int x, y;			// 位置坐标
	int width, height;	// 大小
	int count;			// 控件数量
	int current;		// 当前停留的控件
	CONTROL_T controls[10]; 
	COLORREF bgColor;   // 背景色
}WINDOW_T;


// 控件显示函数
void control_show(const CONTROL_T* control);

// 向窗口里添加一个控件
int win_add_ctrl(WINDOW_T* win,const CONTROL_T* ctrl);

// 显示一个窗口
void window_show(const WINDOW_T* win);

// 创建一个控件
CONTROL_T create_ctrl(int _x,int _y,int _width,int _height,int _type,
	int _state,const char* _text, COLORREF defColor, COLORREF focusColor, COLORREF textColor);

// 处理窗口
WINDOW_T window_run(WINDOW_T* win);

// 显示当前时间
void show_time_now();

// 加载背景图片
void init_bk_image(LPCTSTR path);

// 清屏
void clearScreen();


#endif // 