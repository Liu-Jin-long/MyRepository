#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main()
{
	char c[30];
	gets(c);
	int i = 0;
	while (c[i])
	{
		if (c[i] == 32) printf("%%020");//要输出%只需在前面再加上一个%，要输出\只需在前面再加上一个\，要输出双引号也只需在前面加上一个\即可
		else printf("%c", c[i]);
		i++;
	}
}