#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void reverse(char a[],int low,int high)
{
	
	while (low < high) //��ת
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
		if (i == 0 && str[i] > 32) j = i;//�ж�str[0]�Ƿ�Ϊ��ĸ
		else if (str[i] > 32 && str[i - 1] == 32)  j = i; //�����ַ��ֱ�Ϊ�ո���ĸʱ��¼low
		else ;

		if (str[i] > 32 && (str[i + 1] == 32 || str[i + 1] == 0)) reverse(str, j, i); //�����ַ��ֱ�Ϊ��ĸ���ո�(��βΪ\0)ʱ �ҵ�high�� ��ת����
	}
	/*str[length + 1] = 0;*/
	printf("%s", str);
}

	