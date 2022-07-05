#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void main()
{
	char c[30];
	gets(c);
	for (int j = 0 , i = 0; j < 2; j++)//遍历两边
	{
		while (c[i])
		{
			if(c[i] > 47 && c[i] < 58 && j==0) printf("%c", c[i]);//先打印数字
			if(c[i] > 96 && c[i] < 123 && j==1) printf("%c", c[i]);//再打印字母
			i++;
		}
		i = 0;
	}

}