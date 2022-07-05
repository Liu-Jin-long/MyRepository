#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//void main()//mystrcpy
//{
//	char from[20] = "hello my world!";
//	char to[20] = "";
//	int i = 0;
//	while (from[i]) {
//		to[i] = from[i];
//		i++;
//	}
//	printf("%s", to);
//}
//
//int mystrlen(char c[]) //mystrlen
//{
//	char c[20] = "hello my world!";
//	int count = 0;
//	while (c[count])
//	{
//		count++;
//	}
//	return count;                  
//}

void mystrcat(char a[], char b[]) //mystrcat
{

	int i = 0, j = 0;
	while (a[i]) i++;
	while (b[j])
	{
		a[i++] = b[j++];
	}
}

void main()
{
	char a[20] = "hello";
	char b[10] = " my world";
	mystrcat(a, b);
	printf("%s", a);
}



//int mystrcmp(char a[], int alength,char b[],int blength)
//{
//	int i = 0;
//	while (a[i] == b[i] && i <= (alength > blength ? alength:blength)) i++;//找到不相等时i
//	if (a[i] > b[i]) 
//	{
//		return 1;
//	}
//	else if (a[i] < b[i]) 
//	{
//		return -1;
//	}
//	else //相等时
//	{
//		if (alength > blength) 
//		{
//			return 1;
//		}
//		else if(alength < blength)
//		{
//			return -1;
//		}
//		else
//		{
//			return 0;
//		}
//	}
//}
