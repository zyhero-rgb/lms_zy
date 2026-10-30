#include <stdio.h>
#include "control.h"
#include "order.h"
#include <string.h>
#include <stdlib.h>
#include "user.h"
#include "popupwin.h"


static ORDER_T* orders[200];
static int orders_count;
static int cur_order;
static int per_page = 4; // 查询结果每页显示数量

// 加载全部订单
static void load_all_orders()
{
	orders_count = 0;
	if (order_list == NULL)
	{
		printf("load data unsuccessfully\n");
		return;
	}
	LNode* temp = order_list->next;
	while (temp != NULL)
	{
		ORDER_T* order = (ORDER_T*)temp->data;
		if (order != NULL)
		{
			orders[orders_count] = order;
			orders_count++;
		}
		temp = temp->next;
	}
}


/*
	*订单查询
	*所有角色可按订单号、客户姓名、订单状态等条件查询订单
*/
static bool query_order(const char* order_ID = "", const char* cus_name = "", const int order_status = -1)
{
	orders_count = 0;

	if (order_list == NULL || order_ID == NULL || cus_name == NULL)
	{
		return false;
	}

	LNode* temp = order_list->next;

	while (temp != NULL)
	{
		ORDER_T* order = (ORDER_T*)temp->data;

		if (order != NULL)
		{
			bool match = true;

			if (order_ID[0] != '\0' && strcmp(order->oder_ID, order_ID) != 0)
			{
				match = false;
			}

			if (cus_name[0] != '\0' && strstr(order->customer_name, cus_name) == NULL)
			{
				match = false;
			}
			 
			if (order_status != -1 && order->order_status != order_status)
			{
				match = false;
			}

			if (match && orders_count < 200)
			{
				orders[orders_count++] = order;
			}
		}

		temp = temp->next;
	}
	printf("orders_count :%d\n",orders_count);
	return orders_count > 0;
}

// 订单状态文字
static const char* query_status_text(int status)
{
	switch (status)
	{
	case ORDER_PENDING_REVIEW:    return "待审核";
	case ORDER_REJECTED:          return "已驳回";
	case ORDER_PENDING_OUTBOUND:  return "待出库";
	case ORDER_PENDING_TRANSPORT: return "待运输";
	case ORDER_IN_TRANSIT:        return "运输中";
	case ORDER_DELIVERED:         return "已送达";
	case ORDER_COMPLETED:         return "已完成";
	default:                      return "未知";
	}
}

// 绘制查询结果区域
static void draw_query_frame(const WINDOW_T* win)
{
	const int x = win->x + 50;
	const int y = win->y + 240;
	const int w = 860;
	const int h = 350;

	setfillcolor(RGB(245, 248, 252));
	solidrectangle(x, y, x + w, y + h);

	setlinecolor(RGB(120, 140, 170));
	setlinestyle(PS_SOLID, 1);
	rectangle(x, y, x + w, y + h);

	setbkmode(TRANSPARENT);
	settextcolor(RGB(24, 34, 52));
	settextstyle(20, 0, _T("Microsoft YaHei"), 0, 0, FW_BOLD, false, false, false);
	outtextxy(x + 20, y + 10, _T("查询结果"));

	// 未查找到订单
	if (orders_count == 0)
	{
		settextstyle(20, 0, _T("Microsoft YaHei"));
		settextcolor(RGB(120, 135, 155));
		const char* tip = "没有找到订单";
		outtextxy(x + (w - textwidth(tip)) / 2,
			y + (h - textheight(tip)) / 2, tip);
		return;
	}

	// 总页数和当前页
	int total_pages = (orders_count + per_page - 1) / per_page;
	int page = cur_order;
	if (page < 0)
		page = 0;
	if (page >= total_pages)
		page = total_pages - 1;

	char page_text[64] = { 0 };
	sprintf_s(page_text, sizeof(page_text),"第 %d / %d 页", page + 1, total_pages);

	settextstyle(16, 0, _T("Microsoft YaHei"));
	settextcolor(RGB(90, 105, 125));
	outtextxy(x + w - textwidth(page_text) - 20, y + 14, page_text);

	// 表格布局
	const int header_h = 42;
	const int row_h = 60;
	const int table_top = y + 44;
	const int table_bottom = table_top + header_h + per_page * row_h;

	// 表头背景
	setfillcolor(RGB(225, 235, 248));
	solidrectangle(x + 1, table_top, x + w - 1, table_top + header_h);

	// 横线：表头上下 + 每行底部
	setlinecolor(RGB(150, 165, 185));
	line(x, table_top, x + w, table_top);
	line(x, table_top + header_h, x + w, table_top + header_h);
	for (int i = 1; i <= per_page; i++)
	{
		int line_y = table_top + header_h + i * row_h;
		line(x, line_y, x + w, line_y);
	}

	// 4 列竖线
	int col_w[4] = { 250, 220, 200, 190 };
	int line_x = x;
	for (int i = 0; i <= 4; i++)
	{
		line(line_x, table_top, line_x, table_bottom);
		if (i < 4)
		{
			line_x += col_w[i];
		}
	}

	// 表头
	settextstyle(18, 0, _T("Microsoft YaHei"), 0, 0, FW_BOLD, false, false, false);
	settextcolor(RGB(24, 34, 52));

	const char* headers[4] = { "订单号", "客户姓名", "联系电话", "订单状态" };
	int header_x = x;
	for (int i = 0; i < 4; i++)
	{
		int tx = header_x + (col_w[i] - textwidth(headers[i])) / 2;
		int ty = table_top + (header_h - textheight(headers[i])) / 2;
		outtextxy(tx, ty, headers[i]);
		header_x += col_w[i];
	}

	// 当前页的 4 条订单
	settextstyle(18, 0, _T("Microsoft YaHei"));
	settextcolor(RGB(24, 34, 52));

	int start = page * per_page;
	for (int i = 0; i < per_page; i++)
	{
		int index = start + i;
		if (index >= orders_count)
		{
			break;
		}

		ORDER_T* order = orders[index];
		if (order == NULL)
		{
			continue;
		}

		int row_y = table_top + header_h + i * row_h;
		int text_y = row_y + (row_h - textheight("A")) / 2;

		outtextxy(x + 10, text_y, order->oder_ID);
		outtextxy(x + col_w[0] + 10, text_y, order->customer_name);
		outtextxy(x + col_w[0] + col_w[1] + 10, text_y, order->customer_phone);
		outtextxy(x + col_w[0] + col_w[1] + col_w[2] + 10,
			text_y, query_status_text(order->order_status));
	}
}

