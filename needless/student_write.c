#include <uci.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

// 完全保留你原版的LOG宏
#define LOG(fmt, ...) \
 printf("%s %d: ", \
 __FUNCTION__, __LINE__); \
 printf(fmt, ##__VA_ARGS__)

int main(int argc, char *argv[])
{
    struct uci_context *ctx;
    struct uci_ptr ptr;
    char *str = strdup(argv[1]);

    // 创建UCI上下文
    ctx = uci_alloc_context();

    // 强制指定配置文件目录：/root/test/uci/（核心：固定你的学生配置路径）
    uci_set_confdir(ctx, "/root/test/uci");

    // 解析配置路径
    uci_lookup_ptr(ctx, &ptr, str, true);

    // 设置新值（适配新版UCI，无编译错误）
    ptr.value = argv[2];

    // 修改配置
    uci_set(ctx, &ptr);

    // 提交保存
    uci_commit(ctx, &ptr.p, false);

    // 打印成功日志
    LOG("配置修改成功！路径：%s 新值：%s\n", argv[1], argv[2]);

    // 释放资源
    uci_free_context(ctx);
    free(str);

    return (0);
}