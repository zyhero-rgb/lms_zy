#include "auditorderwin.h"
#include "control.h"
#include "order.h"
#include <stdio.h>
#include <string.h>
#include "file.h"

static ORDER_T* pending_orders[200];	// 所有待审核订单
static int all_orders = 0;			// 所有待审核的订单总数
static int cur_order = 0;			// 当前待审核订单

// 加载待审核的订单
static void load_pending_orders()
{
	if (order_list == NULL)
	{
		return;
	}
	LNode* temp = order_list->next;
	while (temp != NULL)
	{
		ORDER_T* order = (ORDER_T*)temp->data;
		if (order != NULL && order->order_status == ORDER_PENDING_REVIEW)
		{
			pending_orders[all_orders] = order;
			all_orders++;
		}
		temp = temp->next;
	}
}
// 审核
static void audit_order(ORDER_T* order,bool status,const char* reason)
{
	if (order == NULL || order->order_status != ORDER_PENDING_REVIEW)
	{
		printf("order not exists\n");
		return;
	}
	if (status)
	{
		order->order_status = ORDER_PENDING_OUTBOUND; //确认订单，订单状态改变为待出库
		printf("%s\n", order->order_status == ORDER_PENDING_OUTBOUND ? "dai chu ku" : "dai shen he");
	}
	else
	{
		order->order_status = ORDER_REJECTED; // 驳回
		strcpy_s(order->rejected_reason, sizeof(order->rejected_reason), reason); // 填写理由
	}
	print_order(order);
	// 修改文件保存的内容
	if (order_file != NULL)
	{
		fclose(order_file);
		order_file = NULL;
	}
	fopen_s(&order_file, order_file_name, "wb+");
	if (order_file != NULL)
	{
		file_write_list(order_file, order_list, sizeof(ORDER_T));
	}
	else
	{
		printf("failed to open file\n");
	}
}

// 订单状态文字
static const char* audit_status_text(int status)
{
	switch (status)
	{
		case ORDER_PENDING_REVIEW:   return "待审核";
		case ORDER_REJECTED:         return "已驳回";
		case ORDER_PENDING_OUTBOUND: return "待出库";
		case ORDER_PENDING_TRANSPORT:return "待运输";
		case ORDER_IN_TRANSIT:       return "运输中";
		case ORDER_DELIVERED:        return "已送达";
		case ORDER_COMPLETED:        return "已完成";
		default:                     return "未知";
	}
}

// 货物类型文字
static const char* audit_goods_text(int type)
{
	switch (type)
	{
		case GOODS_GENERAL:   return "普通货物";
		case GOODS_FRAGILE:   return "易碎品";
		case GOODS_COLD:      return "冷链货物";
		case GOODS_HAZARDOUS: return "危险品";
		case GOODS_VALUABLE:  return "贵重物品";
		case GOODS_BULKY:     return "大件货物";
		default:              return "未知";
	}
}

