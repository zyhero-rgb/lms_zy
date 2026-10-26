#include "control.h"
#include "loginwin.h"
#include <stdio.h>
#include "user.h"

int loginWin()
{
	// 380 x 460 的登录窗口
	const int winW = 380;
	const int winH = 460;
	const int win_x = (WIN_WIDTH - winW) / 2;
	const int win_y = (WIN_HEIGHT - winH) / 2;

	const int fieldW = 280;
	const int left = win_x + (winW - fieldW) / 2;

	const COLORREF panel = RGB(22, 35, 62);
	const COLORREF accent = RGB(64, 158, 255);
	const COLORREF buttonBg = RGB(240, 246, 253);
	const COLORREF buttonText = RGB(24, 34, 52);
	const COLORREF labelText = RGB(202, 216, 236);
	const COLORREF editBg = RGB(245, 248, 252);
	const COLORREF editFocus = RGB(214, 232, 250);
	const COLORREF editText = RGB(24, 34, 52);

	WINDOW_T login_win = {
		.x = win_x,
		.y = win_y,
		.width = winW,
		.height = winH,
		.count = 0,
		.current = 0,
		.bgColor = panel
	};

	// 用户名标签
	CONTROL_T user_label = {
		.x = left,
		.y = win_y + 105,
		.width = fieldW,
		.height = 28,
		.type = LABEL,
		.state = 0,
		.defaultColor = panel,
		.focusColor = panel,
		.textColor = labelText,
		.fontSize = 20
	};
	strcpy_s(user_label.text, sizeof(user_label.text), "用户名");

	// 用户名输入框
	CONTROL_T user_edit = {
		.x = left,
		.y = win_y + 135,
		.width = fieldW,
		.height = 44,
		.type = EDIT,
		.state = 1,
		.defaultColor = editBg,
		.focusColor = editFocus,
		.textColor = editText,
		.fontSize = 24
	};

	// 密码标签
	CONTROL_T pwd_label = {
		.x = left,
		.y = win_y + 200,
		.width = fieldW,
		.height = 28,
		.type = LABEL,
		.state = 0,
		.defaultColor = panel,
		.focusColor = panel,
		.textColor = labelText,
		.fontSize = 20
	};
	strcpy_s(pwd_label.text, sizeof(pwd_label.text), "密码");

	// 密码输入框
	CONTROL_T pwd_edit = {
		.x = left,
		.y = win_y + 230,
		.width = fieldW,
		.height = 44,
		.type = EDIT_PWD,
		.state = 0,
		.defaultColor = editBg,
		.focusColor = editFocus,
		.textColor = editText,
		.fontSize = 24
	};

	// 登录按钮
	CONTROL_T login_button = {
		.x = left,
		.y = win_y + 305,
		.width = fieldW,
		.height = 46,
		.type = BUTTON,
		.state = 0,
		.defaultColor = buttonBg,
		.focusColor = accent,
		.textColor = buttonText,
		.fontSize = 24
	};
	strcpy_s(login_button.text, sizeof(login_button.text), "登 录");

	// 返回按钮
	CONTROL_T return_button = {
		.x = left,
		.y = win_y + 365,
		.width = fieldW,
		.height = 46,
		.type = BUTTON,
		.state = 0,
		.defaultColor = buttonBg,
		.focusColor = accent,
		.textColor = buttonText,
		.fontSize = 24
	};
	strcpy_s(return_button.text, sizeof(return_button.text), "返 回");

	win_add_ctrl(&login_win, &user_label);// 0
	win_add_ctrl(&login_win, &pwd_label); // 1
	win_add_ctrl(&login_win, &user_edit); // 2
	win_add_ctrl(&login_win, &pwd_edit); // 3
	win_add_ctrl(&login_win, &login_button); // 4 登录按钮
	win_add_ctrl(&login_win, &return_button);// 5 返回按钮

	CONTROL_T title = {
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
	strcpy_s(title.text, sizeof(title.text), "用户登录");
	win_add_ctrl(&login_win, &title);

	login_win = window_run(&login_win);
	if (login_win.current == 4)
	{
		if (login_check(login_win.controls[2].text, login_win.controls[3].text))
		{
			printf("name:%s\n", login_win.controls[2].text);
			printf("pwd:%s\n", login_win.controls[3].text);
			printf("login successfully\n");
			// 跳转到管理员界面
			if (current->role == 1 && current->state == 1)
			{
				return 2;
			}
			// 仓储员
			if (current->role == 2 && current->state == 1)
			{
				return 9;
			}
			// 调度员
			if (current->role == 3 && current->state == 1)
			{
				return 8;
			}
			// 客服
			if (current->role == 4 && current->state == 1)
			{
				return 7;
			}
		}
		else
		{
			printf("name:%s\n", login_win.controls[2].text);
			printf("pwd:%s\n", login_win.controls[3].text);
			printf("fail to login\n");
			return 1;
		}
	}
	else if (login_win.current == 5)
	{
		return 0;
	}
	return 0;
}
