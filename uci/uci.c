#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libubox/list.h>
#include <libubox/uloop.h>
#include <uci.h>

#define CFG_FILE_PATH "/root/test/test_uci/student"

#define NAME_LEN 20
#define ID_LEN 20

typedef struct student
{
    char name[NAME_LEN];
    char id[ID_LEN];
    struct list_head list;
} Stu;

// 定时器结构体，控制定时打印
struct uloop_timeout g_time_send;

struct list_head student_list;

// 定时器
static void timer_callback(struct uloop_timeout *t)
{
    Stu *sta, *sta_tmp;

    list_for_each_entry_safe(sta, sta_tmp, &student_list, list)
    {
        printf("name: %s id : %s \n", sta->name, sta->id);
    }

    // 重置定时器
    uloop_timeout_set(t, 5000);
}

int load_cfg()
{
    struct uci_context *ctx;
    struct uci_package *pkg;
    struct uci_section *sec;
    const char *name;
    const char *id;
    Stu *p_student;

    ctx = uci_alloc_context();

    // 读取硬盘UCI配置文件，解析加载到内存
    uci_load(ctx, CFG_FILE_PATH, &pkg);

    struct uci_element *ele;

    uci_foreach_element(&pkg->sections, ele)
    {
        // UCI元素转为配置段结构体
        sec = uci_to_section(ele);

        if (strcmp(sec->type, "student") == 0)
        {
            p_student = malloc(sizeof(Stu));

            if (p_student == NULL)
            {
                uci_unload(ctx, pkg);
                uci_free_context(ctx);
                return -1;
            }

            memset(p_student, 0, sizeof(Stu));

            INIT_LIST_HEAD(&p_student->list);

            name = uci_lookup_option_string(ctx, sec, "name");
            if (name != NULL)
            {
                printf("name: %s ", name);
                snprintf(p_student->name, NAME_LEN, "%s", name);
            }

            id = uci_lookup_option_string(ctx, sec, "id");
            if (id != NULL)
            {
                printf("id : %s\n", id);
                snprintf(p_student->id, ID_LEN, "%s", id);
            }

            list_add_tail(&p_student->list, &student_list);
        }
    }

    uci_unload(ctx, pkg);

    uci_free_context(ctx);

    return 0;
}

int main()
{
    INIT_LIST_HEAD(&student_list);

    if (load_cfg() < 0) {
        printf("配置加载失败\n");
        return -1;
    }

    // 初始化uloop事件定时器框架
    uloop_init();

    g_time_send.pending = false;
    g_time_send.cb = timer_callback;

    uloop_timeout_set(&g_time_send, 5000);

    uloop_run();

    Stu *sta, *sta_tmp;
    list_for_each_entry_safe(sta, sta_tmp, &student_list, list)
    {
        list_del(&sta->list);
        free(sta);
        sta = NULL;
    }
    printf("已释放所有学生内存，安全退出！\n");

    return 0;
}



// 1.cmake编译 分模块
// 2.通过uci读取预存IP 端口
// 3.键盘输入获取模块
// 4.网络通信模块
// 5.实现双方对话