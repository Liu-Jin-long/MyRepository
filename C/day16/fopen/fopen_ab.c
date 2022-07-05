#include <stdio.h>

int main()
{
	FILE *fp=fopen("file","ab+");
	if(NULL==fp)
	{
		perror("fopen");
		return -1;
	}
	//fwrite("world",sizeof(char),5,fp);
	char buf[10]={0};
	fread(buf,1,5,fp);
	puts(buf);
	return 0;
}
