#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main()
{
	char str[30];
	gets(str);
	/*printf("%s", str);*/
	int count = 0, i = 0;
	while (str[i] && str[i + 1])
	{
		if (str[i] > 32 && str[i + 1] == 32)
		{
			count++;
		}//相邻字符为单词、空格时计数加一
		i++;
	}
	if (i == 0) {//最后一个字符为单个单词时
		if (str[i] > 32) return 1;
		else return 0;
	}
	else if (str[i - 1] == 32 && str[i] > 32)//最后两个字符分别为空格、字母时
	{
		count++;
	}
	else;

	printf("%d", count);
	return count;
}