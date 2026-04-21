#include <libubox/uloop.h>

// 声明外部定时器模块接口
extern void student_timer_init(void);
// 声明链表内存释放函数
extern void free_all(void);

int main()
{
    // 初始化uloop事件框架
    uloop_init();
    // 初始化学生定时器（纯模块调用）
    student_timer_init();
    // 启动事件循环（阻塞运行）
    uloop_run();
    // 程序退出后资源清理
    free_all();
    uloop_done();
    return 0;
}

