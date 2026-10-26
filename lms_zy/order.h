#ifndef _ORDER_H
#define _ORDER_H

#include <stdio.h>
#include "list.h"

// 订单状态
#define ORDER_PENDING_REVIEW	1	// 待审核
#define ORDER_REJECTED			2	// 已驳回
#define ORDER_PENDING_OUTBOUND	3	// 待出库
#define ORDER_IN_TRANSIT		4	// 运输中
#define ORDER_DELIVERED			5	// 已送达
#define ORDER_COMPLETED			6	// 已完成

// 货物类型
#define GOODS_GENERAL    1			// 普通货物
#define GOODS_FRAGILE    2			// 易碎品
#define GOODS_COLD       3			// 冷链货物
#define GOODS_HAZARDOUS  4			// 危险品
#define GOODS_VALUABLE   5			// 贵重物品
#define GOODS_BULKY      6			// 大件货物


extern LNode* order_list;			// 订单数据链表
extern FILE* order_file;			// 订单数据文件

// 订单结构体 
typedef struct oredr_t
{
	char oder_ID[20];				// 订单号
	char origin_address[100];		// 发货地址
	char shipping_address[100];		// 收货地址
	char desired_delivery_time[20];	// 期望送达时间

	char customer_name[20];			// 客户姓名
	char customer_phone[13];		// 客户电话

	int order_status;				// 订单状态
	char goods_name[100];			// 货物名称
	int goods_type;					// 类型
	float goods_weight;				// 重量
	int goods_count;				// 数量
	float goods_volumn;				// 体积
	
}ORDER_T;

// 初始化几个订单数据
void order_data_init();

// 打印货物信息
void print_order(void* order);

// 创建订单
//void craete_new_order(const char* address_from,const char* address_to,const char* cus_name,
	//const char*cus_phone,const char*g_name,const int g_type,const float g_weight,const int g_count,const float g_volumn);

#endif