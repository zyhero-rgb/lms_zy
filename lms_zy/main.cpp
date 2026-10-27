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
#include "adminwin.h"
#include "systemmanagewin.h"
#include "createuserwin.h"
#include "passwordresetwin.h"
#include "searchuserwin.h"
#include "customer_service.h"
#include "dispatcher.h"
#include "warehouse_worker.h"
#include "order.h"
#include "auditorderwin.h"


LNode* user_list = NULL;
FILE* user_file = NULL;
USER_T* current = NULL;
LNode* order_list = NULL;
FILE* order_file = NULL;

typedef int (*window_handler)(void);


int main()
{
	std::thread time_thread(get_time_thread);
	time_thread.detach(); // 脱离主线程

	// 启动程序，初始化一个管理员的数据
	user_data_init();
	order_data_init();
	printList(user_list,print_user);
	printList(order_list, print_order);

	initgraph(WIN_WIDTH, WIN_HEIGHT);
	init_bk_image(_T("d:/Users/人擦/Desktop/devc++/项目/images/bk.png"));

	// 函数指针数组		   //   0          1        2            3                 4            5                 6
	window_handler wins[] = { startWin,loginWin,adminwin,system_manage_win ,createUer_win ,password_reset_win ,search_user_win ,
								customer_service_win ,dispatcher_win ,warehouse_worker_win ,audit_order_win };
	                           //       7                    8                 9               10

	int cur_win = 0;
	while (true)
	{
		cur_win = wins[cur_win]();
	}

	// 释放空间 关闭文件
	destroyList(user_list);
	destroyList(order_list);
	fclose(user_file);
	fclose(order_file);
	closegraph();
	return 0;
}
