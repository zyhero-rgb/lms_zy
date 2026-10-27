#include "adminwin.h"
#include "control.h"
#include "user.h"
#include <stdio.h>
#include <string.h>

// 管理员登录后跳转到该界面
int adminwin()
{
	printf("adminwin\n");
	// 窗口大小和坐标
	const int winWid = 780;
	const int winHgh = 500;
	const int win_x = (WIN_WIDTH - winWid) / 2;
	const int win_y = (WIN_HEIGHT - winHgh) / 2;

	const COLORREF bkColor = RGB(22, 35, 62);				// 背景颜色
	const COLORREF btnColor_default = RGB(240, 246, 253);	// 按钮默认颜色
	const COLORREF btnColor_focus = RGB(64, 158, 255);		// 按钮选中颜色
	const COLORREF btnTextColor = RGB(24, 34, 52);			// 按钮文字颜色
	const COLORREF labelColor = RGB(210, 224, 242);			// 标签文字颜色
	const COLORREF titleColor = RGB(255, 255, 255);			// 标题文字颜色
	const COLORREF statusColor = RGB(120, 200, 255);		// 提示文字颜色

	// 窗口
	WINDOW_T admin_win = {
		.x = win_x,
		.y = win_y,
		.width = winWid,
		.height = winHgh,
		.count = 0,
		.current = 0,
		.bgColor = bkColor
	};

	// 按钮宽高
	const int btnW = 212;
	const int btnH = 84;
	// 第一行按钮的x坐标
	const int col1 = win_x + 46;
	const int col2 = win_x + 284;
	const int col3 = win_x + 522;
	const int row1 = win_y + 170;
	const int row2 = win_y + 276;

	CONTROL_T btn_system = {
		.x = col1,
		.y = row1,
		.width = btnW,
		.height = btnH,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor
	};
	strcpy_s(btn_system.text, sizeof(btn_system.text), "系统管理");

	CONTROL_T btn_order = {
		.x = col2,
		.y = row1,
		.width = btnW,
		.height = btnH,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor
	};
	strcpy_s(btn_order.text, sizeof(btn_order.text), "订单管理");

	CONTROL_T btn_storage = {
		.x = col3,
		.y = row1,
		.width = btnW,
		.height = btnH,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor
	};
	strcpy_s(btn_storage.text, sizeof(btn_storage.text), "仓储管理");

	CONTROL_T btn_transport = {
		.x = col1,
		.y = row2,
		.width = btnW,
		.height = btnH,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor
	};
	strcpy_s(btn_transport.text, sizeof(btn_transport.text), "运输管理");

	CONTROL_T btn_statistics = {
		.x = col2,
		.y = row2,
		.width = btnW,
		.height = btnH,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor
	};
	strcpy_s(btn_statistics.text, sizeof(btn_statistics.text), "统计分析");

	CONTROL_T btn_logout = {
		.x = col3,
		.y = row2,
		.width = btnW,
		.height = btnH,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor
	};
	strcpy_s(btn_logout.text, sizeof(btn_logout.text), "退出登录");

	
	// 标签最后添加，不影响按钮索引
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
	strcpy_s(title.text, sizeof(title.text), "管理员工作台");

	CONTROL_T welcome = {
		.x = win_x + 40,
		.y = win_y + 86,
		.width = winWid - 80,
		.height = 30,
		.type = LABEL,
		.state = 0,
		.defaultColor = bkColor,
		.focusColor = bkColor,
		.textColor = labelColor,
		.fontSize = 20
	};
	sprintf_s(welcome.text, sizeof(welcome.text), "%s，欢迎回来",
		current != NULL ? current->name : "管理员");

	CONTROL_T status = {
		.x = win_x + 46,
		.y = win_y + 390,
		.width = winWid - 92,
		.height = 30,
		.type = LABEL,
		.state = 0,
		.defaultColor = bkColor,
		.focusColor = bkColor,
		.textColor = statusColor,
		.fontSize = 18
	};
	strcpy_s(status.text, sizeof(status.text), "请选择功能模块");

	// 先添加按钮，保证按钮索引为 0–5
	win_add_ctrl(&admin_win, &btn_system);     // 0
	win_add_ctrl(&admin_win, &btn_order);      // 1
	win_add_ctrl(&admin_win, &btn_storage);    // 2
	win_add_ctrl(&admin_win, &btn_transport);  // 3
	win_add_ctrl(&admin_win, &btn_statistics); // 4
	win_add_ctrl(&admin_win, &btn_logout);     // 5

	win_add_ctrl(&admin_win, &title);   // 6
	win_add_ctrl(&admin_win, &welcome); // 7
	win_add_ctrl(&admin_win, &status);  // 8

	admin_win = window_run(&admin_win);
	// 系统管理界面
	if (admin_win.current == 0)
	{
		return 3;
	}


	if (admin_win.current == 1)
	{
		return 10;
	}

	if (admin_win.current == 5)
	{
		return 0;
	}
	return 1;
}
