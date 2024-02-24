#ifndef __WORK_QUEUE_H__
#define __WORK_QUEUE_H__
#include "common.h"
//队列结点
typedef struct tag_node
{
    int sfd;
    const client_info_t *p_client_info;
    int state;
    char buf[DATA_LEN];
    struct tag_node *pnext;
} Node_t, *pNode_t;
//队列
typedef struct
{
    pNode_t queue_head, queue_tail; //队列头节点和尾节点
    int queue_capacity;             //队列容量
    int queue_size;                 //队列大小
    pthread_mutex_t mutex;          //队列锁
} Queue_t, *pQueue_t;
void queue_init(pQueue_t pq, int queue_capacity);
void queue_set(pQueue_t pq, pNode_t pnew);
void queue_get(pQueue_t pq, pNode_t *p);
#endif