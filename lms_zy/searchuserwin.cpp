#include "searchuserwin.h"
#include "control.h"
#include "user.h"
#include <stdio.h>
#include <string.h>

// 当前查询结果和分页状态
static USER_T* g_rows[200];   // 查询结果
static int g_count = 0;       // 用户总数
static int g_page = 0;        // 当前页，从 0 开始
static const int PAGE_SIZE = 5; // 每页显示的角色数据的个数

// 角色文字
static const char* role_text(int role)
{
	switch (role)
	{
	case 1: return "管理员";
	case 2: return "仓储员";
	case 3: return "调度员";
	case 4: return "客服";
	default: return "未知";
	}
}

// 状态文字
static const char* state_text(int state)
{
	return state == 1 ? "可用" : "不可用";
}

// 绘制用户数据表格
void draw_user_table(const WINDOW_T* win)
{
	const int table_x = win->x + 50;
	const int table_y = win->y + 190;

	// 表格的宽高
	const int table_width = 720;
	const int table_height = 300;

	const int row_height = 50;      // 行高
	const int column_width = 180;   // 列宽

	const int rows = 6;             // 表头 + 5 行数据
	const int columns = 4;			// 四行数据

	// 表格底色
	setfillcolor(RGB(245, 248, 252));
	solidrectangle(table_x, table_y,
		table_x + table_width, table_y + table_height);

	// 画网格线
	setlinecolor(RGB(120, 140, 170));
	setlinestyle(PS_SOLID, 3);

	// 5 条竖线
	for (int i = 0; i <= columns; i++)
	{
		line(table_x + i * column_width, table_y,
			table_x + i * column_width, table_y + table_height);
	}

	// 7 条横线
	for (int j = 0; j <= rows; j++)
	{
		line(table_x, table_y + j * row_height,
			table_x + table_width, table_y + j * row_height);
	}

	// 输出表头
	setbkmode(TRANSPARENT);
	settextcolor(RGB(24, 34, 52));
	settextstyle(20, 0, _T("Microsoft YaHei"));

	const char* headers[4] = { "用户ID", "用户名", "角色", "状态" };
	for (int c = 0; c < columns; c++)
	{
		// 表头坐标
		int x = table_x + c * column_width + 12;
		int y = table_y + (row_height - textheight(headers[c])) / 2;
		outtextxy(x, y, headers[c]);
	}

	// 当前页的数据
	settextstyle(24, 0, _T("Microsoft YaHei"));
	int start = g_page * PAGE_SIZE;
	for (int i = 0; i < PAGE_SIZE; i++)
	{
		int index = start + i;
		if (index >= g_count)
		{
			break;
		}

		USER_T* u = g_rows[index];
		if (u == NULL)
		{
			continue;
		}

		int y = table_y + (i + 1) * row_height+ (row_height - textheight("A")) / 2;

		outtextxy(table_x + 12, y, u->ID);
		outtextxy(table_x + column_width + 12, y, u->name);
		outtextxy(table_x + column_width * 2 + 12, y, role_text(u->role));
		outtextxy(table_x + column_width * 3 + 12, y, state_text(u->state));
	}

	// 没有数据
	if (g_count == 0)
	{
		const char* empty = "没有找到用户";
		int x = table_x + (table_width - textwidth(empty)) / 2;
		int y = table_y + (table_height - textheight(empty)) / 2;
		outtextxy(x, y, empty);
	}
}

