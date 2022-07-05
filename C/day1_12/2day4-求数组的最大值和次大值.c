#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


void main()
{
	int a[10] = { 5,8,6,9,7,2,3,6,7,10 };
	int max_1 = a[0] > a[1] ? a[0] : a[1];
	int max_2 = a[1] + a[0] - max_1;
	for (int i = 2; i <= 9; i++)
	{
		if (a[i] >= max_1)//数组值大于等于最大值时
		{
			max_2 = max_1;
			max_1 = a[i];
		}
		else {
			if (a[i] > max_2) max_2 = a[i];//数组值小于最大值而大于次大值时
		}
	}
	printf("max_1=%d\nmax_2=%d", max_1, max_2);
}
