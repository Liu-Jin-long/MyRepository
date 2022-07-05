#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define N 20


void main()
{
	int array[N] = {1,2,2,2,3,3,3,4,4,5,5,5,6,6,6,7,8,8,9,9};
	int j = 0,i = 0;//双指针
	while (i < N)
	{
		if (array[i] == array[j])
		{
			i++;	
		}
		else
		{
			array[++j] = array[i++];//不等时j后移赋值
		}
	}
	for (int i = 0; i <= j; i++) printf("%d", array[i]);
	

}