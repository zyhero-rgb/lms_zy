#include "control.h"
#include "loginwin.h"
#include <stdio.h>
#include "user.h"

int loginWin()
{

	// 登录窗口：300 × 400，居中
	WINDOW_T login_win = {
		.x = (WIN_WIDTH - LOGIN_WIN_WID) / 2,
		.y = (WIN_HEIGHT - LOGIN_WIN_HET) / 2,
		.width = LOGIN_WIN_WID,
		.height = LOGIN_WIN_HET,
		.count = 0,
		.current = 0,
		.bgColor = BLUE
	};

	// 控件宽 220，左右各留 40：40 + 220 + 40 = 300
	const int fieldW = 220;
	const int left = login_win.x + (login_win.width - fieldW) / 2;

	// 用户名标签
	CONTROL_T user_label = {
		.x = left,
		.y = login_win.y + 55,
		.width = fieldW,
		.height = 40,
		.type = LABEL,
		.state = 0,
		.defaultColor = BLUE,
		.focusColor = BLUE,
		.textColor = WHITE
	};
	strcpy_s(user_label.text, sizeof(user_label.text), "user name");

	// 用户名输入框
	CONTROL_T user_edit = {
		.x = left,
		.y = login_win.y + 100,
		.width = fieldW,
		.height = 40,
		.type = EDIT,
		.state = 1,
		.defaultColor = WHITE,
		.focusColor = RED,
		.textColor = BLACK
	};

	// 密码标签
	CONTROL_T pwd_label = {
		.x = left,
		.y = login_win.y + 150,
		.width = fieldW,
		.height = 40,
		.type = LABEL,
		.state = 0,
		.defaultColor = BLUE,
		.focusColor = BLUE,
		.textColor = WHITE
	};
	strcpy_s(pwd_label.text, sizeof(pwd_label.text), "password");

	// 密码输入框
	CONTROL_T pwd_edit = {
		.x = left,
		.y = login_win.y + 195,
		.width = fieldW,
		.height = 40,
		.type = EDIT_PWD,
		.state = 0,
		.defaultColor = WHITE,
		.focusColor = RED,
		.textColor = BLACK
	};

	// 登录按钮
	CONTROL_T login_button = {
		.x = left,
		.y = login_win.y + 250,
		.width = fieldW,
		.height = 40,
		.type = BUTTON,
		.state = 0,
		.defaultColor = WHITE,
		.focusColor = RED,
		.textColor = BLACK
	};
	strcpy_s(login_button.text, sizeof(login_button.text), "login");

	// 返回按钮
	CONTROL_T return_button = {
		.x = left,
		.y = login_win.y + 305,
		.width = fieldW,
		.height = 40,
		.type = BUTTON,
		.state = 0,
		.defaultColor = WHITE,
		.focusColor = RED,
		.textColor = BLACK
	};
	strcpy_s(return_button.text, sizeof(return_button.text), "back");

	win_add_ctrl(&login_win, &user_label);// 0
	win_add_ctrl(&login_win, &pwd_label); // 1
	win_add_ctrl(&login_win, &user_edit); // 2
	win_add_ctrl(&login_win, &pwd_edit); // 3
	win_add_ctrl(&login_win, &login_button); // 4 登录按钮
	win_add_ctrl(&login_win, &return_button);// 5 返回按钮

	login_win = window_run(&login_win);
	if (login_win.current == 4)
	{
		login_check(login_win.controls[2].text, login_win.controls[3].text);
		printf("login successfully\n");
		return 0;
	}
	else if (login_win.current == 5)
	{
		return 0;
	}
	return 0;
}