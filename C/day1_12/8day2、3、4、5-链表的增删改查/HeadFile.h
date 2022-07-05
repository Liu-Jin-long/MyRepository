#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef int ElemType;

typedef struct LNode//Á´±í½áµã
{
	ElemType data;
	struct LNode* next;
}LNode, * LinkList;

void print_linkList(LinkList L);
void Tail_Insert(LinkList* head, LinkList* tail, ElemType data);
void Head_Insert(LinkList* head, LinkList* tail, ElemType data);
void LNode_Delete(LinkList* head, LinkList* tail, ElemType data);
void Order_Insert(LinkList* head, LinkList* tail, ElemType data);
