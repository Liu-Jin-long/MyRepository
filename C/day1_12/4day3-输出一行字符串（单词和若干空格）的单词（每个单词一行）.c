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
		if (str[i] != 32)//������ĸ��ʼ��ӡ
		{
			printf("%c", str[i]);
		}
		if (str[i] == 32 && str[i + 1] > 32) printf("\n");//�����ַ��ֱ�Ϊ��ĸ���ո�ʱ����
		i++;
	}

}