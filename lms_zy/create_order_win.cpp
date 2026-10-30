#include "create_order_win.h"
#include "control.h"
#include <string.h>
#include "order.h"
#include "popupwin.h"
#include <stdlib.h>
#include "user.h"

// 绘制表单标题和字段标签
static void draw_order_form(const WINDOW_T* win)
{
	const int formX = win->x + 40;
	const int formY = win->y + 165;
	const int formW = 780;
	const int formH = 390;

	// 表单外框
	setlinecolor(RGB(90, 110, 140));
	setlinestyle(PS_SOLID, 1);
	rectangle(formX, formY, formX + formW, formY + formH);

	// 标题
	setbkmode(TRANSPARENT);
	settextcolor(RGB(255, 255, 255));
	settextstyle(30, 0, _T("Microsoft YaHei"), 0, 0, FW_BOLD, false, false, false);

	LPCTSTR title = _T("创建订单");
	outtextxy(win->x + (win->width - textwidth(title)) / 2, win->y + 20, title);

	// 字段标签
	settextcolor(RGB(210, 224, 242));
	settextstyle(18, 0, _T("Microsoft YaHei"));

	const int leftX = win->x + 60;
	const int rightX = win->x + 460;
	const int row1 = win->y + 210;
	const int row2 = win->y + 300;
	const int row3 = win->y + 390;
	const int row4 = win->y + 480;

	outtextxy(leftX, row1 - 28, _T("发货地址"));
	outtextxy(rightX, row1 - 28, _T("收货地址"));

	outtextxy(leftX, row2 - 28, _T("客户姓名"));
	outtextxy(rightX, row2 - 28, _T("客户电话"));

	outtextxy(leftX, row3 - 28, _T("货物名称"));
	outtextxy(rightX, row3 - 28, _T("货物类型（1-6）"));

	outtextxy(leftX, row4 - 28, _T("重量（kg）"));
	outtextxy(win->x + 310, row4 - 28, _T("数量"));
	outtextxy(win->x + 560, row4 - 28, _T("体积（L）"));

	// 底部说明
	settextcolor(RGB(120, 200, 255));
	outtextxy(win->x + 60, win->y + 570,
		_T("类型：0普通 1易碎 2冷链 3危险 4贵重 5大件"));
}

