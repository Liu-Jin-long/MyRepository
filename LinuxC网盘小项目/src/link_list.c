#include "link_list.h"
#include "common.h"
#include "tcp_epoll_op.h"
// 客户端信息链表头插法插入结点
void link_list_head_insert(LinkList *head, client_info_t data)
{
	LNode *p = (LNode *)calloc(1, sizeof(LNode));
	// p->data = data;
	memcpy(&p->client_info, &data, sizeof(client_info_t));
	p->next = NULL;
	if (*head == NULL)
	{
		*head = p;
	}
	else
	{
		p->next = *head;
		*head = p;
	}
}
// 客户端信息链表删除结点
void link_list_delete_node(LinkList *head, int sfd)
{
	LNode *p, *pre;
	p = pre = *head;
	if (*head == NULL)
	{
		printf("链表已为空");
		return;
	}
	if (p->client_info.sfd == sfd) // 删除头结点
	{
		*head = p->next;
		free(p);
		p = pre = NULL;
		return;
	}
	else
	{
		while (p)
		{
			if (p->client_info.sfd == sfd) // 若删除尾结点，pre->next=NULL;
			{
				pre->next = p->next;
				free(p);
				break;
			}
			pre = p;
			p = p->next;
		}
		p = pre = NULL;
	}
}
// 轮盘链表头插法插入结点
void roulette_link_list_head_insert(LinkList_Timeout *head, int sfd)
{
	LNode_Timeout *p = (LNode_Timeout *)calloc(1, sizeof(LNode_Timeout));
	p->sfd = sfd;
	p->next = NULL;
	if (*head == NULL)
	{
		*head = p;
	}
	else
	{
		p->next = *head;
		*head = p;
	}
}
// 轮盘所有链表删除相应结点
void roulette_link_list_delete_node(LinkList_Timeout *roulette, int sfd)
{
	LNode_Timeout *p, *pre;
	for (int i = 0; i < TIMEOUT_DISCONNECT_SECOND; ++i)
	{
		p = pre = *(roulette + i);
		if (*(roulette + i) == NULL)
		{
			continue;
		}
		if (p->sfd == sfd) // 删除头结点
		{
			*(roulette + i) = p->next;
			free(p);
			p = pre = NULL;
			continue;
		}
		else
		{
			while (p)
			{
				if (p->sfd == sfd) // 若删除尾结点，pre->next=NULL;
				{
					pre->next = p->next;
					free(p);
					p = pre = NULL;
					continue;
				}
				pre = p;
				p = p->next;
			}
			p = pre = NULL;
		}
	}
}
// 轮盘超时删除一个链表
void roulette_delete_link_list(LinkList_Timeout *timeout_list_head, LinkList *head, int epfd)
{
	LNode_Timeout *p, *tmp;
	p = tmp = *timeout_list_head;
	while (p)
	{
		if (p)
		{
			link_list_delete_node(head, p->sfd);
			// 取消监控
			epoll_delete(epfd, p->sfd);
			// printf("epoll_delete %d\n", p->sfd);
			close(p->sfd);
			tmp = p->next;
			free(p);
			p = tmp;
		}
	}
	*timeout_list_head = NULL;
}
// 更新轮盘 检查流逝时间
void roulette_update_and_check(LinkList_Timeout *roulette, LinkList *head, int *index, time_t *p_past, int epfd, int sfd)
{
	if (sfd != -1)
	{
		// 删除轮盘中存在的该sfd
		roulette_link_list_delete_node(roulette, sfd);
		// 插入到当前轮盘索引的链表
		roulette_link_list_head_insert(roulette + *index, sfd);
	}
	// 检查当前时间是否走了一秒
	time_t now = time(NULL);
	if (now - *p_past >= 1)
	{
		*p_past = now;
		(*index)++;
		if (*index >= TIMEOUT_DISCONNECT_SECOND)
		{
			*index = 0;
		}
		roulette_delete_link_list(roulette + *index, head, epfd);
	}
	// 打印
	if (0 == *index % 10)
	{

		printf("当前时间=%ld 轮盘当前索引:%d\n", time(NULL), *index);
		LNode_Timeout *p;
		for (int i = 0; i < TIMEOUT_DISCONNECT_SECOND; i++)
		{
			p = *(roulette + i);
			while (p)
			{

				printf("轮盘索引%d有fd=%d  ", i, p->sfd);
				p = p->next;
			}
		}
		printf("\n");
	}
}