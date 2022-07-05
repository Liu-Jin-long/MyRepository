#include <stdio.h>

int main()
{
	FILE *fp=fopen("file","rb+");
	char c;
	while(!feof(fp))//当你读取出错后，feof才知道你到结尾了
	{
		c=fgetc(fp);
		if(-1==c)
		{
			printf("%d\n",c);
		}else{
			putchar(c);
		}
	}
	return 0;
}