// 创建订单界面
int create_order_win()
{
	const int winWid = 860;
	const int winHgh = 650;
	const int win_x = (WIN_WIDTH - winWid) / 2;
	const int win_y = (WIN_HEIGHT - winHgh) / 2;

	const COLORREF bkColor = RGB(22, 35, 62);
	const COLORREF btnColor_default = RGB(240, 246, 253);
	const COLORREF btnColor_focus = RGB(64, 158, 255);
	const COLORREF btnTextColor = RGB(24, 34, 52);
	const COLORREF editColor = RGB(245, 248, 252);
	const COLORREF editFocusColor = RGB(214, 232, 250);
	const COLORREF editTextColor = RGB(24, 34, 52);

	// 窗口
	WINDOW_T order_win = {
		.x = win_x,
		.y = win_y,
		.width = winWid,
		.height = winHgh,
		.count = 0,
		.current = 0,
		.bgColor = bkColor,
		.draw_table = draw_order_form
	};

	// 确认创建
	CONTROL_T confirm_button = {
		.x = win_x + 80,
		.y = win_y + 90,
		.width = 180,
		.height = 48,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor,
		.fontSize = 22
	};
	strcpy_s(confirm_button.text, sizeof(confirm_button.text), "确认创建");

	// 取消创建
	CONTROL_T cancel_button = {
		.x = win_x + 300,
		.y = win_y + 90,
		.width = 180,
		.height = 48,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor,
		.fontSize = 22
	};
	strcpy_s(cancel_button.text, sizeof(cancel_button.text), "取消创建");

	// 返回上级
	CONTROL_T back_button = {
		.x = win_x + 680,
		.y = win_y + 90,
		.width = 140,
		.height = 48,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor,
		.fontSize = 20
	};
	strcpy_s(back_button.text, sizeof(back_button.text), "返回上级");

	const int editW = 350;
	const int editH = 44;
	const int rightX = win_x + 460;

	// 发货地址
	CONTROL_T from_edit = {
		.x = win_x + 60,
		.y = win_y + 210,
		.width = editW,
		.height = editH,
		.type = EDIT,
		.state = 0,
		.defaultColor = editColor,
		.focusColor = editFocusColor,
		.textColor = editTextColor,
		.fontSize = 20
	};

	// 收货地址
	CONTROL_T to_edit = {
		.x = rightX,
		.y = win_y + 210,
		.width = editW,
		.height = editH,
		.type = EDIT,
		.state = 0,
		.defaultColor = editColor,
		.focusColor = editFocusColor,
		.textColor = editTextColor,
		.fontSize = 20
	};

	// 客户姓名
	CONTROL_T name_edit = {
		.x = win_x + 60,
		.y = win_y + 300,
		.width = editW,
		.height = editH,
		.type = EDIT,
		.state = 0,
		.defaultColor = editColor,
		.focusColor = editFocusColor,
		.textColor = editTextColor,
		.fontSize = 20
	};

	// 客户电话
	CONTROL_T phone_edit = {
		.x = rightX,
		.y = win_y + 300,
		.width = editW,
		.height = editH,
		.type = EDIT,
		.state = 0,
		.defaultColor = editColor,
		.focusColor = editFocusColor,
		.textColor = editTextColor,
		.fontSize = 20
	};

	// 货物名称
	CONTROL_T goods_edit = {
		.x = win_x + 60,
		.y = win_y + 390,
		.width = editW,
		.height = editH,
		.type = EDIT,
		.state = 0,
		.defaultColor = editColor,
		.focusColor = editFocusColor,
		.textColor = editTextColor,
		.fontSize = 20
	};

	// 货物类型
	CONTROL_T type_edit = {
		.x = rightX,
		.y = win_y + 390,
		.width = editW,
		.height = editH,
		.type = EDIT,
		.state = 0,
		.defaultColor = editColor,
		.focusColor = editFocusColor,
		.textColor = editTextColor,
		.fontSize = 20
	};

	// 重量
	CONTROL_T weight_edit = {
		.x = win_x + 60,
		.y = win_y + 480,
		.width = 210,
		.height = editH,
		.type = EDIT,
		.state = 0,
		.defaultColor = editColor,
		.focusColor = editFocusColor,
		.textColor = editTextColor,
		.fontSize = 20
	};

	// 数量
	CONTROL_T count_edit = {
		.x = win_x + 310,
		.y = win_y + 480,
		.width = 210,
		.height = editH,
		.type = EDIT,
		.state = 0,
		.defaultColor = editColor,
		.focusColor = editFocusColor,
		.textColor = editTextColor,
		.fontSize = 20
	};

	// 体积
	CONTROL_T volume_edit = {
		.x = win_x + 560,
		.y = win_y + 480,
		.width = 210,
		.height = editH,
		.type = EDIT,
		.state = 0,
		.defaultColor = editColor,
		.focusColor = editFocusColor,
		.textColor = editTextColor,
		.fontSize = 20
	};

	// 按钮和输入框索引 0–11
	win_add_ctrl(&order_win, &confirm_button); // 0 确认
	win_add_ctrl(&order_win, &cancel_button);  // 1 取消
	win_add_ctrl(&order_win, &back_button);    // 2 返回上级
	win_add_ctrl(&order_win, &from_edit);      // 3 发货地址
	win_add_ctrl(&order_win, &to_edit);        // 4 收货地址
	win_add_ctrl(&order_win, &name_edit);      // 5 客户名
	win_add_ctrl(&order_win, &phone_edit);     // 6 客户电话
	win_add_ctrl(&order_win, &goods_edit);     // 7 货物名
	win_add_ctrl(&order_win, &type_edit);      // 8 货物类型 int
	win_add_ctrl(&order_win, &weight_edit);    // 9 货物总重量 float
	win_add_ctrl(&order_win, &count_edit);     // 10 货物数量 int 
	win_add_ctrl(&order_win, &volume_edit);    // 11 货物总体积 float

	
	while (true)
	{
		order_win = window_run(&order_win);
		// 确认创建订单
		if (order_win.current == 0)
		{
			// 保证所有的信息都有输入
			if (strlen(order_win.controls[3].text) == 0 || strlen(order_win.controls[4].text) == 0
				|| strlen(order_win.controls[5].text) == 0 || strlen(order_win.controls[6].text) == 0
				|| strlen(order_win.controls[7].text) == 0 || strlen(order_win.controls[8].text) == 0
				|| strlen(order_win.controls[9].text) == 0 || strlen(order_win.controls[10].text) == 0
				|| strlen(order_win.controls[11].text) == 0|| strlen(order_win.controls[8].text) > 1
				|| strlen(order_win.controls[10].text) > 4)
			{
				// 弹窗提醒
				create_popup_win(false);
			}
			else
			{
				int type = atoi(order_win.controls[8].text);		// 类型
				int count = atoi(order_win.controls[10].text);		// 数量
				float weight = atof(order_win.controls[9].text);	// 重量
				float volume = atof(order_win.controls[11].text);	// 体积
				if (type < 0 || type > 5 || count < 0 || count > 1000 || weight < 0 || volume < 0)
				{
					// 弹窗提醒
					create_popup_win(false);
				}
				else
				{
					int status = create_new_order(order_win.controls[3].text,order_win.controls[4].text, order_win.controls[5].text,
						order_win.controls[6].text, order_win.controls[7].text,type,weight,count,volume);
					create_popup_win(status);

				}
			}
			// 清空输入框
			for (int i = 3; i <= 11; i++)
			{
				order_win.controls[i].text[0] = '\0';
			}
		}
		else if (order_win.current == 1)
		{
			return 11;
		}
		else if (order_win.current == 2)
		{
			// 管理员
			if (current->role == 1)
			{
				return 13;
			}
			// 客服
			else if (current->role == 4)
			{
				return 7;
			}
		}
	}

	return 0;
}
