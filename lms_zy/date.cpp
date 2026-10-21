#include <time.h>
#include "date.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <mutex>

char dateStr[64] = { 0 };
std::mutex time_mtx;

// 获取时间
DATE_T get_time() {

	DATE_T date = { 0 };
	time_t nowTime = time(NULL);
	struct tm localTime; 
	localtime_s(&localTime, &nowTime);   // 取地址传入

	date.year = localTime.tm_year + 1900;
	date.month = localTime.tm_mon + 1;
	date.day = localTime.tm_mday;
	date.hour = localTime.tm_hour;
	date.mins = localTime.tm_min;
	date.scds = localTime.tm_sec;
	return date;
}

// 获取时间字符串
void get_time_thread()
{
	while (true)
	{
		DATE_T time_now = get_time();
		char buffer[64] = { 0 };
		sprintf_s(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d",
			time_now.year, time_now.month, time_now.day,
			time_now.hour, time_now.mins, time_now.scds);
		{
			// 锁只保护这次字符串拷贝，不要把 Sleep 也锁在里面
			std::lock_guard<std::mutex> lock(time_mtx);
			strcpy_s(dateStr, sizeof(dateStr), buffer);
		}
		Sleep(200);
	}
}