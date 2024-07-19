//hhah
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUM 0+1
int main()
{
	FILE* f=fopen("test","ab+");
	char buf[100];
	bzero(buf,sizeof(buf));
	fread(buf,sizeof(char),5,f);
	printf("%s读取结束\n",buf);
	bzero(buf,sizeof(buf));
	strcpy(buf,"hello world");
	fwrite(buf,sizeof(char),strlen(buf),f);
	printf("\n");
	// bzero(buf,sizeof(buf));
	// fread(buf,sizeof(char),5,f);
	// printf("%s读取结束\n",buf);
	return 0;
}
