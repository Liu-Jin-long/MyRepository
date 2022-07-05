#include "HeadFile.h"

void main()
{
	BiTree root = NULL;
	QNode* QHead = NULL, *QTail = NULL;
	ElemType data;
	while (scanf("%c", &data) != EOF)
	{
		if (data == '\n') break;
		Build_BiTree(&root, &QHead, &QTail, data);
	}
	PreOrder(root);
	putchar('\n');
	InOrder(root);
	putchar('\n');
	PostOrder(root);
	

}