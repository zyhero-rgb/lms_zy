#ifndef _USER_H
#define _USER_H

// 用户账户和密码的长度
#define USER_NAME_LEN 30
#define USER_PWD_LEN 16
#include <stdio.h>
#include "list.h"

// 用户结构体
typedef struct user_t
{
	char name[USER_NAME_LEN + 1];
	char pwd[USER_PWD_LEN + 1];
	int state;
	int role;
}USER_T;

extern LNode* user_list; // 存储用户数据
extern FILE* user_file;	 // 数据持久化

// 登录验证
bool login_check(const char* name,const char* pwd);


#endif