// 用户查询界面
int search_user_win()
{
	g_count = 0;
	g_page = 0;
	// 数据加载
	LNode* temp = user_list->next;
	while (temp != NULL)
	{
		g_rows[g_count] = (USER_T*)temp->data;
		g_count++;
		temp = temp->next;
		if (g_count >= 200)
		{
			break;
		}
	}

	// 窗口大小和坐标
	const int winWid = 820;
	const int winHgh = 640;
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

	WINDOW_T query_win = {
		.x = win_x,
		.y = win_y,
		.width = winWid,
		.height = winHgh,
		.count = 0,
		.current = 0,
		.bgColor = bkColor,
		.draw_table = draw_user_table
	};

	// 用户名搜索框
	CONTROL_T search_edit = {
		.x = win_x + 180,
		.y = win_y + 137,
		.width = 400,
		.height = 44,
		.type = EDIT,
		.state = 0,
		.defaultColor = editColor,
		.focusColor = editFocusColor,
		.textColor = editTextColor,
		.fontSize = 22
	};

	// 查询按钮
	CONTROL_T query_button = {
		.x = win_x + 600,
		.y = win_y + 137,
		.width = 150,
		.height = 44,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor,
		.fontSize = 22
	};
	strcpy_s(query_button.text, sizeof(query_button.text), "查询");

	// 上一页
	CONTROL_T prev_button = {
		.x = win_x + 50,
		.y = win_y + 540,
		.width = 120,
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
		.x = win_x + 190,
		.y = win_y + 540,
		.width = 120,
		.height = 44,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor,
		.fontSize = 20
	};
	strcpy_s(next_button.text, sizeof(next_button.text), "下一页");

	// 返回上级
	CONTROL_T back_button = {
		.x = win_x + 620,
		.y = win_y + 540,
		.width = 150,
		.height = 44,
		.type = BUTTON,
		.state = 0,
		.defaultColor = btnColor_default,
		.focusColor = btnColor_focus,
		.textColor = btnTextColor,
		.fontSize = 20
	};
	strcpy_s(back_button.text, sizeof(back_button.text), "返回上级");

	win_add_ctrl(&query_win, &search_edit);   // 0
	win_add_ctrl(&query_win, &query_button);  // 1
	win_add_ctrl(&query_win, &prev_button);   // 2
	win_add_ctrl(&query_win, &next_button);   // 3
	win_add_ctrl(&query_win, &back_button);   // 4

	// 标题
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
	strcpy_s(title.text, sizeof(title.text), "智能物流管理系统用户查询界面");

	// 当前用户和角色
	const char* roleName = "未知";
	if (current != NULL)
	{
		switch (current->role)
		{
		case 1: roleName = "管理员"; break;
		case 2: roleName = "仓储员"; break;
		case 3: roleName = "调度员"; break;
		case 4: roleName = "客服"; break;
		default: roleName = "未知"; break;
		}
	}

	CONTROL_T current_label = {
		.x = win_x + 50,
		.y = win_y + 90,
		.width = 380,
		.height = 30,
		.type = LABEL,
		.state = 0,
		.defaultColor = bkColor,
		.focusColor = bkColor,
		.textColor = labelColor,
		.fontSize = 20
	};
	sprintf_s(current_label.text, sizeof(current_label.text),
		"当前用户：%s（%s）",current != NULL ? current->name : "未登录",roleName);

	// 登录时间（先留占位，后续可接真实登录时间）
	CONTROL_T login_label = {
		.x = win_x + 440,
		.y = win_y + 90,
		.width = 330,
		.height = 30,
		.type = LABEL,
		.state = 0,
		.defaultColor = bkColor,
		.focusColor = bkColor,
		.textColor = labelColor,
		.fontSize = 18
	};
	strcpy_s(login_label.text, sizeof(login_label.text), "登录时间：--:--:--");

	// 搜索标签
	CONTROL_T search_label = {
		.x = win_x + 50,
		.y = win_y + 145,
		.width = 130,
		.height = 30,
		.type = LABEL,
		.state = 0,
		.defaultColor = bkColor,
		.focusColor = bkColor,
		.textColor = labelColor,
		.fontSize = 20
	};
	strcpy_s(search_label.text, sizeof(search_label.text), "用户名搜索：");

	// 页码提示
	CONTROL_T page_label = {
		.x = win_x + 330,
		.y = win_y + 548,
		.width = 250,
		.height = 28,
		.type = LABEL,
		.state = 0,
		.defaultColor = bkColor,
		.focusColor = bkColor,
		.textColor = statusColor,
		.fontSize = 18
	};
	strcpy_s(page_label.text, sizeof(page_label.text), "共 0 页 当前第 1 页");

	// 底部状态提示
	CONTROL_T status = {
		.x = win_x + 50,
		.y = win_y + 600,
		.width = winWid - 100,
		.height = 24,
		.type = LABEL,
		.state = 0,
		.defaultColor = bkColor,
		.focusColor = bkColor,
		.textColor = statusColor,
		.fontSize = 16
	};
	strcpy_s(status.text, sizeof(status.text), "请输入用户名后点击查询");

	// 标签最后添加，不影响输入框和按钮索引
	win_add_ctrl(&query_win, &title);         // 5
	win_add_ctrl(&query_win, &current_label); // 6
	win_add_ctrl(&query_win, &login_label);   // 7
	win_add_ctrl(&query_win, &search_label);  // 8
	win_add_ctrl(&query_win, &page_label);    // 9
	win_add_ctrl(&query_win, &status);        // 10

	query_win = window_run(&query_win);
	// 查询按钮
	if (query_win.current == 1)
	{
		return 0;
	}
	return 0;
}