// 绘制订单信息面板
static void draw_audit_info(const WINDOW_T* win)
{
	const int x = win->x + 40;
	const int y = win->y + 160;
	const int w = 780;
	const int h = 370;

	// 面板
	setfillcolor(RGB(245, 248, 252));
	solidrectangle(x, y, x + w, y + h);
	setlinecolor(RGB(150, 165, 185));
	setlinestyle(PS_SOLID, 1);
	rectangle(x, y, x + w, y + h);

	// 面板标题
	setbkmode(TRANSPARENT);
	settextcolor(RGB(24, 34, 52));
	settextstyle(22, 0, _T("Microsoft YaHei"), 0, 0, FW_BOLD, false, false, false);
	outtextxy(x + 20, y + 14, _T("订单信息"));

	ORDER_T* order = pending_orders[cur_order];
	if (order == NULL)
	{
		const char* empty = "暂无待审核订单";
		settextstyle(22, 0, _T("Microsoft YaHei"));
		settextcolor(RGB(100, 115, 135));
		int ex = x + (w - textwidth(empty)) / 2;
		int ey = y + (h - textheight(empty)) / 2;
		outtextxy(ex, ey, empty);
		return;
	}

	char line[256] = { 0 };
	settextstyle(22, 0, _T("Microsoft YaHei"));
	settextcolor(RGB(24, 34, 52));

	int left = x + 20;
	int right = x + 400;
	int row = y + 52;

	// 第一行：订单号 / 状态
	sprintf_s(line, sizeof(line), "订单号：%s", order->oder_ID);
	outtextxy(left, row, line);
	sprintf_s(line, sizeof(line), "状态：%s", audit_status_text(order->order_status));
	outtextxy(right, row, line);

	// 第二行：客户 / 电话
	row += 38;
	sprintf_s(line, sizeof(line), "客户：%s", order->customer_name);
	outtextxy(left, row, line);
	sprintf_s(line, sizeof(line), "电话：%s", order->customer_phone);
	outtextxy(right, row, line);

	// 第三行：发货地址
	row += 38;
	sprintf_s(line, sizeof(line), "发货地址：%s", order->origin_address);
	outtextxy(left, row, line);

	// 第四行：收货地址
	row += 38;
	sprintf_s(line, sizeof(line), "收货地址：%s", order->shipping_address);
	outtextxy(left, row, line);

	// 第五行：期望送达 / 货物
	row += 38;
	sprintf_s(line, sizeof(line), "期望送达：%s", order->desired_delivery_time);
	outtextxy(left, row, line);
	sprintf_s(line, sizeof(line), "货物：%s", order->goods_name);
	outtextxy(right, row, line);

	// 第六行：类型 / 重量
	row += 38;
	sprintf_s(line, sizeof(line), "类型：%s", audit_goods_text(order->goods_type));
	outtextxy(left, row, line);
	sprintf_s(line, sizeof(line), "重量：%.2fKG", order->goods_weight);
	outtextxy(right, row, line);

	// 第七行：数量 / 体积
	row += 38;
	sprintf_s(line, sizeof(line), "数量：%d", order->goods_count);
	outtextxy(left, row, line);
	sprintf_s(line, sizeof(line), "体积：%.3fL", order->goods_volumn);
	outtextxy(right, row, line);
}

