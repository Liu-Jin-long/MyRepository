#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

void main()
{
	int a[10] = { 2,8,66,-2,888,9999,7777,4444,456,123 };
	int min = abs(a[0] - a[1]);
	int x = a[0];
	int y = a[1];
	for (int i = 0; i < 10; i++)
	{
		for (int j = i + 1; j < 10; j++)
		{
			if (abs(a[i] - a[j]) < min)
			{
				min = abs(a[i] - a[j]);//������С����ֵ
				x = a[i];
				y = a[j];
			}
		}
	}
	printf("x=%d,y=%d ��С�Ĳ�ֵ����ֵ��%d", x, y, min);
}
