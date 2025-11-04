#include "user.h"

// 全局变量定义
Package* packageList = NULL;
int totalPackages = 0;
User* userList = NULL;
int totalUsers = 0;
User currentUser;
Package matchedPackages[10];
int matchedCount = 0;

// 清空输入缓冲区
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 判断字符串是否为空（全空格或空）
int isStrEmpty(const char* str) {
    if (!str) return 1;
    while (*str) {
        if (!isspace((unsigned char)*str)) return 0;
        str++;
    }
    return 1;
}

// 去除字符串首尾空格
void trimStr(char* str) {
    if (!str) return;
    // 去除开头空格
    char* start = str;
    while (isspace((unsigned char)*start)) start++;
    // 去除结尾空格
    char* end = str + strlen(str) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;
    // 重新赋值
    memmove(str, start, end - start + 1);
    str[end - start + 1] = '\0';
}

// 从文本文件加载套餐（格式：id,月费,通话,流量,宽带,意向数）
int loadPackagesFromText() {
    // 释放旧数据
    if (packageList) {
        free(packageList);
        packageList = NULL;
    }
    totalPackages = 0;

    FILE* fp = fopen(PACKAGE_FILE, "r");
    if (!fp) {
        // 首次运行创建空文件
        fp = fopen(PACKAGE_FILE, "w");
        fclose(fp);
        return 1;
    }

    // 先统计行数（套餐数量）
    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        trimStr(line);
        if (!isStrEmpty(line)) totalPackages++;
    }
    rewind(fp);

    // 分配内存
    if (totalPackages > 0) {
        packageList = (Package*)malloc(totalPackages * sizeof(Package));
        if (!packageList) {
            fclose(fp);
            return 0;
        }

        // 解析每行数据
        int i = 0;
        while (fgets(line, sizeof(line), fp)) {
            trimStr(line);
            if (isStrEmpty(line)) continue;

            // 拆分字段（逗号分隔）
            char* token = strtok(line, ",");
            if (token) {
                trimStr(token);
                strncpy(packageList[i].packageId, token, 19);
            }

            token = strtok(NULL, ",");
            if (token) {
                trimStr(token);
                packageList[i].monthlyFee = atof(token);
            }

            token = strtok(NULL, ",");
            if (token) {
                trimStr(token);
                packageList[i].callDuration = atoi(token);
            }

            token = strtok(NULL, ",");
            if (token) {
                trimStr(token);
                packageList[i].flow4G = atoi(token);
            }

            token = strtok(NULL, ",");
            if (token) {
                trimStr(token);
                packageList[i].broadband = atoi(token);
            }

            token = strtok(NULL, ",");
            if (token) {
                trimStr(token);
                packageList[i].intentCount = atoi(token);
            }

            i++;
        }
    }

    fclose(fp);
    return 1;
}

// 保存套餐到文本文件
int savePackagesToText() {
    FILE* fp = fopen(PACKAGE_FILE, "w");
    if (!fp) return 0;

    for (int i = 0; i < totalPackages; i++) {
        // 格式：id,月费,通话,流量,宽带,意向数（换行分隔）
        fprintf(fp, "%s,%.2f,%d,%d,%d,%d\n",
                packageList[i].packageId,
                packageList[i].monthlyFee,
                packageList[i].callDuration,
                packageList[i].flow4G,
                packageList[i].broadband,
                packageList[i].intentCount);
    }

    fclose(fp);
    return 1;
}

