#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

void main()
{
	int b;
	scanf("%d", &b);
	int count = 0;
	for (int i = 0; i < 32; i++) 
	{
		if (b & 1 << i) count++;//ÿλ��
	}
	printf("ʮ������%d�Ķ�������1�ĸ���Ϊ%d", b, count);

}