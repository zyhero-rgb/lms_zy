#include "popupwin.h"
#include "control.h"
#include <graphics.h>
#include <tchar.h>

// 通用弹窗
void popup_show(LPCTSTR title, LPCTSTR message, bool failure)
{
	const int winW = 440;
	const int winH = 200;
	const int winX = (WIN_WIDTH - winW) / 2;
	const int winY = (WIN_HEIGHT - winH) / 2;
	//                             红色                  蓝色
	COLORREF accent = failure ? RGB(220, 70, 75) : RGB(64, 158, 255);
	COLORREF panel = failure ? RGB(48, 26, 30) : RGB(22, 35, 62);
	COLORREF textColor = RGB(235, 240, 250);

	// 弹窗主体
	setlinecolor(accent);
	setlinestyle(PS_SOLID, 2);
	setfillcolor(panel);
	fillroundrect(winX, winY, winX + winW, winY + winH, 18, 18);

	// 顶部主题色装饰线
	setfillcolor(accent);
	solidrectangle(winX + 30, winY + 26, winX + winW - 30, winY + 30);

	// 标题
	setbkmode(TRANSPARENT);
	settextcolor(accent);
	settextstyle(28, 0, _T("Microsoft YaHei"), 0, 0, FW_BOLD, false, false, false);

	int titleWidth = textwidth(title);
	outtextxy(winX + (winW - titleWidth) / 2, winY + 52, title);

	// 提示内容
	settextcolor(textColor);
	settextstyle(20, 0, _T("Microsoft YaHei"));

	int messageWidth = textwidth(message);
	outtextxy(winX + (winW - messageWidth) / 2, winY + 112, message);

	// 提示窗口停留 2 秒后自动关闭
	Sleep(2000);
}

// 登录提示窗口
void login_popup_win(bool status)
{
	if (status == true)
	{
		popup_show(_T("登录成功"),_T("欢迎使用"),!status);
	}
	else
	{
		popup_show(_T("登录失败"), _T("密码或用户名不正确 请重新登录"), status);
	}
	
}

// 创建角色和订单提示弹窗
void create_popup_win(bool status)
{
	if (status)
	{
		popup_show(_T("操作成功"),_T("创建成功"),!status);
	}
	else
	{
		popup_show(_T("操作失败"), _T("创建失败"), status);
	}
}
