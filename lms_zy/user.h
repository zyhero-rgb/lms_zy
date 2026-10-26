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
	char ID[20];					// ID自动生成
	char name[USER_NAME_LEN + 1];	// 用户名
	char pwd[USER_PWD_LEN + 1];		// 密码
	int state;						// 是否是可用状态
	int role;						// 角色
}USER_T;

extern LNode* user_list; // 存储用户数据
extern FILE* user_file;	 // 数据持久化
extern USER_T* current;  // 当前登录的角色


// 登录验证
bool login_check(const char* name,const char* pwd);

// 角色信息初始化
void user_data_init();

// 打印输出
void print_user(void *user);

// 创建角色
void create_user(const char *name,const char*pwd,const int role);

// 重置密码
void resert_pwd(const char* name);

#endif
