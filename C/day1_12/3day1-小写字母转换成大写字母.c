#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


int main()
{
	char c;
	while (rewind(stdin), scanf("%c", &c) != EOF)
	{
		if (c <= 122 && c >= 97)
		{
			printf("%c\n", c - 32);
		}
		else return -1;
	}
}