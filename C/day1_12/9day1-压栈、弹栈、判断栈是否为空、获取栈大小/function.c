#include "HeadFile.h"


void Init_Stack(p_Stack s)
{
	s->size = 0;
	s->top = NULL;
}

void Pop(p_Stack s)
{
	if (s->top == NULL)
	{
		printf("栈为空，不可出栈");
	}
	else
	{
		S_Node* p = s->top->next;
		free(s->top);
		s->top = p;
		p = NULL;
		s->size--;
	}
}

void Push(p_Stack s,ElemType data)
{
	S_Node* p = (S_Node*)calloc(1, sizeof(S_Node));
	p->data = data;
	p->next = NULL;
	if (s->top == NULL)//栈为空
	{
		s->top = p;
	}
	else
	{
		p->next = s->top;
		s->top = p;
	}
	s->size++;
}

int IsEmpty(Stack s)
{
	if (s.top == NULL) return 1;//栈为空
	else return 0;
}

int Stack_Size(Stack s)
{
	return s.size;
}
