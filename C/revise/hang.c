#include<unistd.h>
#include<stdio.h>
int main()
{
	for(int i=0;i<5;i++)
	{
		printf("i=%d\n",i);
	}
	sleep(3);
	printf("Hello!");
} 
