#include <stdio.h>
#include <unistd.h>
#include <libubox/uloop.h>
// 直接声明要调用的外部函数，不需要包含 student.h
extern void uci_load_student(void);
extern void print_by_rank(void);
extern void free_all(void);
// 全局定时器
struct uloop_timeout g_time_send;
// 计数器
static int counter = 0;
// 定时器回调函数
static void timer_callback(struct uloop_timeout *t)
{
    counter++;
    printf("定时器触发了，次数：%d\n", counter);
    // 读取学生配置 + 打印排名
    uci_load_student();
    print_by_rank();
    // 重新设置5秒定时器
    uloop_timeout_set(t, 5000);
}

// 定时器初始化接口（供main.c调用）
void student_timer_init(void)
{
    // 定时器初始化
    g_time_send.pending = false;
    g_time_send.cb = timer_callback;
    // 5秒触发一次
    uloop_timeout_set(&g_time_send, 5000);
}