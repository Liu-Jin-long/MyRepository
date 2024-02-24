#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


int appear_once(int a[],int low,int high) {//��101��������������50�������������Σ�1����������һ�Σ� �ҳ�������һ�ε��Ǹ���
	int num = 0;
	for (int i = low; i <=high; i++)
	{
		num ^= a[i];//���
	}
	return num;
}

void appear_twice(int a[],int length,int* x,int* y)//��102��������������50�������������Σ�2����������һ�Σ� �ҳ�������һ�ε���2������
{
	int n = 0;
	for (int i = 0; i < length; i++)
	{
		n ^= a[i];
	}
	n = -n & n;
	int mid = 0, high = length - 1;
	int temp=a[0];
	while (mid <high)//ģ�¿��ţ�������Ԫ�ط�Ϊ������
	{

		while ((a[high] & n) >= 1 && high > mid) high--;
		a[mid] = a[high];
		while ((a[mid] & n) == 0 && high > mid) mid++;
		a[high] = a[mid];
	}
	a[high] = temp;//����a[0]
	if ((temp & n) == 0)
	{
		*x = appear_once(a, 0, high);
		*y = appear_once(a, high + 1, length - 1);
	}
	else if ((temp & n) >= 1)
	{
	*x = appear_once(a, 0, high - 1);
	*y = appear_once(a, high, length - 1);
	}
	else ;
}



void main()
{
	//int a[101];
	//for (int i = 0; i < 101; i++) //ѭ����ֵ����
	//{
	//	if (i < 50) a[i] = 100 + i;
	//	else if (i > 50) a[i] = 49 + i;
	//	else a[i] = 100;
	//	//printf("%d ", a[i]);
	//}
	//int n = appear_once(a, 0, 100);
	//printf("%d\n", n);
	
	int b[102];
	b[50] = 666;
	b[101] = 888;
	for (int i = 0; i < 101; i++) //ѭ����ֵ����
	{
		if (i < 50) b[i] = 100 + i;
		if (i > 50) b[i] = 49 + i;
		//printf("%d ", b[i]);
	}
	int x, y;
	appear_twice(b, 102, &x, &y);
	printf("����һ�ε��������ֱ�Ϊ%d��%d", x, y);
}