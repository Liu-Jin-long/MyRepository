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
	LNode* p = (LNode*)calloc(1, sizeof(LNode));//clear allocation,������Ϊ��̬�ڴ���䲢���㡱 calloc�������Ŀռ�ȫ����ʼ��Ϊ0
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
		printf("������Ϊ��");
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
			if (p->data == data)//��ɾ��β��㣬pre->next=NULL;
			{
				pre->next = p->next;
				free(p);//pδ��ΪNULL����ʱΪҰָ�룻
				break;
			}
			pre = p;
			p = p->next;
		}
		if (pre->next == NULL)//ɾ������βʱ
		{
			*tail = pre;
		}
		if (p == NULL)
		{
			printf("δ�ҵ��ý��\n");
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
	if (p == NULL)//��ʱ�޽�㣬�½����Ϊͷβ���
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
			if (new_insert->data > p->data)//�½��ֵ���ڹ���ָ��ֵ
			{
				pre = p;
				p = p->next;
			}
			else//��һ������С�ڻ����
			{
				new_insert->next = p;
				pre->next = new_insert;
				return;
			}
		}
		pre->next = new_insert;//�½����뵽����β
		(*tail) = new_insert;
	}

}