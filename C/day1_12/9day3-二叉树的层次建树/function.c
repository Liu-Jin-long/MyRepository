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
		(*tail)->next = QueueNew;//入队
		*tail = QueueNew;
		if ((*head)->bi_node->lchild == NULL)//队首结点保存的树结点（地址）的左孩子可以插入
		{
			(*head)->bi_node->lchild = TreeNew;

		}
		else
		{
			if ((*head)->bi_node->rchild == NULL)//队首结点保存的树结点（地址）的右孩子可以插入
			{
				(*head)->bi_node->rchild = TreeNew;
				QNode* temp = (*head)->next;//队首结点出队
				free(*head);
				*head = temp;
				temp = NULL;
			}
		}
	}
}


void PreOrder(BiTree T) //先序遍历
{
	if (T)
	{
		putchar(T->data);
		PreOrder(T->lchild);
		PreOrder(T->rchild);
	}
}
void InOrder(BiTree T) //中序遍历
{
	if (T)
	{
		InOrder(T->lchild);
		putchar(T->data);
		InOrder(T->rchild);
	}
}
void PostOrder(BiTree T)//后序遍历
{
	if (T) 
	{
		PostOrder(T->lchild);
		PostOrder(T->rchild);
		putchar(T->data);
	}
	
}