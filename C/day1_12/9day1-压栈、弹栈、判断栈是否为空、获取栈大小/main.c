#include "HeadFile.h"


void main()
{
	Stack s;
	Init_Stack(&s);
	Push(&s, 5);
	Push(&s, 9);
	Push(&s, 3);
	Push(&s, 10);
	Pop(&s);
	Pop(&s);
	printf("Stack is empty? %s\n", IsEmpty(s)==1 ? "YES" :"NO");
	
}