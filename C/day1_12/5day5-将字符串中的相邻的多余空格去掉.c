#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 100


void main()
{
	char c[N];
	gets(c);
	int j = 0, i = 0;//˫ָ��
	while (c[i])//�����ո�ֻ����һ��
	{
		if (c[i] == 32 && c[0] == 32) //������ͷ�ո�
		{
			i++;
			continue;
		}
		if ((c[i] == 32 && c[i + 1] == 32))//�����ո�ֻ����һ��
		{
			i++;
			continue;
		}
		if (c[i] > 96 && c[i] < 123 && c[i + 1]>96 && c[i + 1] < 123)//��ĸ��ǰ�ƶ�
		{
			c[j++] = c[i++];
			continue;
		}
		c[j++] = c[i++];
	}
	c[j] = 0;//������
	if(j!=0&&c[j-1]==32) c[j-1] = 0;//���һλΪ�ո�ʱ
	printf("%s\n", c);
}


