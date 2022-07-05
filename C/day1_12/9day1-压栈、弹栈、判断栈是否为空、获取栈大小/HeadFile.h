#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef int ElemType; 

typedef struct S_Node//Õ»½áµã
{
	ElemType data;
	struct S_Node* next;
}S_Node,*p_Node;

typedef struct Stack
{
	p_Node top;
	int size;
}Stack,*p_Stack;

void Init_Stack(p_Stack s);
int Stack_Size(Stack s);
int IsEmpty(Stack s);
void Push(p_Stack s, ElemType data);
void Pop(p_Stack s);