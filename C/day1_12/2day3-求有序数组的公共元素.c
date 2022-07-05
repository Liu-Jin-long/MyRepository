#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

void two_array(int a[], int aleng, int b[], int bleng)//两个有序数组的公共元素
{
	int pa = 0, pb = 0;//两数组下标指针
	while (pa < aleng && pb < bleng)
	{
		if (a[pa] > b[pb])
		{
			pb++;
		}
		else if (a[pa] < b[pb])
		{
			pa++;
		}
		else {
			printf("%d ", a[pa]);
			pa++;
			pb++;
		}
	}
}

void three_array(int a[], int aleng, int b[], int bleng, int c[], int cleng)//三个有序数组的公共元素
{
	int pa = 0, pb = 0, ps = 0;//两数组下标指针
	int ab_same_elem[10];
	while (pa < aleng && pb < bleng)
	{
		if (a[pa] > b[pb])
		{
			pb++;
		}
		else if (a[pa] < b[pb])
		{
			pa++;
		}
		else {
			ab_same_elem[ps] = a[pa];
			ps++;
			pa++;
			pb++;
		}
	}
	two_array(c, cleng, ab_same_elem, ps);

}



void main()
{
	int a[10] = { 1,3,5,7,8,9,12,13,14,15 };
	int b[8] = { 1,2,7,10,11,13,14,18 };
	int c[9] = { 2,3,5,7,8,9,10,11,13 };
	two_array(a, 10, b, 8);
	printf("\n");
	three_array(a, 10, b, 8, c, 9);

}