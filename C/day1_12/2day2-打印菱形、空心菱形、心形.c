#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

//
//void main()//主要找到行和列的关系再进行判断打印
//{
//	for (int i = 1; i < 10; i++)
//	{
//		for (int j = 1; j < 10; j++)
//		{
//			if (j > abs(5 - i) && j < 10 - abs(5 - i) && j % 2 == i % 2)
//			{
//				printf("*");
//			}
//			else printf(" ");
//		}
//		printf("\n");
//	}
//
//}
//
//void main()
//{
//	for (int i = 1; i < 10; i++)
//	{
//		for (int j = 1; j < 10; j++)
//		{
//			if (j == abs(5 - i) + 1 || j == 9 - abs(5 - i))
//			{
//				printf("*");
//			}
//			else printf(" ");
//		}
//		printf("\n");
//	}
//
//}

void main()
{
	for (int i = 1; i < 5; i++)
	{
		for (int j = 1; j <= 19; j++)
		{
			if (((j >= 5 - i && j <= 5 + i) || (j >= 15 - i && j <= 15 + i)) && j % 2 != i % 2)//或加括号否则短路
			{
				printf("*");
			}
			else printf(" ");
		}
		printf("\n");
	}


	for (int i = 1; i <= 9; i++)
	{
		for (int j = 1; j <= 19; j++)
		{
			if (j >= i + 1 && j <= 19 - i && j % 2 != i % 2)
			{
				printf("*");
			}
			else printf(" ");
		}
		printf("\n");


	}


}