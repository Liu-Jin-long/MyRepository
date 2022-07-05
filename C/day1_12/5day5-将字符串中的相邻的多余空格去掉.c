#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 100


void main()
{
	char c[N];
	gets(c);
	int j = 0, i = 0;//双指针
	while (c[i])//连续空格只保留一个
	{
		if (c[i] == 32 && c[0] == 32) //消除开头空格
		{
			i++;
			continue;
		}
		if ((c[i] == 32 && c[i + 1] == 32))//连续空格只保留一个
		{
			i++;
			continue;
		}
		if (c[i] > 96 && c[i] < 123 && c[i + 1]>96 && c[i + 1] < 123)//字母向前移动
		{
			c[j++] = c[i++];
			continue;
		}
		c[j++] = c[i++];
	}
	c[j] = 0;//结束符
	if(j!=0&&c[j-1]==32) c[j-1] = 0;//最后一位为空格时
	printf("%s\n", c);
}


