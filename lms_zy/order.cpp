#include "order.h"
#include "file.h"
#include "list.h"
#include <stdio.h>
#include <string.h>
#include "date.h"
#include <stdlib.h>
#include <mutex>
#include <time.h>

// 货物类型
static const char* goods_type_names[] = {
	"未知",
	"普通货物",
	"易碎品",
	"冷链货物",
	"危险品",
	"贵重物品",
	"大件货物"
};

// 状态
static const char* order_status_names[] = {
	"待审核",
	"已驳回",
	"待出库",	
	"待运输",
	"运输中",
	"已送达",
	"已完成"
};

// 初始化几个订单数据
void order_data_init()
{
	order_list = init_List();
	order_file = file_open(order_file_name);

	if (order_list == NULL || order_file == NULL)
	{
		printf("order data init error !!\n");
		exit(1);
	}
	if (file_getSize(order_file) == 0)
	{
		ORDER_T* order1 = (ORDER_T*)malloc(sizeof(ORDER_T));
		ORDER_T* order2 = (ORDER_T*)malloc(sizeof(ORDER_T));
		if (order1 == NULL || order2 == NULL)
		{
			printf("malloc failed\n");
			exit(1);
		}
		memset(order1, 0, sizeof(ORDER_T));
		memset(order2, 0, sizeof(ORDER_T));
		// 获取日期
		char buffer_ID1[20] = { 0 };
		char buffer_ID2[20] = { 0 };
		DATE_T date = get_time();


		sprintf_s(buffer_ID1, sizeof(buffer_ID1), "WL%04d%02d%02d%06d", date.year, date.month, 
			date.day,getList_len(order_list) + 1);
		strcpy_s(order1->oder_ID, sizeof(order1->oder_ID), buffer_ID1);										// ID
		strcpy_s(order1->origin_address, sizeof(order1->origin_address), "Shanghai");						// 发货地址
		strcpy_s(order1->shipping_address, sizeof(order1->shipping_address), "Beijing");					// 收货地址
		strcpy_s(order1->desired_delivery_time, sizeof(order1->desired_delivery_time), "2026-11-01");		// 期望送达时间
		strcpy_s(order1->customer_name, sizeof(order1->customer_name), "Tom");								// 客户名	
		strcpy_s(order1->customer_phone, sizeof(order1->customer_phone), "13800138000");					// 电话
		order1->order_status = ORDER_PENDING_REVIEW;														// 订单状态
		strcpy_s(order1->goods_name, sizeof(order1->goods_name), "Laptop");									// 货物名称
		order1->goods_type = GOODS_GENERAL;																	// 货物类型
		order1->goods_weight = 2.5f;																		// 货物重量
		order1->goods_count = 10;																			// 数量
		order1->goods_volume = 0.03f;																		// 体积


		sprintf_s(buffer_ID2, sizeof(buffer_ID2), "WL%04d%02d%02d%06d", date.year, date.month,
			date.day, getList_len(order_list) + 2);
		strcpy_s(order2->oder_ID, sizeof(order2->oder_ID), buffer_ID2);										// ID
		strcpy_s(order2->origin_address, sizeof(order2->origin_address), "Sichuan");						// 发货地址
		strcpy_s(order2->shipping_address, sizeof(order2->shipping_address), "Beijing");					// 收货地址
		strcpy_s(order2->desired_delivery_time, sizeof(order2->desired_delivery_time), "2026-11-02");		// 期望送达时间
		strcpy_s(order2->customer_name, sizeof(order2->customer_name), "Max");								// 客户名	
		strcpy_s(order2->customer_phone, sizeof(order2->customer_phone), "13800138220");					// 电话
		order2->order_status = ORDER_PENDING_REVIEW;														// 订单状态
		strcpy_s(order2->goods_name, sizeof(order2->goods_name), "iphone");									// 货物名称
		order2->goods_type = GOODS_GENERAL;																	// 货物类型
		order2->goods_weight = 2.5f;																		// 货物重量
		order2->goods_count = 10;																			// 数量
		order2->goods_volume = 0.03f;																		// 体积


		// 添加到文件中
		file_add(order_file, sizeof(ORDER_T), order1);
		file_add(order_file, sizeof(ORDER_T), order2);
	}
	// 数据读入到链表中
	file_read_list(order_file,order_list,sizeof(ORDER_T));
}

