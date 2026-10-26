#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "file.h"
#include "list.h"
#include "user.h"

// 存储用户数据和订单数据的文件名
const char* file_name_str = "usertext.txt";
const char* order_file_name = "ordertext.txt";

/*
    函数名：file_open
    函数功能：打开文件，若文件不存在，则创建文件并打开，否则正常打开
    参数：file_name 文件名
    返回值：FILE*
*/
FILE* file_open(const char* file_name)
{
    FILE* fp = NULL;
    // 若文件不存在，fp为NULL,且不会自动创建文件(r+)
    fopen_s(&fp,file_name,"rb+");
    if (fp == NULL)
    {
        fopen_s(&fp, file_name, "wb+");
        // 创建，再次判断是否打开
        if (fp == NULL)
        {
            perror("fail to open file");
        }
    }
    return fp;
}

/*
    函数名：file_getSize
    函数功能：获取文件大小
    参数：file 文件指针
    返回值：long，失败返回 -1
*/
long file_getSize(FILE* file)
{
    if (file == NULL)
    {
        perror("fail to open file");
        return -1;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file); // 指针移回开头，方便后续读写
    return size;
}

// 文件添加内容
void file_add(FILE* file, int elem_size, void* data)
{
    fseek(file, 0, SEEK_END); // 在文件末尾添加
    size_t n = fwrite(data, elem_size, 1, file);
    fflush(file);
    if (n != 1)
    {
        perror("Failed to write file");
    }
}

/*
    读取文件数据，写入内存
    file:目标文件
    head:目标链表
    size:每次读取的字节数
*/
void file_read_list(FILE* file, LNode* head, int size)
{
    if (file == nullptr || head == nullptr || size <= 0)
    {
        return;
    }
    rewind(file);

    while (true)
    {
        void* data = malloc((size_t)size);
        if (data == nullptr)
        {
            return;
        }
        size_t n = fread_s(data, (size_t)size, (size_t)size, 1, file);
        if (n != 1)
        {
            free(data);
            break;
        }
        insertNode_Tail(head, data);
    }
}

/*
    将链表的内容写入文件
    file:目标文件
    head:目标链表
*/
void file_write_list(FILE* file, LNode* head, int size)
{
    if (file == NULL || size <= 0)
    {
        return;
    }
    LNode* node = head->next;
    while (node != NULL)
    {
        fwrite(node->data, size, 1, file);
        node = node->next;
    }
}