// 订单查询窗口
int order_query_win()
{
	load_all_orders();

	const int winWid = 960;
	const int winHgh = 660;
	const int win_x = (WIN_WIDTH - winWid) / 2;
	const int win_y = (WIN_HEIGHT - winHgh) / 2;

	const COLORREF bkColor = RGB(22, 35, 62);
	const COLORREF btnColor_default = RGB(240, 246, 253);
	const COLORREF btnColor_focus = RGB(64, 158, 255);
	const COLORREF btnTextColor = RGB(24, 34, 52);
	const COLORREF labelColor = RGB(210, 224, 242);
	const COLORREF titleColor = RGB(255, 255, 255);
	const COLORREF editColor = RGB(245, 248, 252);
	const COLORREF editFocusColor = RGB(214, 232, 250);
	const COLORREF editTextColor = RGB(24, 34, 52);

	WINDOW_T query_win = {
		.x = win_x,
		.y = win_y,
		.width = winWid,
		.height = winHgh,
		.count = 0,
		.current = 0,
		.bgColor = bkColor,
		.draw_table = draw_query_frame
	};

	// 订单号输入框
	CONTROL_T order_id_edit = {
		.x = win_x + 160,
		.y = win_y + 90,
		.width = 250,
		.height = 44,
		.type = EDIT,
		.state = 0,
		.defaultColor = editColor,
		.focusColor = editFocusColor,
		.textColor = editTextColor,
		.fontSize = 20
	};

	// 客户名输入框
	CONTROL_T customer_edit = {
		.x = win_x + 600,
		.y = win_y + 90,
		.width = 280,
		.height = 44,
		.type = EDIT,
		.state = 0,
		.defaultColor = editColor,
		.focusColor = editFocusColor,
		.textColor = editTextColor,
		.fontSize = 20
	};

	// 订单状态输入
	CONTROL_T type_edit = {
		.x = win_x + 160,
		.y = win_y + 160,
		.width = 200,
		.height = 44,
		.type = EDIT,
		.state = 0,
		.defaultColor = editColor,
		.focusColor = editFocusColor,
		.textColor = editTextColor,
		.fontSize = 20
	};

	// 查询按钮
	CONTROL_T query_button = {
		.x = win_x + 440,
		.y = win_y + 160,
		.width = 130,
		.height = 44,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor,
		.fontSize = 20
	};
	strcpy_s(query_button.text, sizeof(query_button.text), "查询");

	// 清空输入框
	CONTROL_T clear_button = {
		.x = win_x + 590,
		.y = win_y + 160,
		.width = 140,
		.height = 44,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor,
		.fontSize = 18
	};
	strcpy_s(clear_button.text, sizeof(clear_button.text), "清空输入框");

	// 返回上级
	CONTROL_T back_button = {
		.x = win_x + 750,
		.y = win_y + 160,
		.width = 130,
		.height = 44,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor,
		.fontSize = 20
	};
	strcpy_s(back_button.text, sizeof(back_button.text), "返回上级");

	// 上一页
	CONTROL_T prev_button = {
		.x = win_x + 300,
		.y = win_y + 600,
		.width = 140,
		.height = 44,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor,
		.fontSize = 20
	};
	strcpy_s(prev_button.text, sizeof(prev_button.text), "上一页");

	// 下一页
	CONTROL_T next_button = {
		.x = win_x + 520,
		.y = win_y + 600,
		.width = 140,
		.height = 44,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor,
		.fontSize = 20
	};
	strcpy_s(next_button.text, sizeof(next_button.text), "下一页");

	// 先添加输入框和按钮，保证索引为 0–7
	win_add_ctrl(&query_win, &order_id_edit);  // 0
	win_add_ctrl(&query_win, &customer_edit);  // 1
	win_add_ctrl(&query_win, &type_edit);      // 2
	win_add_ctrl(&query_win, &query_button);   // 3
	win_add_ctrl(&query_win, &clear_button);   // 4
	win_add_ctrl(&query_win, &back_button);    // 5
	win_add_ctrl(&query_win, &prev_button);    // 6
	win_add_ctrl(&query_win, &next_button);    // 7

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
	strcpy_s(title.text, sizeof(title.text), "订单查询界面");

	// 订单号标签
	CONTROL_T order_id_label = {
		.x = win_x + 50,
		.y = win_y + 97,
		.width = 100,
		.height = 30,
		.type = LABEL,
		.state = 0,
		.defaultColor = bkColor,
		.focusColor = bkColor,
		.textColor = labelColor,
		.fontSize = 20
	};
	strcpy_s(order_id_label.text, sizeof(order_id_label.text), "订单号");

	// 客户名标签
	CONTROL_T customer_label = {
		.x = win_x + 470,
		.y = win_y + 97,
		.width = 110,
		.height = 30,
		.type = LABEL,
		.state = 0,
		.defaultColor = bkColor,
		.focusColor = bkColor,
		.textColor = labelColor,
		.fontSize = 20
	};
	strcpy_s(customer_label.text, sizeof(customer_label.text), "客户名");

	// 类型标签
	CONTROL_T type_label = {
		.x = win_x + 50,
		.y = win_y + 167,
		.width = 100,
		.height = 30,
		.type = LABEL,
		.state = 0,
		.defaultColor = bkColor,
		.focusColor = bkColor,
		.textColor = labelColor,
		.fontSize = 20
	};
	strcpy_s(type_label.text, sizeof(type_label.text), "订单状态类型");

	win_add_ctrl(&query_win, &title);           // 8
	win_add_ctrl(&query_win, &order_id_label);  // 9
	win_add_ctrl(&query_win, &customer_label);  // 10
	win_add_ctrl(&query_win, &type_label);      // 11

	while (true)
	{
		query_win = window_run(&query_win);

		// 查询按钮
		if (query_win.current == 3)
		{
			int type = -1;
			bool query_status = false;
			if (strlen(query_win.controls[2].text) == 1)
			{
				type = atoi(query_win.controls[2].text);
			}
			query_status = query_order(query_win.controls[0].text, query_win.controls[1].text, type);
			cur_order = 0;
			queryorder_popup_win(query_status);
		}
		else if (query_win.current == 4)
		{
			// 清空输入框
			for (int i = 0; i < 3; i++)
			{
				query_win.controls[i].text[0] = '\0';
			}
		}
		// 上一页
		else if (query_win.current == 6)
		{
			if (cur_order > 0)
			{
				cur_order--;
			}
		}
		// 下一页
		else if (query_win.current == 7)
		{
			int total_pages = (orders_count + per_page - 1) / per_page;
			if (cur_order < total_pages - 1)
			{
				cur_order++;
			}
		}
		// 返回上级
		else if (query_win.current == 5)
		{
			// 管理员
			if (current->role == 1)
			{
				return 13;
			}
			// 仓储员
			if (current->role == 2)
			{
				return 9;
			}
			// 调度员
			if (current->role == 3)
			{
				return 8;
			}
			// 客服
			if (current->role == 4)
			{
				return 7;
			}
		}
	}
	return 0;
}