// 打印货物信息
void print_order(void* order)
{
	if (order == NULL)
	{
		printf("order is null\n");
		return;
	}
	ORDER_T* o = (ORDER_T*)order;
	const char* status_text = "未知";
	if (o->order_status >= 0 && o->order_status <= 6)
	{
		status_text = order_status_names[o->order_status];
		printf("%d\n", o->order_status);
	}

	const char* type_text = "未知";
	if (o->goods_type >= 0 && o->goods_type <= 6)
	{
		type_text = goods_type_names[o->goods_type];
	}

	printf("========== Order ==========\n");
	printf("ID          : %s\n", o->oder_ID);
	printf("Status      : %s\n", status_text);
	printf("Customer    : %s\n", o->customer_name);
	printf("Phone       : %s\n", o->customer_phone);
	printf("From        : %s\n", o->origin_address);
	printf("To          : %s\n", o->shipping_address);
	printf("Delivery    : %s\n", o->desired_delivery_time);
	printf("Goods       : %s\n", o->goods_name);
	printf("Type        : %s\n", type_text);
	printf("Weight      : %.2fKG\n", o->goods_weight);
	printf("Count       : %d\n", o->goods_count);
	printf("Volume      : %.3fL\n", o->goods_volume);
	printf("===========================\n");
}

// 判断闰年
static int is_leap_year(int year)
{
	return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

// 某年某月有多少天
static int days_in_month(int year, int month)
{
	int days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	// 闰年二月
	if (month == 2 && is_leap_year(year))
	{
		return 29;
	}
	return days[month - 1];
}

// DATE_T 往后加 days 天
static DATE_T add_days(DATE_T date, int days)
{
	date.day += days;
	// 超过该月份的最大天数
	while (date.day > days_in_month(date.year, date.month))
	{
		date.day -= days_in_month(date.year, date.month);
		date.month++;

		if (date.month > 12)
		{
			date.month = 1;
			date.year++;
		}
	}

	return date;
}

// 创建订单
bool create_new_order(const char* address_from, const char* address_to, const char* cus_name,
	const char* cus_phone, const char* g_name, const int g_type, const float g_weight,
	const int g_count, const float g_volumn)
{
	ORDER_T* order = (ORDER_T*)malloc(sizeof(ORDER_T));
	if (order == NULL)
	{
		printf("malloc failed");
		return false;
	}
	// 获取日期
	char buffer_ID[20] = { 0 };
	char buffer_DDT[20] = { 0 };
	
	// 获取创建订单时的日期
	DATE_T date = get_time();

	sprintf_s(buffer_ID, sizeof(buffer_ID), "WL%04d%02d%02d%06d", date.year, date.month,
		date.day, getList_len(order_list) + 1);

	// 预计送达时间：用订单创建时间往后推 3 天
	DATE_T delivery = add_days(date, 3);

	sprintf_s(buffer_DDT, sizeof(buffer_DDT), "%04d-%02d-%02d",
		delivery.year, delivery.month, delivery.day);

	strcpy_s(order->oder_ID, sizeof(order->oder_ID), buffer_ID);									// ID
	strcpy_s(order->origin_address, sizeof(order->origin_address), address_from);					// 发货地址
	strcpy_s(order->shipping_address, sizeof(order->shipping_address), address_to);					// 收货地址
	strcpy_s(order->desired_delivery_time, sizeof(order->desired_delivery_time), buffer_DDT);		// 期望送达时间
	strcpy_s(order->customer_name, sizeof(order->customer_name), cus_name);							// 客户名	
	strcpy_s(order->customer_phone, sizeof(order->customer_phone), cus_phone);						// 电话
	order->order_status = ORDER_PENDING_REVIEW;														// 订单状态
	strcpy_s(order->goods_name, sizeof(order->goods_name), g_name);									// 货物名称
	order->goods_type = g_type;																		// 货物类型
	order->goods_weight = g_weight;																	// 货物重量
	order->goods_count = g_count;																	// 数量
	order->goods_volume = g_volumn;																	// 体积

	insertNode_Tail(order_list, order);
	file_add(order_file, sizeof(ORDER_T), order);

	printf("create an order successfully\n");
	return true;
}
