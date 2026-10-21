#include "startwin.h"
#include "control.h"
#include <string.h>

int startWin()
{
	// 先算出窗口左上角坐标
	int win_x = (WIN_WIDTH - 400) / 2;
	int win_y = (WIN_HEIGHT - 300) / 2;

	// 创建3个按钮
	CONTROL_T btn_login
	{
		.x = win_x + 40, 
		.y = win_y + 60, 
		.width = 320, 
		.height = 50, 
		.type = BUTTON, 
		.state = 0, 
		.defaultColor = WHITE,
		.focusColor = RED, 
		.textColor = BLACK
	};
	strcpy_s(btn_login.text, sizeof(btn_login.text), "1.登录");

	CONTROL_T btn_forget
	{
		.x = win_x + 40,
		.y = win_y + 120,
		.width = 320,
		.height = 50,
		.type = BUTTON,
		.state = 0,
		.defaultColor = WHITE,
		.focusColor = RED,
		.textColor = BLACK
	};
	strcpy_s(btn_forget.text, sizeof(btn_forget.text), "2.忘记密码");

	CONTROL_T btn_exit
	{
		.x = win_x + 40,
		.y = win_y + 180,
		.width = 320,
		.height = 50,
		.type = BUTTON,
		.state = 0,
		.defaultColor = WHITE,
		.focusColor = RED,
		.textColor = BLACK
	};
	strcpy_s(btn_exit.text, sizeof(btn_exit.text), "3.退出");

	// 初始界面窗口
	WINDOW_T first_win = {
		.x = win_x,
		.y = win_y,
		.width = STARTWIN_WIDTH,
		.height = STARTWIN_HEIGHT,
		.count = 0,
		.current = 0,
		.bgColor = BLUE
	};

	win_add_ctrl(&first_win, &btn_login);
	win_add_ctrl(&first_win, &btn_forget);
	win_add_ctrl(&first_win, &btn_exit);

	first_win = window_run(&first_win);
	if (first_win.current == 0)
	{
		return 1;
	}
	else if (first_win.current == 1)
	{

	}
	else if (first_win.current == 2)
	{
		exit(0);
	}
}
