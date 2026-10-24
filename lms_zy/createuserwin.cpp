#include <stdio.h>
#include <string.h>
#include "createuserwin.h"
#include "control.h"
#include "user.h"


// 创建角色/用户窗口
int createUer_win()
{

	printf("createUer_win\n");

	// 窗口大小和坐标
	const int winWid = 640;
	const int winHgh = 560;
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

	WINDOW_T create_win = {
		.x = win_x,
		.y = win_y,
		.width = winWid,
		.height = winHgh,
		.count = 0,
		.current = 0,
		.bgColor = bkColor
	};

	// 输入框和按钮的位置
	const int editX = win_x + 190;
	const int editW = 370;
	const int editH = 44;

	const int btnW = 150;
	const int btnH = 48;
	const int btnY = win_y + 390;
	const int okX = win_x + 150;
	const int cancelX = win_x + 340;

	// 用户名
	CONTROL_T user_edit = {
		.x = editX,
		.y = win_y + 103,
		.width = editW,
		.height = editH,
		.type = EDIT,
		.state = 0,
		.defaultColor = editColor,
		.focusColor = editFocusColor,
		.textColor = editTextColor,
		.fontSize = 24
	};

	// 密码
	CONTROL_T pwd_edit = {
		.x = editX,
		.y = win_y + 168,
		.width = editW,
		.height = editH,
		.type = EDIT_PWD,
		.state = 0,
		.defaultColor = editColor,
		.focusColor = editFocusColor,
		.textColor = editTextColor,
		.fontSize = 24
	};

	// 二次确认密码
	CONTROL_T confirm_edit = {
		.x = editX,
		.y = win_y + 233,
		.width = editW,
		.height = editH,
		.type = EDIT_PWD,
		.state = 0,
		.defaultColor = editColor,
		.focusColor = editFocusColor,
		.textColor = editTextColor,
		.fontSize = 24
	};

	// 角色
	CONTROL_T role_edit = {
		.x = editX,
		.y = win_y + 298,
		.width = editW,
		.height = editH,
		.type = EDIT,
		.state = 0,
		.defaultColor = editColor,
		.focusColor = editFocusColor,
		.textColor = editTextColor,
		.fontSize = 24
	};

	// 确定按钮
	CONTROL_T ok_button = {
		.x = okX,
		.y = btnY,
		.width = btnW,
		.height = btnH,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor,
		.fontSize = 24
	};
	strcpy_s(ok_button.text, sizeof(ok_button.text), "确定");

	// 取消按钮
	CONTROL_T cancel_button = {
		.x = cancelX,
		.y = btnY,
		.width = btnW,
		.height = btnH,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor,
		.fontSize = 24
	};
	strcpy_s(cancel_button.text, sizeof(cancel_button.text), "取消");

	// 先添加输入框和按钮，保证索引为 0–5
	win_add_ctrl(&create_win, &user_edit);    // 0 用户名输入框
	win_add_ctrl(&create_win, &pwd_edit);     // 1 密码输入框
	win_add_ctrl(&create_win, &confirm_edit); // 2 密码确认框
	win_add_ctrl(&create_win, &role_edit);    // 3 角色设置框
	win_add_ctrl(&create_win, &ok_button);    // 4 确认按钮
	win_add_ctrl(&create_win, &cancel_button);// 5 取消按钮

	// 标签最后添加，不影响输入框和按钮索引
	const int labelX = win_x + 70;
	const int labelW = 110;
	const int labelH = 30;

	// 提示标签标签
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
	strcpy_s(title.text, sizeof(title.text), "创建角色/用户");

	CONTROL_T user_label = {
		.x = labelX,
		.y = win_y + 110,
		.width = labelW,
		.height = labelH,
		.type = LABEL,
		.state = 0,
		.defaultColor = bkColor,
		.focusColor = bkColor,
		.textColor = labelColor,
		.fontSize = 20
	};
	strcpy_s(user_label.text, sizeof(user_label.text), "用户名");

	CONTROL_T pwd_label = {
		.x = labelX,
		.y = win_y + 175,
		.width = labelW,
		.height = labelH,
		.type = LABEL,
		.state = 0,
		.defaultColor = bkColor,
		.focusColor = bkColor,
		.textColor = labelColor,
		.fontSize = 20
	};
	strcpy_s(pwd_label.text, sizeof(pwd_label.text), "密码");

	CONTROL_T confirm_label = {
		.x = labelX,
		.y = win_y + 240,
		.width = labelW,
		.height = labelH,
		.type = LABEL,
		.state = 0,
		.defaultColor = bkColor,
		.focusColor = bkColor,
		.textColor = labelColor,
		.fontSize = 20
	};
	strcpy_s(confirm_label.text, sizeof(confirm_label.text), "确认密码");

	CONTROL_T role_label = {
		.x = labelX,
		.y = win_y + 305,
		.width = labelW,
		.height = labelH,
		.type = LABEL,
		.state = 0,
		.defaultColor = bkColor,
		.focusColor = bkColor,
		.textColor = labelColor,
		.fontSize = 20
	};
	strcpy_s(role_label.text, sizeof(role_label.text), "角色");

	CONTROL_T role_hint = {
		.x = editX,
		.y = win_y + 348,
		.width = editW,
		.height = 24,
		.type = LABEL,
		.state = 0,
		.defaultColor = bkColor,
		.focusColor = bkColor,
		.textColor = statusColor,
		.fontSize = 20
	};
	strcpy_s(role_hint.text, sizeof(role_hint.text),
		"角色填写：1 仓储员 / 2 调度员 / 3 客服 (输入对应的数字)");

	CONTROL_T status = {
		.x = labelX,
		.y = win_y + 450,
		.width = winWid - 140,
		.height = 26,
		.type = LABEL,
		.state = 0,
		.defaultColor = bkColor,
		.focusColor = bkColor,
		.textColor = statusColor,
		.fontSize = 16
	};
	strcpy_s(status.text, sizeof(status.text), "请输入完整的用户信息");

	win_add_ctrl(&create_win, &title);         // 6
	win_add_ctrl(&create_win, &user_label);    // 7
	win_add_ctrl(&create_win, &pwd_label);     // 8
	win_add_ctrl(&create_win, &confirm_label); // 9
	win_add_ctrl(&create_win, &role_label);    // 10
	win_add_ctrl(&create_win, &role_hint);     // 11
	win_add_ctrl(&create_win, &status);        // 12

	create_win = window_run(&create_win);

	// 确认按钮
	if (create_win.current == 4)
	{	
		/*
			1.输入用户名，密码，角色
			2.按下确认按钮
			3.判断是否都有输入
			4.用户名查重
			5.密码长度检查
			6.两次的密码是否相同
			7.获取角色选择的数字
			8.调用create_user函数创建用户，加入user_list链表，写入文件
		*/
		// 判断是否输入用户名，密码，确认密码，角色选择
		if (strlen(create_win.controls[0].text) == 0 || strlen(create_win.controls[1].text) == 0 || 
			strlen(create_win.controls[2].text) == 0 || strlen(create_win.controls[3].text) == 0)
		{
			// 输入密码或用户名为空，创建失败
			printf("please input user name and user pwd to create a user\n");
			return 4;
		}

		if (strlen(create_win.controls[0].text) > 30)
		{
			printf("user name length is out of range\n");
			return 4;
		}

		// 密码长度规定为6 - 10
		if (strlen(create_win.controls[1].text) > 10 || strlen(create_win.controls[1].text) < 6)
		{
			printf("password length is out of range\n");
			return 4;
		}
		// 用户名查重
		LNode* temp = user_list->next;
		if (temp != NULL)
		{
			char* _name = NULL;
			while (temp != NULL)
			{
				// 取链表节点的数据域
				USER_T* user = (USER_T*)temp->data;
				if (user == NULL)
				{
					temp = temp->next;
					continue;
				}
				_name = user->name;
				if (strcmp(_name, create_win.controls[0].text) == 0)
				{
					printf("%s already exists\n", create_win.controls[0].text);
					return 4;
				}
				temp = temp->next;
			}
		}
		// 判断两次密码是否相同
		if (strcmp(create_win.controls[1].text, create_win.controls[2].text) != 0)
		{
			printf("密码不一致\n");
			return 4;
		}

		if (strlen(create_win.controls[3].text) > 1)
		{
			printf("role choice is unavilable\n");
			return 4;
		}

		// 获取角色选择的第一个字符
		if (create_win.controls[3].text[0] < '1' || create_win.controls[3].text[0] >'3')
		{
			printf("role choice is out of range\n");
			return 4;
		}
		const int role = create_win.controls[3].text[0] - '0' + 1;
		create_user(create_win.controls[0].text, create_win.controls[1].text,role);
		return 3;
	}
	return 0;
}
