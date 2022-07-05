#include "HeadFile.h"




void main()
{
	LinkList head = NULL, tail = NULL;
	ElemType data;
	while (scanf("%d", &data) != EOF)
	{
		/*Tail_Insert(&head, &tail, data);
		Head_Insert(&head, &tail, data);*/
		Order_Insert(&head, &tail, data);
		print_linkList(head);
	}
	
	
	while (scanf("%d", &data) != EOF)
	{
		LNode_Delete(&head, &tail, data);
		print_linkList(head);
	}
}