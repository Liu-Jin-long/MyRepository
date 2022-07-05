#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define N 5

typedef int ElemType;

typedef struct  {
	ElemType data[N];
	int front, rear;
}SqQueue;

void DeQueue(SqQueue* Q, ElemType* data);
void EnQueue(SqQueue* Q, ElemType data);
void Init_SqQueue(SqQueue* Q);
