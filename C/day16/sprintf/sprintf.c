#include <stdio.h>

typedef struct{
	int num;
	float score;
	char name[20];
}stu;

int main()
{
	stu s={1001,92.3,"lele"};
	char buf[1024]={0};
	sprintf(buf,"%d %5.2f %s",s.num,s.score,s.name);
	puts(buf);
	return 0;
}
