#ifndef _DATE_H
#define _DATE_H

#include <mutex>

typedef struct date_t
{
	// 年月日-时分秒
	int year;
	int month;
	int day;
	int hour;
	int mins;
	int scds;
}DATE_T;

extern char dateStr[64];

extern std::mutex time_mtx;

// 获取时间
DATE_T get_time();

// 获取时间字符串
void get_time_thread();

#endif