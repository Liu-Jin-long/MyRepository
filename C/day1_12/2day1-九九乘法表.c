#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

void main() {
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j <= i; j++)
		{
			printf("%d*%d=%-2d ", i, j, i * j);//-Ϊ����루Ĭ���Ҷ��룩��2Ϊռ�����ֽڴ�ӡ
		}
		printf("\n");
	}
}
