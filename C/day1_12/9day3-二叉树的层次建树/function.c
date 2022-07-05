#include "HeadFile.h"


void Build_BiTree(BiTree* root, QNode** head, QNode** tail,ElemType data)
{
	BiNode * TreeNew= (BiNode*)calloc(1, sizeof(BiNode));
	TreeNew->data = data;
	QNode* QueueNew = (QNode*)calloc(1, sizeof(QNode));
	QueueNew->bi_node = TreeNew;
	if (*root == NULL)
	{
		*root = TreeNew;
		*head = QueueNew;
		*tail = QueueNew;
	}
	else
	{
		(*tail)->next = QueueNew;//���
		*tail = QueueNew;
		if ((*head)->bi_node->lchild == NULL)//���׽�㱣�������㣨��ַ�������ӿ��Բ���
		{
			(*head)->bi_node->lchild = TreeNew;

		}
		else
		{
			if ((*head)->bi_node->rchild == NULL)//���׽�㱣�������㣨��ַ�����Һ��ӿ��Բ���
			{
				(*head)->bi_node->rchild = TreeNew;
				QNode* temp = (*head)->next;//���׽�����
				free(*head);
				*head = temp;
				temp = NULL;
			}
		}
	}
}


void PreOrder(BiTree T) //�������
{
	if (T)
	{
		putchar(T->data);
		PreOrder(T->lchild);
		PreOrder(T->rchild);
	}
}
void InOrder(BiTree T) //�������
{
	if (T)
	{
		InOrder(T->lchild);
		putchar(T->data);
		InOrder(T->rchild);
	}
}
void PostOrder(BiTree T)//�������
{
	if (T) 
	{
		PostOrder(T->lchild);
		PostOrder(T->rchild);
		putchar(T->data);
	}
	
}