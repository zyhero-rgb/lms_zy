#ifndef _LIST_H
#define _LIST_H

// 通用链表节点构体
typedef struct Node
{
    void* data;
    struct Node* next;
} LNode;

// 链表初始化
LNode* init_List();

// 创建节点
LNode* createLNode(void* data);

// 在链表尾部插入新的节点
void insertNode_Tail(LNode* head, void* data);

// 在链表头部插入新的节点
void insertNode_Head(LNode* head, void* data);

// 获取链表长度
int getList_len(LNode* head);

// 在指定位置插入节点
void insertNode_AtPos(LNode* head, void* data, int pos);

// 查找节点
void* findNode(LNode* head, int pos);

// 删除指定节点
void deleteNode(LNode* head, int pos);

// 释放链表的内存
void destroyList(LNode* head);

// 打印链表信息
void printList(LNode* head, void (*print)(void*));

#endif