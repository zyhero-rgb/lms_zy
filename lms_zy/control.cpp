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
	bool isButton = control->type == BUTTON;
	bool isInput = control->type == EDIT || control->type == EDIT_PWD;
	int fontH = control->fontSize > 0 ? control->fontSize : 26;

	// 控件框：圆角、描边，按钮和输入框使用不同圆角
	if (isButton || isInput)
	{
		COLORREF fill = control->state == 1 ? control->focusColor : control->defaultColor;
		COLORREF border = control->state == 1? (isButton ? RGB(255, 255, 255) : RGB(64, 158, 255)): RGB(160, 175, 195);
		// 设置填充的颜色和边框的颜色
		setfillcolor(fill);
		setlinecolor(border);
		setlinestyle(PS_SOLID, 1);

		int radius = isButton ? 12 : 8;
		fillroundrect(control->x, control->y,control->x + control->width, control->y + control->height,radius, radius);
	}

	settextstyle(fontH, 0, _T("Microsoft YaHei"), 0, 0,(isButton || fontH >= 30) ? FW_BOLD : FW_NORMAL,false, false, false);
	setbkmode(TRANSPARENT);

	int textY = control->y + (control->height - fontH) / 2;

	// 标签
	if (control->type == LABEL)
	{
		settextcolor(control->textColor);
		int textX = control->x + 10;
		if (fontH >= 30)
		{
			textX = control->x + (control->width - textwidth(control->text)) / 2;
		}
		outtextxy(textX, textY, control->text);
	}
	// 普通输入框
	else if (control->type == EDIT)
	{
		settextcolor(control->textColor);
		outtextxy(control->x + 12, textY, control->text);
	}
	// 按钮
	else if (control->type == BUTTON)
	{
		settextcolor(control->state == 1 ? RGB(255, 255, 255) : control->textColor);
		int textX = control->x + (control->width - textwidth(control->text)) / 2;
		outtextxy(textX, textY, control->text);
	}
	// 密码框
	else if (control->type == EDIT_PWD)
	{
		settextcolor(control->textColor);
		char pwd[100] = { 0 };
		int len = (int)strlen(control->text);
		if (len > 99)
		{
			len = 99;
		}
		for (int i = 0; i < len; i++)
		{
			pwd[i] = '*';
		}
		pwd[len] = '\0';
		outtextxy(control->x + 12, textY, pwd);
	}
}

// 显示一个窗口
void window_show(const WINDOW_T* win)
{
	const int radius = 15;
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 1);
	// 填充窗口的背景色
	setfillcolor(win->bgColor);
	fillroundrect(win->x, win->y, win->x + win->width, win->y + win->height,radius,radius);
	// 显示窗口里的所有控件
	for (int i = 0; i < win->count; i++)
	{
		control_show(&(win->controls[i]));
	}

	// 如果窗口设置了额外的绘制函数，在这里画表格等内容
	if (win->draw_table != NULL)
	{
		win->draw_table(win);
	}
}

// 向窗口里添加一个控件
int win_add_ctrl(WINDOW_T* win, const CONTROL_T* ctrl)
{
	if (win->count >= 16)
	{
		printf("count is out range\n");
		return -1;
	}
	win->controls[win->count] = *ctrl;
	win->count++;
	return 0;
}

// 找下一个可选择的控件，仅当前的文件可使用
static int next_control(const WINDOW_T* win, int cur, int step)
{
	int count = win->count;
	do
	{
		cur = (cur + step + count) % count;
	} 
	while (win->controls[cur].type == LABEL);
	return cur;
}

// 处理窗口
WINDOW_T window_run(WINDOW_T* win )
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

	// 先使用背景图覆盖，防止窗口重叠
	clearScreen();
	window_show(win);
	show_time_now();

	// 记录上一次的时间
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
			// 先处理字符输入
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
					else if (msg.ch > 32 && msg.ch < 127 && len < 39)
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
				// 记录上次选中的控件，重新绘制新选中的控件和上次选中的控件
				int last = cur;
				win->controls[cur].state = 0;
				cur = next_control(win, cur, 1);
				win->controls[cur].state = 1;
				control_show(&win->controls[last]);
				control_show(&win->controls[cur]);
			}
			else if (msg.vkcode == VK_UP)
			{
				int last = cur;
				win->controls[cur].state = 0;
				cur = next_control(win, cur, -1);
				win->controls[cur].state = 1;
				control_show(&win->controls[last]);
				control_show(&win->controls[cur]);
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