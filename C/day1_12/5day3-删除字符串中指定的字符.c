#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main()
{
	char c[30];
	gets(c);
	int i = 0;
	char delete = 'a';
	while (c[i])
	{
		if(c[i] != 'a') printf("%c", c[i]);
		i++;
	}


}