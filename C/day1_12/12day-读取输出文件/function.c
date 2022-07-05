#include "HeadFile.h"


void print_linkList(LinkList L)
{
	while (L)
	{
		printf("%d ", L->Total_Score);
		L = L->next;
	}
	printf("\n");
}

void print(LinkList head,FILE* fp)
{
	while (head)
	{
		fprintf(fp, "%d    %s    %c    %d    %d    %d\n", head->stu.num, head->stu.name, head->stu.sex, head->stu.course1, head->stu.course2, head->stu.course3);
		head = head->next;
	}
}


void Order_Insert(LinkList* head, LinkList* tail, int num, char name[], char sex, int course1, int course2, int course3,int Total_Score)
{
	LNode* new_insert = (LNode*)calloc(1, sizeof(LNode));
	new_insert->Total_Score = Total_Score;
	new_insert->stu.num = num;
	strcpy(new_insert->stu.name, name);
	new_insert->stu.sex = sex;
	new_insert->stu.course1 = course1;
	new_insert->stu.course2 = course2;
	new_insert->stu.course3 = course3;

	new_insert->next = NULL;
	LNode* pre = *head, * p = *head;
	if (p == NULL)//此时无结点，新结点作为头尾结点
	{
		*head = *tail = new_insert;
	}
	else if (new_insert->Total_Score >= p->Total_Score)
	{
		new_insert->next = p;
		*head = new_insert;
	}
	else
	{
		while (p)
		{
			if (new_insert->Total_Score < p->Total_Score)//新结点值小于工作指针值
			{
				pre = p;
				p = p->next;
			}
			else//第一次遇到大于或等于
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