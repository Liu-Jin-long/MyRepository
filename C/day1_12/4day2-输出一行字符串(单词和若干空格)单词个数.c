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
		}//�����ַ�Ϊ���ʡ��ո�ʱ������һ
		i++;
	}
	if (i == 0) {//���һ���ַ�Ϊ��������ʱ
		if (str[i] > 32) return 1;
		else return 0;
	}
	else if (str[i - 1] == 32 && str[i] > 32)//��������ַ��ֱ�Ϊ�ո���ĸʱ
	{
		count++;
	}
	else;

	printf("%d", count);
	return count;
}