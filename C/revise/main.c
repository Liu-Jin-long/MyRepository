#include <stdio.h>
#define NUM 0+1
int main()
{
	int i=NUM*NUM;
	printf("i=%d\n",i);
	printf("Hello World!\n");
	printf("&i=%p\n",&i);
	void* p=(void**)&i;
	//printf("p=%ld\n",(long)*p);
	p=(void*)-1;
	printf("i=%d\n",i);
	printf("&i=%p\n",&i);
	printf("(void**)&i=%p\n",(void**)&i);
	printf("p=%p\n",p);
	printf("&p=%p\n",&p);
	
	i=p;
	printf("i=%d\n",i);
	printf("&i=%p\n",&i);
	//print("&&i=%p\n");
	return 0;
}
