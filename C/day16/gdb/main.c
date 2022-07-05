#include <stdio.h>
#include <stdlib.h>
void print(int i)
{
	printf("i=%d\n",i);
}

int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		printf("error args\n");
		return -1;
	}
	int i=atoi(argv[1]);
	int j=atoi(argv[2]);
	int arr[5]={1,2,3,4,5};
	for(i=0;i<5;i++)
	{
		printf("%3d",arr[i]);
	}
	putchar('\n');
	print(i);
	return 0;
}
