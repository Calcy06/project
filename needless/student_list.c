#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"student.h"



// 全局链表头
LIST_HEAD(student_list);

// 添加学生
void add_student(const char* name,int chinese,int math,int english)
{
    struct student* s = malloc(sizeof(struct student));
    if(!s){
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    strcpy(s->name,name);
    s->chinese = chinese;
    s->math = math;
    s->english = english;
    s->total = chinese + math + english;

    INIT_LIST_HEAD(&s->list);
    list_add_tail(&s->list,&student_list);
}

// 自定义功能函数：按总分排名打印（修复对齐+漏写总分问题）
void print_by_rank(void)
{
    struct student *p, *q;
    int flag;
    int rank = 1;

    // 空链表判断
    if(list_empty(&student_list))
    {
        printf("暂无学生信息!\n");
        return;
    }

    // 修复链表冒泡排序逻辑（嵌入式安全交换）
    do
    {
        flag = 0;
        struct list_head *i, *j;
        // 遍历链表节点（用链表头遍历，避免entry遍历修改异常）
        for (i = student_list.next; i != &student_list && i->next != &student_list; i = i->next)
        {
            j = i->next;
            p = list_entry(i, struct student, list);
            q = list_entry(j, struct student, list);

            if (p->total < q->total)
            {
                // 安全交换两个节点（嵌入式标准写法）
                list_del(i);
                list_add(i, j);
                flag = 1;
                // 交换后i指向j，避免跳过节点
                i = j;
            }
        }
    } while(flag);

    // 直接遍历链表打印排名（修复漏写总分参数）
    printf("\n===== 定时排序打印 =====\n");
    printf("%-3s %-12s %-6s %-6s %-6s %-6s\n","名次","姓名","语文","数学","英语","总分");
    printf("----------------------------------------\n");
    list_for_each_entry(p, &student_list, list)
    {
        // 修复：最后一个参数补p->total（格式匹配）
        printf("%-3d %-12s %-6d %-6d %-6d %-6d\n",rank++,p->name,p->chinese,p->math,p->english,p->total);
    }
    printf("=====================================\n\n");
}

// 释放内存
void free_all(void)
{
    struct student* pos,* n;
    list_for_each_entry_safe(pos,n,&student_list,list){
        list_del(&pos->list);
        free(pos);
    }
}

//UCI读取配置
void uci_load_student(void)
{
    // 清空旧数据
    free_all();

    struct uci_context *ctx = uci_alloc_context();
    struct uci_package *pkg = NULL;
    uci_set_confdir(ctx, "/root/test/uci");

    if(uci_load(ctx, "student", &pkg) != UCI_OK){
        uci_free_context(ctx);
        return;
    }

    // 遍历配置
    struct uci_element *ele;
    struct uci_section *sec;
    uci_foreach_element(&pkg->sections, ele){
        sec = uci_to_section(ele);
        if(strcmp(sec->type, "student") != 0) continue;

        const char *name = uci_lookup_option_string(ctx, sec, "name");
        int chinese = atoi(uci_lookup_option_string(ctx, sec, "chinese"));
        int math = atoi(uci_lookup_option_string(ctx, sec, "math"));
        int english = atoi(uci_lookup_option_string(ctx, sec, "english"));

        add_student(name, chinese, math, english);
    }

    uci_unload(ctx, pkg);
    uci_free_context(ctx);
}

// 查询学生
struct student* find_student(const char* name)
{
    struct student* pos;
    list_for_each_entry(pos,&student_list,list){
        if(strcmp(pos->name,name)==0) return pos;
    }
    return NULL;
}

// 修改学生
void modify_student(const char* name,int chinese,int math,int english)
{
    struct student* s=find_student(name);
    if(s){
        s->chinese = chinese;
        s->math = math;
        s->english = english;
        s->total = chinese + math + english;
    }
}

// 删除学生
void delete_student(const char* name)
{
    struct student* pos,* n;
    list_for_each_entry_safe(pos,n,&student_list,list){
        if(strcmp(pos->name,name)==0){
            list_del(&pos->list);
            free(pos);
            return;
        }
    }
}