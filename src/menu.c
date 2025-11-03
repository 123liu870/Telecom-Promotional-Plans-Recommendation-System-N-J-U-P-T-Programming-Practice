#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/menu.h"
#include "../inc/admin.h"
#include "../inc/user.h"

void main_menu()
{
    while (1)
    {
        printf("\n=== 电信优惠套餐管理系统 ===\n");
        printf("1. 管理员登录\n2. 用户登录\n0. 退出\n请选择: ");
        int choice;
        if (scanf("%d", &choice) != 1) { getchar(); continue; }
        getchar();
        if (choice == 0) break;
        else if (choice == 1)
        {
            if (admin_login())
            {
                admin_menu();
            }
        }
        else if (choice == 2)
        {
            User users[MAX_USERS];
            int ucount = 0;
            load_users(users, &ucount);
            int idx = user_login(users, ucount);
            if (idx >= 0)
            {
                user_menu(&users[idx]);
                /* 用户退出后保存 */
                save_users(users, ucount);
            }
        }
        else
        {
            printf("无效选择。\n");
        }
    }
}
