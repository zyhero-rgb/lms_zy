#ifndef _FILE_H
#define _FILE_H
#include <stdio.h>
#include "list.h"

// 文件名
extern const char* file_name_str;
extern const char* order_file_name;

// 打开文件
FILE* file_open(const char* file_name);

// 获取文件大小
long file_getSize(FILE* file);


// 添加新的数据
void file_add(FILE* file, int elem_size, void* data);

/*
    读取文件数据，写入内存
    file:目标文件
    head:目标链表
    size:每次读取的字节数
*/
void file_read_list(FILE* file, LNode* head, int size);

/*
    将链表的内容写入文件
    file:目标文件
    head:目标链表
    size:每次写入的字节数
*/
void file_write_list(FILE* file, LNode* head, int size);

#endif