#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




void main()
{
	char str[30];
	gets(str);
	/*printf("%s", str);*/
	int i = 0;
	while (str[i])
	{
		if (str[i] != 32)//遇到字母开始打印
		{
			printf("%c", str[i]);
		}
		if (str[i] == 32 && str[i + 1] > 32) printf("\n");//相邻字符分别为字母、空格时换行
		i++;
	}

}