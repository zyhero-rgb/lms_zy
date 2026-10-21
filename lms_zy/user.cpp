#include<stdio.h>
#include "user.h"
#include "file.h"
#include <string.h>

// 登录验证
bool login_check(const char* name, const char* pwd)
{
	if (user_list == NULL)
	{
		return false;
	}
	LNode* temp = user_list->next;
	char* _name = NULL;
	char* _pwd = NULL;

	while (temp != NULL)
	{
		USER_T* user = (USER_T*)temp->data;
		if (user == NULL)
		{
			return false;
		}
		_name = user->name;
		_pwd = user->pwd;
		if (strcmp(_name,name) == 0 && strcmp(_pwd,pwd) == 0)
		{
			return true;
		}
		temp = temp->next;
	}
	return false;
}