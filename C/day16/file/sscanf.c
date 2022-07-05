#include <stdio.h>



typedef struct
{
	int num;
	float score;
	char name[20];
}stu;

int main()
{
	stu s={0};
	char buf[1024]="1111 99.9 xiuer";
	sscanf(buf,"%d%f%s",&s.num,&s.score,s.name);
	printf("%d %5.2f %s\n",s.num,s.score,s.name);
	return 0;
}
