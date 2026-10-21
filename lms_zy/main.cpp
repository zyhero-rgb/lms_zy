#include <graphics.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "control.h"
#include "date.h"
#include <thread>
#include "loginwin.h"
#include "startwin.h"
#include "user.h"
#include "list.h"
#include "file.h"

LNode* user_list = NULL;
FILE* user_file = NULL;

typedef int (*window_handler)(void);


int main()
{
	std::thread time_thread(get_time_thread);
	time_thread.detach(); // 脱离主线程

	user_file = file_open("usertext.txt");
	if (user_file == NULL)
	{
		perror("error. user_file is null\n");
		return 1;
	}
	user_list = init_List();

	if (file_getSize(user_file) == 0)
	{
		USER_T user_admin = { "admin","123321",1,1 };
		file_add(user_file, sizeof(USER_T), &user_admin);
	}

	file_read_list(user_file, user_list, sizeof(USER_T));

	initgraph(WIN_WIDTH, WIN_HEIGHT);
	init_bk_image(_T("d:/Users/人擦/Desktop/devc++/项目/images/bk.png"));
	// 函数指针数组
	window_handler wins[] = { startWin,loginWin };

	int cur_win = 0;
	while (true)
	{
		cur_win = wins[cur_win]();
	}

	closegraph();
	return 0;
}