// 从文本文件加载用户（格式：id,姓名,套餐id,通话需求,流量需求,宽带需求,消费）
int loadUsersFromText() {
    // 释放旧数据
    if (userList) {
        free(userList);
        userList = NULL;
    }
    totalUsers = 0;

    FILE* fp = fopen(USER_FILE, "r");
    if (!fp) {
        // 首次运行创建空文件
        fp = fopen(USER_FILE, "w");
        fclose(fp);
        return 1;
    }

    // 先统计行数（用户数量）
    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        trimStr(line);
        if (!isStrEmpty(line)) totalUsers++;
    }
    rewind(fp);

    // 分配内存
    if (totalUsers > 0) {
        userList = (User*)malloc(totalUsers * sizeof(User));
        if (!userList) {
            fclose(fp);
            return 0;
        }

        // 解析每行数据
        int i = 0;
        while (fgets(line, sizeof(line), fp)) {
            trimStr(line);
            if (isStrEmpty(line)) continue;

            // 拆分字段（逗号分隔）
            char* token = strtok(line, ",");
            if (token) {
                trimStr(token);
                strncpy(userList[i].userId, token, 19);
            }

            token = strtok(NULL, ",");
            if (token) {
                trimStr(token);
                strncpy(userList[i].userName, token, 49);
            }

            token = strtok(NULL, ",");
            if (token) {
                trimStr(token);
                strncpy(userList[i].currentPackageId, token, 19);
            }

            token = strtok(NULL, ",");
            if (token) {
                trimStr(token);
                userList[i].callDemand = atoi(token);
            }

            token = strtok(NULL, ",");
            if (token) {
                trimStr(token);
                userList[i].flowDemand = atoi(token);
            }

            token = strtok(NULL, ",");
            if (token) {
                trimStr(token);
                userList[i].broadbandDemand = atoi(token);
            }

            token = strtok(NULL, ",");
            if (token) {
                trimStr(token);
                userList[i].consumption = atof(token);
            }

            i++;
        }
    }

    fclose(fp);
    return 1;
}

// 保存用户到文本文件
int saveUsersToText() {
    FILE* fp = fopen(USER_FILE, "w");
    if (!fp) return 0;

    for (int i = 0; i < totalUsers; i++) {
        // 格式：id,姓名,套餐id,通话需求,流量需求,宽带需求,消费（换行分隔）
        fprintf(fp, "%s,%s,%s,%d,%d,%d,%.2f\n",
                userList[i].userId,
                userList[i].userName,
                userList[i].currentPackageId,
                userList[i].callDemand,
                userList[i].flowDemand,
                userList[i].broadbandDemand,
                userList[i].consumption);
    }

    fclose(fp);
    return 1;
}

// 查找用户（返回指针，未找到返回NULL）
User* findUser(const char* userId) {
    for (int i = 0; i < totalUsers; i++) {
        if (strcmp(userList[i].userId, userId) == 0) {
            return &userList[i];
        }
    }
    return NULL;
}

// 填写需求调查
void inputDemandByForm() {
    printf("\n===== 需求调查 =====\n");
    
    printf("请输入每月通话需求（分钟）：");
    while (1) {
        if (scanf("%d", &currentUser.callDemand) == 1 && currentUser.callDemand > 0) {
            break;
        }
        printf("输入错误，请重新输入正整数：");
        clearInputBuffer();
    }
    clearInputBuffer();

    printf("请输入每月4G流量需求（MB）：");
    while (1) {
        if (scanf("%d", &currentUser.flowDemand) == 1 && currentUser.flowDemand > 0) {
            break;
        }
        printf("输入错误，请重新输入正整数：");
        clearInputBuffer();
    }
    clearInputBuffer();

    printf("是否需要宽带（1=需要，0=不需要）：");
    int hasBroadband;
    while (1) {
        if (scanf("%d", &hasBroadband) == 1 && (hasBroadband == 0 || hasBroadband == 1)) {
            break;
        }
        printf("输入错误，请输入0或1：");
        clearInputBuffer();
    }
    clearInputBuffer();

    currentUser.broadbandDemand = 0;
    if (hasBroadband == 1) {
        printf("请输入期望宽带带宽（M）：");
        while (1) {
            if (scanf("%d", &currentUser.broadbandDemand) == 1 && currentUser.broadbandDemand > 0) {
                break;
            }
            printf("输入错误，请重新输入正整数：");
            clearInputBuffer();
        }
        clearInputBuffer();
    }

    // 更新当前用户在列表中的数据
    User* user = findUser(currentUser.userId);
    if (user) {
        user->callDemand = currentUser.callDemand;
        user->flowDemand = currentUser.flowDemand;
        user->broadbandDemand = currentUser.broadbandDemand;
    }

    // 保存到文件
    if (saveUsersToText()) {
        printf("需求提交成功！\n");
    } else {
        printf("需求保存失败！\n");
    }
}

