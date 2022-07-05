#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef char ElemType;

typedef struct BiNode
{
	ElemType data;
	struct BiNode* lchild, * rchild;
}BiNode, * BiTree;

typedef struct QNode
{
	BiNode* bi_node;
	struct QNode* next;
}QNode,*Queue;

void Build_BiTree(BiTree* root, QNode** head, QNode** tail, ElemType data);
void PreOrder(BiTree T);
void InOrder(BiTree T);
void PostOrder(BiTree T);