#include <stdio.h>
#include <stdlib.h>
#include "list.h"

// 链表初始化
LNode* init_List()
{
    LNode* head = (LNode*)malloc(sizeof(LNode));
    if (head == NULL)
    {
        perror("init list unsuccessully");
        exit(1);
    }
    head->data = NULL;
    head->next = NULL;
    printf("init successful\n");
    return head;
}

// 创建节点
LNode* createLNode(void* data)
{
    LNode* node = (LNode*)malloc(sizeof(LNode));
    if (node == NULL)
    {
        printf("malloc unsuccessful\n");
        exit(1);
    }
    node->data = data;
    node->next = NULL;
    return node;
}

// 在链表头部插入新的节点
void insertNode_Head(LNode* head, void* data)
{
    LNode* newNode = createLNode(data);
    newNode->next = head->next;
    head->next = newNode;
}

// 在链表尾部插入新的节点
void insertNode_Tail(LNode* head, void* data)
{
    // 创建节点
    LNode* newNode = createLNode(data);
    LNode* temp = head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = newNode;
}

// 获取链表长度（节点个数）
int getList_len(LNode* head)
{
    int count = 0;
    LNode* temp = head->next;
    while (temp != NULL)
    {
        count++;
        temp = temp->next;
    }
    return count;
}

// 在指定位置插入节点
void insertNode_AtPos(LNode* head, void* data, int pos)
{
    int len = getList_len(head);
    if (pos <= 0 || pos > len + 1)
    {
        printf("pos unavilable\n");
        return;
    }

    LNode* temp = head;
    LNode* newNode = createLNode(data);
    for (int i = 0; i < pos - 1; i++)
    {
        temp = temp->next;
    }
    newNode->next = temp->next;
    temp->next = newNode;
}

// 释放链表的内存
void destroyList(LNode* head)
{
    if (head == NULL)
    {
        return;
    }
    LNode* temp = head;
    while (temp != NULL)
    {
        LNode* node = temp;
        temp = temp->next;
        free(node->data);
        free(node);
    }
}

// 查找节点
void* findNode(LNode* head, int pos)
{
    int len = getList_len(head);
    if (pos < 1 || pos > len)
    {
        printf("position unavailable\n");
        return NULL;
    }
    LNode* temp = head->next;
    for (int i = 1; i < pos; i++)
    {
        temp = temp->next;
    }
    return temp->data;
}

// 删除指定节点
void deleteNode(LNode* head, int pos)
{
    int len = getList_len(head);
    if (pos <= 0 || pos > len)
    {
        printf("position is unavilable\n");
        return;
    }
    LNode* t = head;
    for (int i = 0; i < pos - 1; i++)
    {
        t = t->next;
    }
    LNode* node = t->next;
    t->next = node->next;
    free(node->data);
    free(node);
    printf("delete successfully\n");
}

// 打印链表信息
void printList(LNode* head, void (*print)(void*))
{
    LNode* temp = head->next;
    while (temp != NULL)
    {
        print(temp->data);
        temp = temp->next;
    }
}