#include "passwordresetwin.h"
#include "control.h"
#include <string.h>
#include "user.h"


// 密码重置界面
int password_reset_win()
{
	// 窗口大小和坐标
	const int winWid = 560;
	const int winHgh = 440;
	const int win_x = (WIN_WIDTH - winWid) / 2;
	const int win_y = (WIN_HEIGHT - winHgh) / 2;

	const COLORREF bkColor = RGB(22, 35, 62);				// 背景颜色
	const COLORREF btnColor_default = RGB(240, 246, 253);	// 按钮默认颜色
	const COLORREF btnColor_focus = RGB(64, 158, 255);		// 按钮选中颜色
	const COLORREF btnTextColor = RGB(24, 34, 52);			// 按钮文字颜色
	const COLORREF labelColor = RGB(210, 224, 242);			// 标签文字颜色
	const COLORREF titleColor = RGB(255, 255, 255);			// 标题文字颜色
	const COLORREF statusColor = RGB(120, 200, 255);		// 提示文字颜色
	const COLORREF editColor = RGB(245, 248, 252);			// 输入框默认颜色
	const COLORREF editFocusColor = RGB(214, 232, 250);		// 输入框选中颜色
	const COLORREF editTextColor = RGB(24, 34, 52);			// 输入框文字颜色

	// 重置密码窗口
	WINDOW_T reset_win = {
		.x = win_x,
		.y = win_y,
		.width = winWid,
		.height = winHgh,
		.count = 0,
		.current = 0,
		.bgColor = bkColor
	};

	// 用户名输入框
	CONTROL_T user_edit = {
		.x = win_x + 200,
		.y = win_y + 108,
		.width = 300,
		.height = 44,
		.type = EDIT,
		.state = 0,
		.defaultColor = editColor,
		.focusColor = editFocusColor,
		.textColor = editTextColor,
		.fontSize = 24
	};

	// 确认重置按钮
	CONTROL_T reset_button = {
		.x = win_x + 100,
		.y = win_y + 190,
		.width = 360,
		.height = 56,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor,
		.fontSize = 24
	};
	strcpy_s(reset_button.text, sizeof(reset_button.text), "确认重置");

	// 返回上级按钮
	CONTROL_T back_button = {
		.x = win_x + 100,
		.y = win_y + 270,
		.width = 360,
		.height = 56,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor,
		.fontSize = 24
	};
	strcpy_s(back_button.text, sizeof(back_button.text), "返回上级");

	// 先添加输入框和按钮，保证索引为 0–2
	win_add_ctrl(&reset_win, &user_edit);     // 0
	win_add_ctrl(&reset_win, &reset_button);  // 1
	win_add_ctrl(&reset_win, &back_button);   // 2

	// 标签最后添加，不影响输入框和按钮索引
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
	strcpy_s(title.text, sizeof(title.text), "密码重置");

	CONTROL_T user_label = {
		.x = win_x + 60,
		.y = win_y + 115,
		.width = 130,
		.height = 30,
		.type = LABEL,
		.state = 0,
		.defaultColor = bkColor,
		.focusColor = bkColor,
		.textColor = labelColor,
		.fontSize = 24
	};
	strcpy_s(user_label.text, sizeof(user_label.text), "输入用户名");

	// 状态提示
	CONTROL_T status = {
		.x = win_x + 60,
		.y = win_y + 360,
		.width = winWid - 120,
		.height = 28,
		.type = LABEL,
		.state = 0,
		.defaultColor = bkColor,
		.focusColor = bkColor,
		.textColor = statusColor,
		.fontSize = 16
	};
	strcpy_s(status.text, sizeof(status.text), "请输入要重置密码的用户名");

	win_add_ctrl(&reset_win, &title);      // 3
	win_add_ctrl(&reset_win, &user_label); // 4
	win_add_ctrl(&reset_win, &status);     // 5

	reset_win = window_run(&reset_win);
	if (reset_win.current == 1)
	{
		if (strlen(reset_win.controls[0].text) > 0)
		{
			resert_pwd(reset_win.controls[0].text);
			return 3;
		}
		else
		{
			// 输入为空，返回上级
			printf("user name is null\n");
			return 3;
		}
	}

	// 返回上级
	if (reset_win.current == 2)
	{
		return 3;
	}
	return 0;
}
