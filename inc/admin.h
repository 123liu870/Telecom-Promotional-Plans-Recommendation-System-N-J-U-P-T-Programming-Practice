#ifndef ADMIN_H
#define ADMIN_H

#include <stdio.h>
#include "user.h"
#include "system.h"

#define KEYWORD_LEN 10
#define MAX_PACKAGES 100

/* 管理员相关函数 */
int admin_login();//登录
void admin_menu();//菜单页

/* 套餐管理 */
int load_packages(Package pkgs[], int *count);
int save_packages(Package pkgs[], int count);
void add_package(Package pkgs[], int *count);
void modify_package(Package pkgs[], int count);
void list_packages(Package pkgs[], int count);

#endif /* ADMIN_H */

