#include "HeadFile.h"

void main()
{
	SqQueue Q;
	Init_SqQueue(&Q);
	ElemType data ;
	DeQueue(&Q, &data);
	EnQueue(&Q, 666);
	EnQueue(&Q, 888);
	DeQueue(&Q, &data);
	printf("data=%d", data);

}