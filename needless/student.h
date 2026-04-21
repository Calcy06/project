#ifndef STUDENT_H
#define STUDENT_H

#include <uci.h>           // 读取UCI配置，自带container_of
#include <libubox/list.h>  // 链表结构+操作（list_head、遍历宏）
#include <stddef.h>        // 提供offsetof（链表依赖的基础宏）

// 学生结构体
struct student{
    struct list_head list;
    char name[20];
    int chinese;
    int math;
    int english;
    int total;
};

// 函数声明
void uci_load_student(void);
void print_by_rank(void);
void free_all(void);

#endif