// 计算用户星级
void calcUserStar() {
    int star = 1;
    if (currentUser.consumption >= 80) star = 5;
    else if (currentUser.consumption >= 60) star = 4;
    else if (currentUser.consumption >= 40) star = 3;
    else if (currentUser.consumption >= 20) star = 2;

    printf("\n===== 用户星级 =====\n");
    printf("用户：%s（%s）\n", currentUser.userName, currentUser.userId);
    printf("近3个月月均消费：%.2f元\n", currentUser.consumption);
    printf("星级评定：%d星\n", star);
}

// 匹配套餐
void matchPackagesByDemand() {
    matchedCount = 0;
    if (totalPackages == 0) {
        printf("系统中暂无套餐数据！\n");
        return;
    }

    // 优先匹配完全满足需求的套餐
    for (int i = 0; i < totalPackages; i++) {
        // 宽带需求匹配
        int broadbandMatch = 0;
        if (currentUser.broadbandDemand == 0) {
            broadbandMatch = (packageList[i].broadband == 0) ? 1 : 0;
        } else {
            broadbandMatch = (packageList[i].broadband >= currentUser.broadbandDemand) ? 1 : 0;
        }

        // 通话和流量需求匹配
        int callMatch = (packageList[i].callDuration >= currentUser.callDemand) ? 1 : 0;
        int flowMatch = (packageList[i].flow4G >= currentUser.flowDemand) ? 1 : 0;

        if (broadbandMatch && callMatch && flowMatch) {
            matchedPackages[matchedCount++] = packageList[i];
            if (matchedCount >= 10) break;
        }
    }

    // 无完全匹配时，匹配接近需求的套餐（放宽至80%）
    if (matchedCount == 0) {
        for (int i = 0; i < totalPackages; i++) {
            int broadbandMatch = (currentUser.broadbandDemand == 0) ? 
                (packageList[i].broadband == 0) : (packageList[i].broadband >= currentUser.broadbandDemand);
            int callNear = (packageList[i].callDuration >= currentUser.callDemand * 0.8) ? 1 : 0;
            int flowNear = (packageList[i].flow4G >= currentUser.flowDemand * 0.8) ? 1 : 0;

            if (broadbandMatch && callNear && flowNear) {
                matchedPackages[matchedCount++] = packageList[i];
                if (matchedCount >= 10) break;
            }
        }
    }
}

// 显示推荐套餐
void showMatchedPackages() {
    if (matchedCount == 0) {
        printf("\n无匹配的套餐，请调整需求！\n");
        return;
    }

    printf("\n===== 推荐套餐（共%d个）=====\n", matchedCount);
    printf("序号 | 套餐编号 | 月资费 | 通话时长 | 流量(MB) | 宽带(M)\n");
    printf("----------------------------------------------------\n");
    for (int i = 0; i < matchedCount; i++) {
        printf("%d    | %s | %.2f元  | %d分钟   | %d      | %d\n",
               i+1,
               matchedPackages[i].packageId,
               matchedPackages[i].monthlyFee,
               matchedPackages[i].callDuration,
               matchedPackages[i].flow4G,
               matchedPackages[i].broadband);
    }

    // 办理确认
    char confirm;
    printf("\n是否办理推荐套餐（y/n）：");
    scanf("%c", &confirm);
    clearInputBuffer();

    if (confirm != 'y' && confirm != 'Y') {
        printf("已取消办理！\n");
        return;
    }

    // 选择套餐
    int choice;
    printf("请选择套餐序号（1-%d）：", matchedCount);
    while (1) {
        if (scanf("%d", &choice) == 1 && choice >= 1 && choice <= matchedCount) {
            break;
        }
        printf("输入错误，请重新选择：");
        clearInputBuffer();
    }
    clearInputBuffer();

    // 更新用户套餐
    strcpy(currentUser.currentPackageId, matchedPackages[choice-1].packageId);
    User* user = findUser(currentUser.userId);
    if (user) {
        strcpy(user->currentPackageId, currentUser.currentPackageId);
    }

    // 更新套餐意向数
    for (int i = 0; i < totalPackages; i++) {
        if (strcmp(packageList[i].packageId, matchedPackages[choice-1].packageId) == 0) {
            packageList[i].intentCount++;
            break;
        }
    }

    // 保存变更
    if (saveUsersToText() && savePackagesToText()) {
        printf("套餐办理成功！当前套餐：%s\n", currentUser.currentPackageId);
    } else {
        printf("办理失败，数据保存出错！\n");
    }
}

