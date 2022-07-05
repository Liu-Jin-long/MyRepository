#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




void big(char* arr[], int size, char** big1, char** big2)
{
	*big1 = strcmp(arr[0], arr[1])>0? arr[0]: arr[1];
	if (*big1 == arr[0]) *big2 = arr[1];
	else *big2 = arr[0];
	for (int i = 2; i < size; i++)
	{
		if (strcmp(*big1, arr[i]) <= 0)
		{
			*big2 = *big1;
			*big1 = arr[i];
		}
		else
		{
			if (strcmp(*big2, arr[i]) < 0) *big2 = arr[i];
		}

	}

}

void main()
{
	char* p[6] = { "hello","world","python","java","work","zero" };
	char* big1;
	char* big2;
	big(p, 6, &big1, &big2);
	puts(big1);
	puts(big2);
}