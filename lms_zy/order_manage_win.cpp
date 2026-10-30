#include "order_manage_win.h"
#include "control.h"
#include "user.h"
#include <stdio.h>
#include <string.h>
#include "date.h"

// 订单管理界面
int order_manage_win()
{
	const int winWid = 760;
	const int winHgh = 430;
	const int win_x = (WIN_WIDTH - winWid) / 2;
	const int win_y = (WIN_HEIGHT - winHgh) / 2;

	const COLORREF bkColor = RGB(22, 35, 62);
	const COLORREF btnColor_default = RGB(240, 246, 253);
	const COLORREF btnColor_focus = RGB(64, 158, 255);
	const COLORREF btnTextColor = RGB(24, 34, 52);
	const COLORREF labelColor = RGB(210, 224, 242);
	const COLORREF titleColor = RGB(255, 255, 255);
	const COLORREF statusColor = RGB(120, 200, 255);

	WINDOW_T order_manage_win = {
		.x = win_x,
		.y = win_y,
		.width = winWid,
		.height = winHgh,
		.count = 0,
		.current = 0,
		.bgColor = bkColor,
		.draw_table = NULL
	};

	const int btnW = 260;
	const int btnH = 56;
	const int col1 = win_x + 80;
	const int col2 = win_x + 400;
	const int row1 = win_y + 140;
	const int row2 = win_y + 205;
	const int row3 = win_y + 270;

	// 1. 创建订单
	CONTROL_T btn_create = {
		.x = col1,
		.y = row1,
		.width = btnW,
		.height = btnH,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor,
		.fontSize = 22
	};
	strcpy_s(btn_create.text, sizeof(btn_create.text), "1. 创建订单");

	// 2. 订单审核
	CONTROL_T btn_audit = {
		.x = col2,
		.y = row1,
		.width = btnW,
		.height = btnH,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor,
		.fontSize = 22
	};
	strcpy_s(btn_audit.text, sizeof(btn_audit.text), "2. 订单审核");

	// 3. 订单查询
	CONTROL_T btn_query = {
		.x = col1,
		.y = row2,
		.width = btnW,
		.height = btnH,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor,
		.fontSize = 22
	};
	strcpy_s(btn_query.text, sizeof(btn_query.text), "3. 订单查询");

	// 4. 订单修改
	CONTROL_T btn_modify = {
		.x = col2,
		.y = row2,
		.width = btnW,
		.height = btnH,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor,
		.fontSize = 22
	};
	strcpy_s(btn_modify.text, sizeof(btn_modify.text), "4. 订单修改");

	// 5. 订单跟踪
	CONTROL_T btn_track = {
		.x = col1,
		.y = row3,
		.width = btnW,
		.height = btnH,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor,
		.fontSize = 22
	};
	strcpy_s(btn_track.text, sizeof(btn_track.text), "5. 订单跟踪");

	// 6. 返回上级
	CONTROL_T btn_back = {
		.x = col2,
		.y = row3,
		.width = btnW,
		.height = btnH,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor,
		.fontSize = 22
	};
	strcpy_s(btn_back.text, sizeof(btn_back.text), "6. 返回上级");


	win_add_ctrl(&order_manage_win, &btn_create); // 0 创建订单
	win_add_ctrl(&order_manage_win, &btn_audit);  // 1 审核订单
	win_add_ctrl(&order_manage_win, &btn_query);  // 2 查询订单
	win_add_ctrl(&order_manage_win, &btn_modify); // 3 修改订单
	win_add_ctrl(&order_manage_win, &btn_track);  // 4 追踪订单
	win_add_ctrl(&order_manage_win, &btn_back);   // 5 返回上级

	// 标题
	CONTROL_T title = {
		.x = win_x,
		.y = win_y + 20,
		.width = winWid,
		.height = 48,
		.type = LABEL,
		.state = 0,
		.defaultColor = bkColor,
		.focusColor = bkColor,
		.textColor = titleColor,
		.fontSize = 30
	};
	strcpy_s(title.text, sizeof(title.text), "智能物流管理系统订单管理界面");

	// 当前用户
	char user_text[80] = { 0 };
	const char* role_text = "未知";
	if (current != NULL)
	{
		switch (current->role)
		{
		case 1: role_text = "管理员"; break;
		case 2: role_text = "仓储员"; break;
		case 3: role_text = "调度员"; break;
		case 4: role_text = "客服"; break;
		default: role_text = "未知"; break;
		}
	}
	sprintf_s(user_text, sizeof(user_text), "当前用户：%s（%s）",
		current != NULL ? current->name : "未登录", role_text);

	CONTROL_T current_label = {
		.x = win_x + 80,
		.y = win_y + 80,
		.width = 360,
		.height = 30,
		.type = LABEL,
		.state = 0,
		.defaultColor = bkColor,
		.focusColor = bkColor,
		.textColor = labelColor,
		.fontSize = 20
	};
	strcpy_s(current_label.text, sizeof(current_label.text), user_text);

	// 登录时间占位
	CONTROL_T login_label = {
		.x = win_x + 430,
		.y = win_y + 80,
		.width = 280,
		.height = 30,
		.type = LABEL,
		.state = 0,
		.defaultColor = bkColor,
		.focusColor = bkColor,
		.textColor = statusColor,
		.fontSize = 18
	};
	DATE_T date = get_time();

	sprintf_s(login_label.text, sizeof(login_label.text), "登录时间：%04d:%02d:%02d",date.year,date.month,date.day);

	win_add_ctrl(&order_manage_win, &title);          // 6
	win_add_ctrl(&order_manage_win, &current_label); // 7
	win_add_ctrl(&order_manage_win, &login_label);   // 8

	order_manage_win = window_run(&order_manage_win);

	// 创建
	if (order_manage_win.current == 0)
	{
		return 11;
	}
	// 审核
	else if (order_manage_win.current == 1)
	{
		return 10;
	}
	// 查询
	else if (order_manage_win.current == 2)
	{
		return 12;
	}
	else
	{
		return 2;
	}

}
