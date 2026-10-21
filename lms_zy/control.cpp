#define _CRT_SECURE_NO_WARNINGS
#include <graphics.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "control.h"
#include "date.h"
#include <windows.h>

IMAGE  g_background;

// 控件显示函数
void control_show(const CONTROL_T* control)
{
	int i;
	// 控件框 
	if (control->type == BUTTON || control->type == EDIT || control->type == EDIT_PWD)
	{
		// 被选中 
		if (control->state == 1)
		{
			setfillcolor(control->focusColor);
		}
		else
		{
			setfillcolor(control->defaultColor);
		}
		fillrectangle(control->x, control->y, control->x + control->width, control->y + control->height);
	}

	// 显示文本
	settextstyle(26, 0, _T("Microsoft YaHei"));
	settextcolor(control->textColor);
	setbkmode(TRANSPARENT);
	// 标签和编辑框 
	if (control->type == LABEL || control->type == EDIT)
	{
		outtextxy(control->x + 5, control->y + 13, control->text);
	}
	// 按钮文本居中显示 
	else if (control->type == BUTTON)
	{
		int textWidth = textwidth(control->text);
		int center_x = control->x + (control->width - textWidth) / 2;
		outtextxy(center_x, control->y + 13, control->text);
	}
	// 密码框 
	else if (control->type == EDIT_PWD)
	{
		char pwd[100];
		for (i = 0; i < strlen(control->text); i++)
		{
			pwd[i] = '*';
		}
		pwd[i] = '\0';
		outtextxy(control->x + 5, control->y + 13, pwd);
	}
}

// 显示一个窗口
void window_show(const WINDOW_T* win)
{
	// 填充窗口的背景色
	setfillcolor(win->bgColor);
	fillrectangle(win->x, win->y, win->x + win->width, win->y + win->height);
	// 显示窗口里的所有控件
	for (int i = 0; i < win->count; i++)
	{
		control_show(&(win->controls[i]));
	}
}

// 向窗口里添加一个控件
int win_add_ctrl(WINDOW_T* win, const CONTROL_T* ctrl)
{
	if (win->count >= 10)
	{
		printf("count is out range\n");
		return -1;
	}
	win->controls[win->count] = *ctrl;
	win->count++;
	return 0;
}

// 创建一个控件
CONTROL_T create_ctrl(int _x, int _y, int _width, int _height, int _type,
	int _state, const char* _text, COLORREF defColor, COLORREF focusColor, COLORREF textColor)
{
	CONTROL_T c;
	c.x = _x;
	c.y = _y;
	c.width = _width;
	c.height = _height;
	c.type = _type;
	c.state = 0;	// 默认不选中
	strcpy_s(c.text, sizeof(c.text),_text);
	c.defaultColor = defColor;
	c.focusColor = focusColor;
	c.textColor = textColor;
	return c;
}

// 找下一个可选择的控件，仅当前的文件可使用
static int next_control(const WINDOW_T* win, int cur, int step)
{
	int count = win->count;
	do
	{
		cur = (cur + step + count) % count;
	} while (win->controls[cur].type == LABEL);
	return cur;
}

// 处理窗口
WINDOW_T window_run(WINDOW_T* win)
{
	// 找到第一个不是标签的控件
	int cur = 0;
	while (cur < win->count && win->controls[cur].type == LABEL)
	{
		cur++;
	}
	if (cur >= win->count)
	{
		return *win;
	}
	win->controls[cur].state = 1;
	clearScreen();
	window_show(win);
	show_time_now();

	char lastTime[64] = { 0 };
	{
		std::lock_guard<std::mutex> lock(time_mtx);
		strcpy_s(lastTime, sizeof(lastTime), dateStr);
	}

	ExMessage msg;
	while (1)
	{
		// 时间变了就只重画时间区域，不整屏清屏
		// 获取当前时间
		char nowTime[64] = { 0 };
		{
			std::lock_guard<std::mutex> lock(time_mtx);
			strcpy_s(nowTime, sizeof(nowTime), dateStr);
		}
		// 判断时间是否发生变化
		if (strcmp(nowTime, lastTime) != 0)
		{
			show_time_now();
			strcpy_s(lastTime, sizeof(lastTime), nowTime);
		}

		// 非阻塞读键盘，等待按键时时间也能继续刷新
		// 同时接收键盘消息和字符输入消息
		while (peekmessage(&msg, EX_KEY | EX_CHAR))
		{
			// 先出里字符输入
			if (msg.message == WM_CHAR)
			{
				CONTROL_T* c = &win->controls[cur];
				if (c->type == EDIT || c->type == EDIT_PWD)
				{
					size_t len = strlen(c->text);
					if (len >= 39)
					{
						printf("text length is out of range\n");
					}
					// 处理退格
					if (msg.ch == '\b')
					{
						if (len > 0)
						{
							c->text[len -1] = '\0';
							printf("%s\n", c->text);
						}
					}
					else if (msg.ch >= 32 && msg.ch < 127 && len < 39)
					{
						c->text[len] = (char)msg.ch;
						c->text[len + 1] = '\0';
						printf("%s\n", c->text);
					}
					// 每次追加一个字符，就重新绘制改编辑控件
					control_show(c);
				}
				// 处理回车
				if (c->type == BUTTON && msg.ch == '\r')
				{
					win->current = cur;
					return *win;
				}
				continue;
			}

			// 跳过按键弹起
			if (msg.message != WM_KEYDOWN)
			{
				continue;
			}

			if (msg.vkcode == VK_ESCAPE)
			{
				return *win;
			}
			// 按下向上和向下键，先更新当前控件状态，再更新下一个非LABEL类控件的状态
			else if (msg.vkcode == VK_DOWN)
			{
				win->controls[cur].state = 0;
				cur = next_control(win, cur, 1);
				win->controls[cur].state = 1;
				window_show(win);
			}
			else if (msg.vkcode == VK_UP)
			{
				win->controls[cur].state = 0;
				cur = next_control(win, cur, -1);
				win->controls[cur].state = 1;
				window_show(win);
			}
		}

		Sleep(20);
	}
}

// 显示当前时间
void show_time_now()
{
	static IMAGE timeBg;
	static bool bgSaved = false;
	static int timeX = 0;
	static int timeY = 10;
	static int timeW = 0;
	static int timeH = 0;

	settextstyle(40, 0, _T("Microsoft YaHei"));
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);

	char timeText[64] = { 0 };
	{
		std::lock_guard<std::mutex> lock(time_mtx);
		strcpy_s(timeText, sizeof(timeText), dateStr);
	}

	if (!bgSaved)
	{
		// 用固定格式的示例字符串计算区域，保证位置和大小固定
		const char* sample = "0000-00-00 00:00:00";
		int textWid = textwidth(sample);
		timeX = WIN_WIDTH - textWid - 15;
		timeW = textWid + 12;
		timeH = textheight(sample) + 10;
		// 第一次先把时间区域的原始背景保存下来
		getimage(&timeBg, timeX, timeY, timeW, timeH);
		bgSaved = true;
	}
	else
	{
		// 先把上一秒的时间擦掉（贴回原背景），再画新时间
		putimage(timeX, timeY, &timeBg);
	}

	outtextxy(timeX, timeY, timeText);
}

// 加载背景图片
void init_bk_image(LPCTSTR path)
{
	loadimage(&g_background,path,WIN_WIDTH,WIN_HEIGHT,true);
	putimage(0,0,&g_background);
}

// 清屏
void clearScreen()
{
	putimage(0, 0, &g_background);
}