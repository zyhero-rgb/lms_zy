#include<stdio.h>
#include "user.h"
#include "file.h"
#include <string.h>
#include <stdlib.h>

// 登录验证
bool login_check(const char* name, const char* pwd)
{
	if (user_list == NULL || name == NULL || pwd == NULL)
	{
		return false;
	}
	// user_list的头节点为空，直接取第二个节点的数据域
	LNode* temp = user_list->next;
	char* _name = NULL;
	char* _pwd = NULL;

	while (temp != NULL)
	{
		// 取链表节点的数据域
		USER_T* user = (USER_T*)temp->data;
		if (user == NULL)
		{
			return false;
		}
		_name = user->name;
		_pwd = user->pwd;
		if (strcmp(_name,name) == 0 && strcmp(_pwd,pwd) == 0)
		{
			current = user; // 登录成功获取当前登录的角色
			return true;
		}
		temp = temp->next;
	}
	return false;
}

void user_data_init()
{
	// 初始化链表，打开文件
	user_list = init_List();
	user_file = file_open("usertext.txt");
	if (user_file == NULL)
	{
		perror("file or user_list is NULL\n");
		return;
	}
	// 判断当前文件大小：0，没有初始数据，写入；不为0，直接加载数据到链表
	if (file_getSize(user_file) == 0)
	{
		// 1: 管理员
		USER_T* user_admin = (USER_T*)malloc(sizeof(USER_T));
		if (user_admin == NULL)
		{
			printf("malloc failed\n");
			return;
		}
		memset(user_admin,0,sizeof(USER_T));
		sprintf_s(user_admin->ID,"%d",1001 + getList_len(user_list));
		strcpy_s(user_admin->name,sizeof(user_admin->name),"admin");
		strcpy_s(user_admin->pwd, sizeof(user_admin->pwd), "88888888");
		user_admin->state = 1; 
		user_admin->role = 1;
		file_add(user_file, sizeof(USER_T), user_admin);
	}
	// 读入到链表
	file_read_list(user_file, user_list, sizeof(USER_T));
}

void print_user(void* user)
{
	USER_T* user_ = (USER_T*)user;
	if (user_ == NULL)
	{
		return;
	}
	printf("ID:%s,name:%s,pwd:%s\n",user_->ID,user_->name,user_->pwd);
}

// 创建角色
void create_user(const char* name, const char* pwd)
{
	if (name == NULL || pwd == NULL)
	{
		perror("name or pwd is unavliable\n");
		return;
	}
	USER_T* newUser = (USER_T*)malloc(sizeof(USER_T));
	if (newUser == NULL)
	{
		perror("malloc failed\n");
		exit(1);
	}
	sprintf_s(newUser->ID, sizeof(newUser->ID), "%d", 1001 + getList_len(user_list));
	strcpy_s(newUser->name, sizeof(newUser->name), name);
	strcpy_s(newUser->pwd, sizeof(newUser->pwd), pwd);
	newUser->state = 1;
	newUser->role = 1;
}