#include "HeadFile.h"


void Init_SqQueue(SqQueue* Q)
{
	Q->front = Q->rear = 0;
}
void EnQueue(SqQueue* Q, ElemType data)
{
	if ((Q->rear + 1) % N == Q->front)
	{
		printf("循环队列已满，不可入列\n");
	}
	else
	{
		Q->data[Q->rear] = data;
		Q->rear = (Q->rear + 1) % N;
	}
}

void DeQueue(SqQueue* Q, ElemType* data)//出列，值由data带出去
{
	if (Q->front == Q->rear)
	{
		printf("循环队列已空，不可出列\n");
	}
	else
	{
		*data = Q->data[Q->front];
		Q->front = (Q->front + 1) % N;
	}
}