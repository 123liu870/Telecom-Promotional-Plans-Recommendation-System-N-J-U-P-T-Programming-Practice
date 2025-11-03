#ifndef ADMIN_H
#define ADMIN_H

#include <stdio.h>
#include "user.h"

#define KEYWORD_LEN 10
#define MAX_PACKAGES 100

/*套餐基本信息*/
typedef struct package
{
    int id;                         // 套餐 ID
    char name[64];                  // 套餐名称，NUL 结尾
    double monthly_fee;             // 月资费（元）
    int data_mb;                    // 每月流量（MB）
    int voice_minutes;              // 语音分钟数
    int sms;                        // 短信条数
    int contract_months;            // 合同月数（0 表示无合约）
    char start_date[11];            // 生效日期，格式 "YYYY-MM-DD"
    char end_date[11];              // 终止日期，格式 "YYYY-MM-DD"（无合约可留空）
    int is_active;                  // 是否启用（0 = 否, 1 = 是）
    char description[256];          // 备注/描述
} Package;

/* 管理员相关函数 */
int admin_login();
void admin_menu();

/* 套餐管理 */
int load_packages(Package pkgs[], int *count);
int save_packages(Package pkgs[], int count);
void add_package(Package pkgs[], int *count);
void modify_package(Package pkgs[], int count);
void list_packages(Package pkgs[], int count);

#endif /* ADMIN_H */

