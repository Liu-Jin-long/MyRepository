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
		if (c[i] == 32) printf("%%020");//Ҫ���%ֻ����ǰ���ټ���һ��%��Ҫ���\ֻ����ǰ���ټ���һ��\��Ҫ���˫����Ҳֻ����ǰ�����һ��\����
		else printf("%c", c[i]);
		i++;
	}
}