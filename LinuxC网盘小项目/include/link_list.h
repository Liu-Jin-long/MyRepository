#ifndef __LINK_LIST_H__
#define __LINK_LIST_H__
#include "common.h"
//客户端信息链表结点
typedef struct LNode
{
	client_info_t client_info;
	struct LNode *next;
} LNode, *LinkList;
void link_list_head_insert(LinkList *head, client_info_t data);
void link_list_delete_node(LinkList *head, int sfd);
//轮盘法超时链表
typedef struct LNode_Timeout
{
	int sfd;
	struct LNode_Timeout *next;
} LNode_Timeout, *LinkList_Timeout;
void roulette_link_list_head_insert(LinkList_Timeout *head, int sfd);
void roulette_link_list_delete_node(LinkList_Timeout *head, int sfd);
void roulette_delete_link_list(LinkList_Timeout *timeout_list_head, LinkList *head);
void roulette_update_and_check(LinkList_Timeout *roulette, LinkList *head, int *index, time_t *p_past, int sfd);
#endif
