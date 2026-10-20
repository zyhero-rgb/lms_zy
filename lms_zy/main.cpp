#include <graphics.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "control.h"
#include "date.h"
#include <thread>

int main()
{
	std::thread time_thread(get_time_thread);
	time_thread.detach(); // 脱离主线程

	initgraph(WIN_WIDTH, WIN_HEIGHT);
	loadimage(NULL, _T("d:/Users/人擦/Desktop/devc++/项目/images/bk.png"), 1024, 768, true);
	// 先算出窗口左上角坐标
	int win_x = (WIN_WIDTH - 400) / 2;
	int win_y = (WIN_HEIGHT - 300) / 2;

	// 创建3个按钮，坐标 = 窗口起点 + 窗口内部偏移
	CONTROL_T btn_login = create_ctrl(
		win_x + 40,    // 在蓝色窗口内部，离窗口左边缘40
		win_y + 60,    // 离窗口顶部40
		320,
		50,
		BUTTON,
		0,
		"1.登录",
		WHITE,
		RED,
		BLACK
	);

	CONTROL_T btn_forget = create_ctrl(
		win_x + 40,
		win_y + 120,
		320,
		50,
		BUTTON,
		0,
		"2.忘记密码",
		WHITE,
		RED,
		BLACK
	);

	CONTROL_T btn_exit = create_ctrl(
		win_x + 40,
		win_y + 180,
		320,
		50,
		BUTTON,
		0,
		"3.退出",
		WHITE,
		RED,
		BLACK
	);

	// 蓝色窗口
	WINDOW_T test_win = {
		.x = win_x,
		.y = win_y,
		.width = 400,
		.height = 300,
		.count = 0,
		.current = 0,
		.bgColor = BLUE
	};

	win_add_ctrl(&test_win, &btn_login);
	win_add_ctrl(&test_win, &btn_forget);
	win_add_ctrl(&test_win, &btn_exit);

	window_run(&test_win);
	closegraph();

	//int get_key = _getch();
	//if (get_key == 224)
	//{
	//	get_key = _getch(); // 读第二次，拿到方向键真实编码
	//	printf("second time\n");
	//}
	//printf("%d", get_key);
	return 0;
}
