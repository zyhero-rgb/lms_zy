#include "systemmanagewin.h"
#include "control.h"
#include <string.h>

// 管理员系统管理界面
int system_manage_win()
{
	// 窗口大小和坐标
	const int winWid = 560;
	const int winHgh = 460;
	const int win_x = (WIN_WIDTH - winWid) / 2;
	const int win_y = (WIN_HEIGHT - winHgh) / 2;

	const COLORREF bkColor = RGB(22, 35, 62);				// 背景颜色
	const COLORREF btnColor_default = RGB(240, 246, 253);	// 按钮默认颜色
	const COLORREF btnColor_focus = RGB(64, 158, 255);		// 按钮选中颜色
	const COLORREF btnTextColor = RGB(24, 34, 52);			// 按钮文字颜色
	const COLORREF labelColor = RGB(210, 224, 242);			// 标签文字颜色
	const COLORREF titleColor = RGB(255, 255, 255);			// 标题文字颜色
	const COLORREF statusColor = RGB(120, 200, 255);		// 提示文字颜色

	WINDOW_T system_win = {
		.x = win_x,
		.y = win_y,
		.width = winWid,
		.height = winHgh,
		.count = 0,
		.current = 0,
		.bgColor = bkColor
	};

	// 按钮宽高和位置
	const int btnW = 360;
	const int btnH = 56;
	const int btnX = win_x + (winWid - btnW) / 2;

	CONTROL_T btn_create_role = create_ctrl(
		btnX, win_y + 110, btnW, btnH, BUTTON, 0,
		"创建角色/用户", btnColor_default, btnColor_focus, btnTextColor);

	CONTROL_T btn_query_user = create_ctrl(
		btnX, win_y + 180, btnW, btnH, BUTTON, 0,
		"用户查询", btnColor_default, btnColor_focus, btnTextColor);

	CONTROL_T btn_reset_pwd = create_ctrl(
		btnX, win_y + 250, btnW, btnH, BUTTON, 0,
		"密码重置", btnColor_default, btnColor_focus, btnTextColor);

	CONTROL_T btn_back = create_ctrl(
		btnX, win_y + 320, btnW, btnH, BUTTON, 0,
		"返回", btnColor_default, btnColor_focus, btnTextColor);

	// 先添加按钮，保证按钮索引为 0–3
	win_add_ctrl(&system_win, &btn_create_role); // 0
	win_add_ctrl(&system_win, &btn_query_user);  // 1
	win_add_ctrl(&system_win, &btn_reset_pwd);   // 2
	win_add_ctrl(&system_win, &btn_back);        // 3

	// 标题和状态提示最后添加，不影响按钮索引
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
		.fontSize = 30
	};
	strcpy_s(title.text, sizeof(title.text), "系统管理");

	CONTROL_T status = {
		.x = win_x + 60,
		.y = win_y + 400,
		.width = winWid - 120,
		.height = 28,
		.type = LABEL,
		.state = 0,
		.defaultColor = bkColor,
		.focusColor = bkColor,
		.textColor = statusColor,
		.fontSize = 18
	};
	strcpy_s(status.text, sizeof(status.text), "请选择系统管理功能");

	win_add_ctrl(&system_win, &title);   // 4
	win_add_ctrl(&system_win, &status);  // 5

	system_win = window_run(&system_win);

	return 0;
}
