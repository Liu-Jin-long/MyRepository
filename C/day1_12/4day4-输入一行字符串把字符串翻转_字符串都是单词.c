#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void reverse(char a[],int low,int high)
{
	
	while (low < high) //翻转
	{
		char c = a[high];
		a[high] = a[low];
		a[low] = c;
		high--;
		low++;
	}
}


void main()
{
	char str[30];
	gets(str);
	int length = strlen(str);
	/*printf("%s", str);*/
	reverse(str, 0, length - 1);
	for (int i = 0,j=0; i < length; i++)
	{
		if (i == 0 && str[i] > 32) j = i;//判断str[0]是否为字母
		else if (str[i] > 32 && str[i - 1] == 32)  j = i; //相邻字符分别为空格、字母时记录low
		else ;

		if (str[i] > 32 && (str[i + 1] == 32 || str[i + 1] == 0)) reverse(str, j, i); //相邻字符分别为字母、空格(串尾为\0)时 找到high， 翻转单词
	}
	/*str[length + 1] = 0;*/
	printf("%s", str);
}

	