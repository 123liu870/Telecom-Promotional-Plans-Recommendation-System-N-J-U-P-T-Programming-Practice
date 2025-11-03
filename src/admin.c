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
