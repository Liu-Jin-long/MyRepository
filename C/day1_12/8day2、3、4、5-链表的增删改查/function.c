#include "HeadFile.h"


void print_linkList(LinkList L)
{
	while (L)
	{
		printf("%d ", L->data);
		L = L->next;
	}
	printf("\n");
}

void Tail_Insert(LinkList* head, LinkList* tail , ElemType data)
{
	LNode* p = (LNode*)calloc(1, sizeof(LNode));//clear allocation,中文名为动态内存分配并清零” calloc会把申请的空间全部初始化为0
	p->data = data;
	p->next = NULL;
	if (*head == NULL)
	{
		*head = p;
		*tail = p;
	}
	else
	{
		(*tail)->next = p;
		*tail = p;
	}
}

void Head_Insert(LinkList* head, LinkList* tail, ElemType data)
{
	LNode* p = (LNode*)calloc(1, sizeof(LNode));
	p->data = data;
	p->next = NULL;
	if (*head == NULL)
	{
		*head = p;
		*tail = p;
	}
	else
	{
		p->next = *head;
		*head = p;
	}
}

void LNode_Delete(LinkList* head, LinkList* tail, ElemType data)
{
	LNode* p, * pre;
	p = pre = *head;
	if (*head == NULL)
	{
		printf("链表已为空");
		return;
	}
	if (p->data==data) 
	{
		*head = p->next;
		if (p->next == NULL)
		{
			*tail = NULL;
		}
	}
	else
	{
		while (p)
		{
			if (p->data == data)//若删除尾结点，pre->next=NULL;
			{
				pre->next = p->next;
				free(p);//p未置为NULL，此时为野指针；
				break;
			}
			pre = p;
			p = p->next;
		}
		if (pre->next == NULL)//删除链表尾时
		{
			*tail = pre;
		}
		if (p == NULL)
		{
			printf("未找到该结点\n");
		}
		p = NULL;
	}

}

void Order_Insert(LinkList* head, LinkList* tail, ElemType data)
{
	LNode* new_insert = (LNode*)calloc(1, sizeof(LNode));
	new_insert->data = data;
	new_insert->next = NULL;
	LNode* pre = *head ,* p=*head;
	if (p == NULL)//此时无结点，新结点作为头尾结点
	{
		*head = *tail = new_insert;
	}
	else if (new_insert->data <= p->data)
	{
		new_insert->next = p;
		*head = new_insert;
	}
	else
	{
		while (p)
		{
			if (new_insert->data > p->data)//新结点值大于工作指针值
			{
				pre = p;
				p = p->next;
			}
			else//第一次遇到小于或等于
			{
				new_insert->next = p;
				pre->next = new_insert;
				return;
			}
		}
		pre->next = new_insert;//新结点插入到链表尾
		(*tail) = new_insert;
	}

}