#include <stdio.h>



typedef struct
{
	int num;
	float score;
	char name[20];
}stu;

int main()
{
	stu s={1111,99.9,"xiuer"};
	char buf[1024];
	sprintf(buf,"%d %5.2f %s",s.num,s.score,s.name);
	puts(buf);
	return 0;
}
