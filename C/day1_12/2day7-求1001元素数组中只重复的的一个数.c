#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int main()//«Û∫Õºı»•1~1000
{
	int repeat = 0;
	int a[1001];
	a[1000] = 10;
	for (int i = 0; i < 1000; i++)
	{
		a[i] = i + 1;
	}
	for (int i = 0; i < 1001; i++)
	{
		repeat = repeat + a[i] - i;
	}
	return repeat;
}