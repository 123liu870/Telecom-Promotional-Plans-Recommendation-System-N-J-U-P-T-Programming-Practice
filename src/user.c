#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "../inc/user.h"

static const char *USER_FILE = "users.dat";

int load_users(User users[], int *count)
{
    FILE *fp = fopen(USER_FILE, "rb");
    if (!fp)
    {
        /* 如果不存在，创建一个默认用户用于测试 */
        *count = 1;
        users[0].id = 1;
        strcpy(users[0].username, "user1");
        strcpy(users[0].password, "pass1");
        strcpy(users[0].name, "普通用户");
        strcpy(users[0].phone, "13800000000");
        save_users(users, *count);
        return 0;
    }
    if (fread(count, sizeof(int), 1, fp) != 1)
    {
        fclose(fp);
        *count = 0;
        return 0;
    }
    if (*count > MAX_USERS) *count = MAX_USERS;
    fread(users, sizeof(User), *count, fp);
    fclose(fp);
    return 1;
}

int save_users(User users[], int count)
{
    FILE *fp = fopen(USER_FILE, "wb");
    if (!fp) return 0;
    fwrite(&count, sizeof(int), 1, fp);
    fwrite(users, sizeof(User), count, fp);
    fclose(fp);
    return 1;
}

/* 与管理员模块使用的遮掩输入类似 */
void read_password_local(char *buf, int buflen)
{
    int idx = 0;
    char ch;
    while (1)
    {
        ch = getch();
        if (ch == '\r' || ch == '\n') { putchar('\n'); break; }
        if (ch == '\b') { if (idx>0) { idx--; putchar('\b'); putchar(' '); putchar('\b'); } }
        else if (idx < buflen -1) { buf[idx++] = ch; putchar('*'); }
    }
    buf[idx] = '\0';
}

/* 成功时返回用户索引，失败返回 -1 */
int user_login(User users[], int count)
{
    char uname[32];
    char pwd[32];
    printf("用户名: ");
    if (scanf("%31s", uname) != 1) return -1;
    getchar();
    printf("密码: ");
    read_password_local(pwd, sizeof(pwd));
    for (int i = 0; i < count; i++)
    {
        if (strcmp(users[i].username, uname) == 0 && strcmp(users[i].password, pwd) == 0)
            return i;
    }
    printf("用户名或密码错误\n");
    return -1;
}

void user_menu(User *user)
{
    while (1)
    {
        printf("\n--- 用户菜单 (用户: %s) ---\n", user->username);
        printf("1. 查看信息\n2. 修改姓名\n3. 修改电话\n0. 退出\n选择: ");
        int cmd;
        if (scanf("%d", &cmd) != 1) { getchar(); continue; }
        getchar();
        if (cmd == 0) break;
        if (cmd == 1)
        {
            printf("ID:%d 用户名:%s 姓名:%s 电话:%s\n", user->id, user->username, user->name, user->phone);
        }
        else if (cmd == 2)
        {
            printf("输入新姓名: ");
            fgets(user->name, sizeof(user->name), stdin);
            user->name[strcspn(user->name, "\n")] = '\0';
            printf("已修改。\n");
        }
        else if (cmd == 3)
        {
            printf("输入新电话: ");
            fgets(user->phone, sizeof(user->phone), stdin);
            user->phone[strcspn(user->phone, "\n")] = '\0';
            printf("已修改。\n");
        }
        else printf("无效选项。\n");
    }
}
