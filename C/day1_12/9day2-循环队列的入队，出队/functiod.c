#include "HeadFile.h"


void Init_SqQueue(SqQueue* Q)
{
	Q->front = Q->rear = 0;
}
void EnQueue(SqQueue* Q, ElemType data)
{
	if ((Q->rear + 1) % N == Q->front)
	{
		printf("ѭ��������������������\n");
	}
	else
	{
		Q->data[Q->rear] = data;
		Q->rear = (Q->rear + 1) % N;
	}
}

void DeQueue(SqQueue* Q, ElemType* data)//���У�ֵ��data����ȥ
{
	if (Q->front == Q->rear)
	{
		printf("ѭ�������ѿգ����ɳ���\n");
	}
	else
	{
		*data = Q->data[Q->front];
		Q->front = (Q->front + 1) % N;
	}
}