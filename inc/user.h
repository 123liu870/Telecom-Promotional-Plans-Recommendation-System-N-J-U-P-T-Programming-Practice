#ifndef USER_H
#define USER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "system.h"

extern User* currentUser;               // 当前登录用户指针
extern Package* packageList;          // 套餐列表指针
extern User* userList;                // 用户列表指针
extern int totalUsers;                 // 用户总数

//需求结构体
typedef struct {
    int data_mb;         // 流量需求(MB)
    int voice_minutes;   // 通话需求(分钟)
    int sms;             // 短信需求(条)
    int valid;           // 需求有效性标记(1:有效)
} Demand;

// 工具函数
int isStrEmpty(const char* str);          // 判断字符串是否为空
void trimStr(char* str);                  // 去除字符串首尾空格

// 文件操作函数
int loadPackagesFromText();               // 从文本文件加载套餐
int savePackagesToText();                 // 保存套餐到文本文件
int loadUsersFromText();                  // 从文本文件加载用户
int saveUsersToText();                    // 保存用户到文本文件
User* findUser(const char* userId);       // 查找用户

// 核心功能函数
void loginUser();                         // 用户登录
int userRegister();                      // 用户注册
void inputDemandByForm();                 // 填写需求调查
void calcUserStar();                      // 计算用户星级
void matchPackagesByDemand();            // 根据需求匹配套餐
void recommendPackages();              // 推荐套餐入口
void recommendForNewUser();              // 为新用户推荐套餐
void itemBasedCFRecommendation();        // 基于物品的协同过滤推荐
void showMatchedPackages();               // 显示推荐套餐
void queryUserPackage();                  // 查询个人套餐
void applyPackageChange();                // 变更套餐

#endif // USER_H