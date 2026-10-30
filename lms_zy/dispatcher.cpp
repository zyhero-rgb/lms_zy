#include "dispatcher.h"
#include "control.h"
#include "user.h"
#include <stdio.h>
#include <string.h>

// 调度员工作台
int dispatcher_win()
{
	const int winWid = 720;
	const int winHgh = 600;
	const int win_x = (WIN_WIDTH - winWid) / 2;
	const int win_y = (WIN_HEIGHT - winHgh) / 2;

	const COLORREF bkColor = RGB(22, 35, 62);
	const COLORREF btnColor_default = RGB(240, 246, 253);
	const COLORREF btnColor_focus = RGB(64, 158, 255);
	const COLORREF btnTextColor = RGB(24, 34, 52);
	const COLORREF labelColor = RGB(210, 224, 242);
	const COLORREF titleColor = RGB(255, 255, 255);
	const COLORREF statusColor = RGB(120, 200, 255);

	WINDOW_T win = {
		.x = win_x,
		.y = win_y,
		.width = winWid,
		.height = winHgh,
		.count = 0,
		.current = 0,
		.bgColor = bkColor,
		.draw_table = NULL
	};

	const char* items[] = {
		"车辆管理",
		"司机管理",
		"运输调度",
		"运输跟踪",
		"订单查询",
		"退出登录"
	};
	const int item_count = 6;

	const int btnW = 400;
	const int btnH = 50;
	const int btnX = win_x + (winWid - btnW) / 2;

	for (int i = 0; i < item_count; i++)
	{
		CONTROL_T btn = {
			.x = btnX,
			.y = win_y + 120 + i * 62,
			.width = btnW,
			.height = btnH,
			.type = BUTTON,
			.state = 0,
			.defaultColor = btnColor_default,
			.focusColor = btnColor_focus,
			.textColor = btnTextColor,
			.fontSize = 24
		};
		strcpy_s(btn.text, sizeof(btn.text), items[i]);
		win_add_ctrl(&win, &btn);
	}

	CONTROL_T title = {
		.x = win_x,
		.y = win_y + 28,
		.width = winWid,
		.height = 48,
		.type = LABEL,
		.state = 0,
		.defaultColor = bkColor,
		.focusColor = bkColor,
		.textColor = titleColor,
		.fontSize = 32
	};
	strcpy_s(title.text, sizeof(title.text), "调度员工作台");

	char welcome_text[80] = { 0 };
	sprintf_s(welcome_text, sizeof(welcome_text), "当前用户：%s（调度员）",
		current != NULL ? current->name : "未登录");

	CONTROL_T welcome = {
		.x = win_x + 50,
		.y = win_y + 90,
		.width = winWid - 100,
		.height = 30,
		.type = LABEL,
		.state = 0,
		.defaultColor = bkColor,
		.focusColor = bkColor,
		.textColor = labelColor,
		.fontSize = 20
	};
	strcpy_s(welcome.text, sizeof(welcome.text), welcome_text);

	CONTROL_T status = {
		.x = win_x + 50,
		.y = win_y + 520,
		.width = winWid - 100,
		.height = 26,
		.type = LABEL,
		.state = 0,
		.defaultColor = bkColor,
		.focusColor = bkColor,
		.textColor = statusColor,
		.fontSize = 16
	};
	strcpy_s(status.text, sizeof(status.text), "请选择功能模块");

	win_add_ctrl(&win, &title);
	win_add_ctrl(&win, &welcome);
	win_add_ctrl(&win, &status);

	win = window_run(&win);

	// 订单查询
	if (win.current == 4)
	{
		return 12;
	}
	// 退出登录
	if (win.current == 5)
	{
		return 0;
	}
	return 0;
}
