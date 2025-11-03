#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h> /* getch() 用于密码输入遮掩，实现无回显输入读取 */

#include "../inc/admin.h"

static const char *PKG_FILE = "packages.dat";

/* 从控制台读取密码并用 '*' 代替显示，返回已填充的缓冲区（ caller 提供 buf ） */
void read_password(char *buf, int buflen)
{
	int idx = 0;
	char ch;
	while (1)
	{
		ch = getch();
		if (ch == '\r' || ch == '\n')
		{
			putchar('\n');
			break;
		}
		else if (ch == '\b')
		{
			if (idx > 0)
			{
				idx--;
				/* 退格并清除 */
				putchar('\b');
				putchar(' ');
				putchar('\b');
			}
		}
		else if (idx < buflen - 1)
		{
			buf[idx++] = ch;
			putchar('*');
		}
	}
	buf[idx] = '\0';
}

int admin_login()
{
	char username[64];
	char password[64];

	printf("管理员用户名: ");
	if (scanf("%63s", username) != 1)
		return 0;
	/* 清除换行残留 */
	getchar();
	printf("密码: ");
	read_password(password, sizeof(password));

	/* 简单硬编码校验：用户名 admin 密码 admin123 */
	if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0)
	{
		printf("登录成功。\n");
		return 1;
	}
	else
	{
		printf("用户名或密码错误。\n");
		return 0;
	}
}

/* 简单的二进制持久化 */
int load_packages(Package pkgs[], int *count)
{
	FILE *fp = fopen(PKG_FILE, "rb");
	if (!fp)
	{
		*count = 0;
		return 0;
	}
	if (fread(count, sizeof(int), 1, fp) != 1)
	{
		fclose(fp);
		*count = 0;
		return 0;
	}
	if (*count > MAX_PACKAGES) *count = MAX_PACKAGES;
	fread(pkgs, sizeof(Package), *count, fp);
	fclose(fp);
	return 1;
}

int save_packages(Package pkgs[], int count)
{
	FILE *fp = fopen(PKG_FILE, "wb");
	if (!fp) return 0;
	fwrite(&count, sizeof(int), 1, fp);
	fwrite(pkgs, sizeof(Package), count, fp);
	fclose(fp);
	return 1;
}

void list_packages(Package pkgs[], int count)
{
	printf("\n当前套餐列表（%d）:\n", count);
	for (int i = 0; i < count; i++)
	{
		Package *p = &pkgs[i];
		printf("ID:%d 名称:%s 资费:%.2f 元 流量:%dMB 通话:%d 分 短信:%d 合约:%d月 启用:%d\n",
			   p->id, p->name, p->monthly_fee, p->data_mb, p->voice_minutes, p->sms, p->contract_months, p->is_active);
	}
}

void add_package(Package pkgs[], int *count)
{
	if (*count >= MAX_PACKAGES)
	{
		printf("已达到最大套餐数 %d，无法添加。\n", MAX_PACKAGES);
		return;
	}
	Package *p = &pkgs[*count];
	p->id = (*count == 0) ? 1 : (pkgs[*count - 1].id + 1);
	printf("输入套餐名称: ");
	fgets(p->name, sizeof(p->name), stdin);
	p->name[strcspn(p->name, "\n")] = '\0';
	printf("月资费(元): ");
	scanf("%lf", &p->monthly_fee);
	printf("每月流量(MB): ");
	scanf("%d", &p->data_mb);
	printf("语音分钟数: ");
	scanf("%d", &p->voice_minutes);
	printf("短信条数: ");
	scanf("%d", &p->sms);
	printf("合约月数(0表示无合约): ");
	scanf("%d", &p->contract_months);
	getchar();
	printf("生效日期 (YYYY-MM-DD): ");
	fgets(p->start_date, sizeof(p->start_date), stdin);
	p->start_date[strcspn(p->start_date, "\n")] = '\0';
	printf("终止日期 (YYYY-MM-DD, 可空): ");
	fgets(p->end_date, sizeof(p->end_date), stdin);
	p->end_date[strcspn(p->end_date, "\n")] = '\0';
	printf("是否启用 (1=是,0=否): ");
	scanf("%d", &p->is_active);
	getchar();
	printf("备注: ");
	fgets(p->description, sizeof(p->description), stdin);
	p->description[strcspn(p->description, "\n")] = '\0';

	(*count)++;
	save_packages(pkgs, *count);
	printf("添加成功（ID=%d）。\n", p->id);
}

void modify_package(Package pkgs[], int count)
{
	if (count == 0)
	{
		printf("没有套餐可供修改。\n");
		return;
	}
	int id;
	printf("输入要修改的套餐 ID: ");
	scanf("%d", &id);
	getchar();
	int found = -1;
	for (int i = 0; i < count; i++) if (pkgs[i].id == id) { found = i; break; }
	if (found == -1)
	{
		printf("未找到 ID=%d 的套餐。\n", id);
		return;
	}
	Package *p = &pkgs[found];
	printf("当前名称: %s\n输入新名称(回车保持不变): ", p->name);
	char buf[128];
	fgets(buf, sizeof(buf), stdin);
	if (buf[0] != '\n') { buf[strcspn(buf, "\n")]='\0'; strncpy(p->name, buf, sizeof(p->name)); }
	printf("当前月资费: %.2f\n输入新资费(0保持不变): ", p->monthly_fee);
	double d; scanf("%lf", &d); if (d > 0) p->monthly_fee = d; getchar();
	save_packages(pkgs, count);
	printf("修改保存。\n");
}

void admin_menu()
{
	Package pkgs[MAX_PACKAGES];
	int pkg_count = 0;
	load_packages(pkgs, &pkg_count);
	while (1)
	{
		printf("\n--- 管理员菜单 ---\n");
		printf("1. 列出套餐\n2. 添加套餐\n3. 修改套餐\n0. 退出管理员\n选择: ");
		int cmd;
		if (scanf("%d", &cmd) != 1) { getchar(); continue; }
		getchar();
		if (cmd == 0) break;
		if (cmd == 1) list_packages(pkgs, pkg_count);
		else if (cmd == 2) add_package(pkgs, &pkg_count);
		else if (cmd == 3) modify_package(pkgs, pkg_count);
		else printf("无效选项。\n");
	}
}
