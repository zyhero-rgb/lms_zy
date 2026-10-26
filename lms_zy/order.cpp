#include "order.h"
#include "file.h"
#include "list.h"
#include <stdio.h>
#include <string.h>
#include "date.h"
#include <stdlib.h>
#include <mutex>

static const char* goods_type_names[] = {
	"未知",
	"普通货物",
	"易碎品",
	"冷链货物",
	"危险品",
	"贵重物品",
	"大件货物"
};

static const char* order_status_names[] = {
	"未知",
	"待审核",
	"已驳回",
	"待出库",
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
		if (order1 == NULL)
		{
			printf("malloc failed\n");
			exit(1);
		}
		memset(order1, 0, sizeof(ORDER_T));

		// 获取日期
		char buffer_ID[20] = { 0 };
		DATE_T date = get_time();
		sprintf_s(buffer_ID, sizeof(buffer_ID), "WL%04d%02d%02d%06d", date.year, date.month, 
			date.day,getList_len(order_list) + 1);

		strcpy_s(order1->oder_ID, sizeof(order1->oder_ID), buffer_ID);										// ID
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
		order1->goods_volumn = 0.03f;																		// 体积
		// 添加到文件中
		file_add(order_file, sizeof(ORDER_T), order1);
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
	printf("Weight      : %.2f\n", o->goods_weight);
	printf("Count       : %d\n", o->goods_count);
	printf("Volume      : %.3f\n", o->goods_volumn);
	printf("===========================\n");
}

//// 创建订单
//void craete_new_order(const char* address_from, const char* address_to, const char* cus_name,
//	const char* cus_phone, const char* g_name, const int g_type, const float g_weight, const int g_count, const float g_volumn)
//{
//	ORDER_T* order = (ORDER_T*)malloc(sizeof(ORDER_T));
//	if (order == NULL)
//	{
//		printf("malloc failed");
//		exit(1);
//	}
//	// 获取日期
//	char buffer_ID[20] = { 0 };
//	DATE_T date = get_time();
//	sprintf_s(buffer_ID, sizeof(buffer_ID), "WL%04d%02d%02d%06d", date.year, date.month,
//		date.day, getList_len(order_list) + 1);
//
//}