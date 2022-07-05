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
		printf("ջΪ�գ����ɳ�ջ");
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
	if (s->top == NULL)//ջΪ��
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
	if (s.top == NULL) return 1;//ջΪ��
	else return 0;
}

int Stack_Size(Stack s)
{
	return s.size;
}