// 查询个人套餐
void queryUserPackage() {
    printf("\n===== 个人套餐信息 =====\n");
    printf("用户编号：%s\n", currentUser.userId);
    printf("用户姓名：%s\n", currentUser.userName);
    printf("当前套餐：%s\n", currentUser.currentPackageId);

    // 查找套餐详情
    int found = 0;
    for (int i = 0; i < totalPackages; i++) {
        if (strcmp(packageList[i].packageId, currentUser.currentPackageId) == 0) {
            printf("套餐详情：\n");
            printf("  月资费：%.2f元\n", packageList[i].monthlyFee);
            printf("  通话时长：%d分钟\n", packageList[i].callDuration);
            printf("  4G流量：%d MB\n", packageList[i].flow4G);
            printf("  宽带：%d M\n", packageList[i].broadband);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("提示：当前套餐信息未找到（可能已下架）\n");
    }
}

// 变更套餐
void applyPackageChange() {
    printf("\n===== 变更套餐 =====\n");
    printf("请重新填写需求以获取新推荐\n");
    inputDemandByForm();
    matchPackagesByDemand();
    showMatchedPackages();
}

// 用户功能菜单
void userFunctionMenu() {
    // 初始化：加载数据
    if (!loadPackagesFromText() || !loadUsersFromText()) {
        printf("数据加载失败，无法启动用户功能！\n");
        return;
    }

    // 用户登录
    printf("\n===== 用户登录 =====\n");
    char userId[20];
    printf("请输入用户编号：");
    fgets(userId, sizeof(userId), stdin);
    userId[strcspn(userId, "\n")] = '\0';
    trimStr(userId);

    User* loginUser = findUser(userId);
    if (loginUser) {
        currentUser = *loginUser;
        printf("登录成功，欢迎 %s！\n", currentUser.userName);
    } else {
        // 新用户注册
        printf("用户不存在，是否注册新用户（y/n）：");
        char reg;
        scanf("%c", &reg);
        clearInputBuffer();
        if (reg != 'y' && reg != 'Y') {
            printf("登录失败，返回主菜单！\n");
            return;
        }

        // 初始化新用户
        strncpy(currentUser.userId, userId, 19);
        printf("请输入用户名：");
        fgets(currentUser.userName, sizeof(currentUser.userName), stdin);
        currentUser.userName[strcspn(currentUser.userName, "\n")] = '\0';
        trimStr(currentUser.userName);
        strcpy(currentUser.currentPackageId, "未办理");
        currentUser.callDemand = 0;
        currentUser.flowDemand = 0;
        currentUser.broadbandDemand = 0;
        printf("请输入近3个月月均消费（元）：");
        scanf("%f", &currentUser.consumption);
        clearInputBuffer();

        // 添加到用户列表
        totalUsers++;
        userList = (User*)realloc(userList, totalUsers * sizeof(User));
        if (!userList) {
            printf("内存分配失败，注册失败！\n");
            totalUsers--;
            return;
        }
        userList[totalUsers - 1] = currentUser;

        // 保存新用户
        if (saveUsersToText()) {
            printf("注册成功，已登录！\n");
        } else {
            printf("注册失败，返回主菜单！\n");
            return;
        }
    }

    // 菜单循环
    while (1) {
        printf("\n===== 用户功能菜单 =====\n");
        printf("1. 填写需求调查\n");
        printf("2. 查看用户星级\n");
        printf("3. 查看推荐套餐\n");
        printf("4. 查询个人套餐\n");
        printf("5. 变更套餐\n");
        printf("6. 返回主菜单\n");
        printf("请选择：");

        int choice;
        if (scanf("%d", &choice) != 1) {
            printf("输入错误，请输入数字！\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                inputDemandByForm();
                break;
            case 2:
                calcUserStar();
                break;
            case 3:
                matchPackagesByDemand();
                showMatchedPackages();
                break;
            case 4:
                queryUserPackage();
                break;
            case 5:
                applyPackageChange();
                break;
            case 6:
                printf("返回主菜单...\n");
                // 释放内存
                if (packageList) free(packageList);
                if (userList) free(userList);
                return;
            default:
                printf("无效选项，请输入1-6！\n");
        }
    }
}
