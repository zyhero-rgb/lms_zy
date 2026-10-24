#include "startwin.h"
#include "control.h"
#include <string.h>

int startWin()
{
	// 460 x 360 的主菜单窗口
	const int winW = 460;
	const int winH = 360;

	// 窗口坐标
	const int win_x = (WIN_WIDTH - winW) / 2;
	const int win_y = (WIN_HEIGHT - winH) / 2;

	const int fieldW = 340;
	const int left = win_x + (winW - fieldW) / 2;

	const COLORREF panel = RGB(33, 53, 94);					// 背景颜色
	const COLORREF btnColor_focus = RGB(64, 158, 255);		// 按钮被选择是的颜色
	const COLORREF buttonBg = RGB(240, 246, 253);			// 按钮背景颜色
	const COLORREF buttonText = RGB(24, 34, 52);

	// 主菜单按钮
	CONTROL_T btn_login
	{
		.x = left,
		.y = win_y + 120,
		.width = fieldW,
		.height = 54,
		.type = BUTTON,
		.state = 0,
		.defaultColor = buttonBg,
		.focusColor = btnColor_focus,
		.textColor = buttonText,
		.fontSize = 28
	};
	strcpy_s(btn_login.text, sizeof(btn_login.text), "登录系统");

	CONTROL_T btn_forget
	{
		.x = left,
		.y = win_y + 190,
		.width = fieldW,
		.height = 54,
		.type = BUTTON,
		.state = 0,
		.defaultColor = buttonBg,
		.focusColor = btnColor_focus,
		.textColor = buttonText,
		.fontSize = 28
	};
	strcpy_s(btn_forget.text, sizeof(btn_forget.text), "忘记密码");

	CONTROL_T btn_exit
	{
		.x = left,
		.y = win_y + 260,
		.width = fieldW,
		.height = 54,
		.type = BUTTON,
		.state = 0,
		.defaultColor = buttonBg,
		.focusColor = btnColor_focus,
		.textColor = buttonText,
		.fontSize = 28
	};
	strcpy_s(btn_exit.text, sizeof(btn_exit.text), "退出系统");

	// 初始界面窗口
	WINDOW_T first_win = {
		.x = win_x,
		.y = win_y,
		.width = winW,
		.height = winH,
		.count = 0,
		.current = 0,
		.bgColor = panel
	};

	// 先添加按钮
	win_add_ctrl(&first_win, &btn_login);	// 0
	win_add_ctrl(&first_win, &btn_forget);	// 1
	win_add_ctrl(&first_win, &btn_exit);	// 2

	// 标题
	CONTROL_T title
	{
		.x = win_x,
		.y = win_y + 30,
		.width = winW,
		.height = 48,
		.type = LABEL,
		.state = 0,
		.defaultColor = panel,
		.focusColor = panel,
		.textColor = RGB(255, 255, 255),
		.fontSize = 32
	};
	strcpy_s(title.text, sizeof(title.text), "智能物流管理系统");
	win_add_ctrl(&first_win, &title);

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
