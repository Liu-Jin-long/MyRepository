#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>



int main()
{
	int a[11] = { 7,5,7,5,7,5,7,5,7,5,5 };
	int candidate = a[0];
	int vote = 0;
	for (int i = 0; i < 11; i++)
	{
		if (candidate == a[i]) {
			vote++;
		}
		else
		{
			vote--;
			if (vote == 0)//相互抵消，为0换人
			{
				vote++;
				candidate = a[i];
			}
		}
	}return candidate;

}