// 订单审核界面
int audit_order_win()
{
	all_orders = 0;
	cur_order = 0;
	load_pending_orders();
	const int winWid = 860;
	const int winHgh = 650;
	const int win_x = (WIN_WIDTH - winWid) / 2;
	const int win_y = (WIN_HEIGHT - winHgh) / 2;

	const COLORREF bkColor = RGB(22, 35, 62);
	const COLORREF btnColor_default = RGB(240, 246, 253);
	const COLORREF btnColor_focus = RGB(64, 158, 255);
	const COLORREF btnTextColor = RGB(24, 34, 52);
	const COLORREF labelColor = RGB(210, 224, 242);
	const COLORREF titleColor = RGB(255, 255, 255);
	const COLORREF statusColor = RGB(120, 200, 255);
	const COLORREF editColor = RGB(245, 248, 252);
	const COLORREF editFocusColor = RGB(214, 232, 250);
	const COLORREF editTextColor = RGB(24, 34, 52);

	WINDOW_T audit_win = {
		.x = win_x,
		.y = win_y,
		.width = winWid,
		.height = winHgh,
		.count = 0,
		.current = 0,
		.bgColor = bkColor,
		.draw_table = draw_audit_info
	};

	// 审核通过
	CONTROL_T approve_button = {
		.x = win_x + 40,
		.y = win_y + 88,
		.width = 150,
		.height = 48,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor,
		.fontSize = 22
	};
	strcpy_s(approve_button.text, sizeof(approve_button.text), "审核通过");

	// 确认驳回
	CONTROL_T reject_button = {
		.x = win_x + 210,
		.y = win_y + 88,
		.width = 150,
		.height = 48,
		.type = BUTTON,
		.state = 0,
		.defaultColor = RGB(84, 27, 29),
		.focusColor = RGB(212, 67, 72),
		.textColor = RGB(255, 255, 255),
		.fontSize = 22
	};
	strcpy_s(reject_button.text, sizeof(reject_button.text), "确认驳回");

	// 驳回原因输入
	CONTROL_T reason_edit = {
		.x = win_x + 500,
		.y = win_y + 88,
		.width = 300,
		.height = 48,
		.type = EDIT,
		.state = 0,
		.defaultColor = editColor,
		.focusColor = editFocusColor,
		.textColor = editTextColor,
		.fontSize = 20
	};

	// 上一条
	CONTROL_T prev_button = {
		.x = win_x + 40,
		.y = win_y + 560,
		.width = 140,
		.height = 44,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor,
		.fontSize = 20
	};
	strcpy_s(prev_button.text, sizeof(prev_button.text), "上一条");

	// 下一条
	CONTROL_T next_button = {
		.x = win_x + 200,
		.y = win_y + 560,
		.width = 140,
		.height = 44,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor,
		.fontSize = 20
	};
	strcpy_s(next_button.text, sizeof(next_button.text), "下一条");

	// 返回
	CONTROL_T back_button = {
		.x = win_x + 680,
		.y = win_y + 560,
		.width = 140,
		.height = 44,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor,
		.fontSize = 20
	};
	strcpy_s(back_button.text, sizeof(back_button.text), "返回");

	// 先添加按钮和输入框，保证索引为 0–5 
	win_add_ctrl(&audit_win, &approve_button); // 0 通过
	win_add_ctrl(&audit_win, &reject_button);  // 1	驳回
	win_add_ctrl(&audit_win, &reason_edit);    // 2	输入驳回理由
	win_add_ctrl(&audit_win, &prev_button);    // 3	上一个
	win_add_ctrl(&audit_win, &next_button);    // 4 下一个
	win_add_ctrl(&audit_win, &back_button);    // 5 返回上级

	// 标题
	CONTROL_T title = {
		.x = win_x,
		.y = win_y + 24,
		.width = winWid,
		.height = 48,
		.type = LABEL,
		.state = 0,
		.defaultColor = bkColor,
		.focusColor = bkColor,
		.textColor = titleColor,
		.fontSize = 32
	};
	strcpy_s(title.text, sizeof(title.text), "订单审核");

	// 驳回原因标签
	CONTROL_T reason_label = {
		.x = win_x + 390,
		.y = win_y + 98,
		.width = 100,
		.height = 28,
		.type = LABEL,
		.state = 0,
		.defaultColor = bkColor,
		.focusColor = bkColor,
		.textColor = labelColor,
		.fontSize = 18
	};
	strcpy_s(reason_label.text, sizeof(reason_label.text), "驳回原因");

	// 页码提示
	CONTROL_T page_label = {
		.x = win_x + 370,
		.y = win_y + 568,
		.width = 260,
		.height = 28,
		.type = LABEL,
		.state = 0,
		.defaultColor = bkColor,
		.focusColor = bkColor,
		.textColor = statusColor,
		.fontSize = 18
	};
	strcpy_s(page_label.text, sizeof(page_label.text), "第 1 / 1 条");

	win_add_ctrl(&audit_win, &title);        // 6
	win_add_ctrl(&audit_win, &reason_label); // 7
	win_add_ctrl(&audit_win, &page_label);   // 8

	

	while (true)
	{
		audit_win = window_run(&audit_win);

		// 上一个
		if (audit_win.current == 3)
		{
			if (cur_order >= 1)
			{
				cur_order--;
			}
			else
			{
				cur_order = 0;
			}
		}// 下一个
		else if (audit_win.current == 4)
		{
			if (cur_order < all_orders - 1)
			{
				cur_order++;
			}
			else
			{
				cur_order = 0;
			}
		}
		// 通过
		else if (audit_win.current == 0 )
		{
			audit_order(pending_orders[cur_order], true,"");
		}
		// 驳回
		else if (audit_win.current == 1)
		{
			if (strlen(audit_win.controls[2].text) != 0)
			{
				audit_order(pending_orders[cur_order], false, audit_win.controls[2].text);
			}
			else
			{
				// 警告
				printf("请输入驳回订单的理由\n");
			}
		}// 返回上级
		else if (audit_win.current == 5)
		{
			return 2;
		}
	}

	return 0;
}
