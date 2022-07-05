#ifndef __WORK_QUEUE_H__
#define __WORK_QUEUE_H__
#include "head.h"

typedef struct fd_node
{
    int new_fd;
    struct fd_node* next;
}Node_t,*pNode_t;

typedef struct 
{
    pNode_t head,tail;
    int que_capacity;//队列容量
    int que_size;//队列当前大小
    pthread_mutex_t mutex;
}Queue_t,*pQueue_t;

void queue_init(pQueue_t pq,int que_capacity);
void queue_set(pQueue_t pq,pNode_t pnew);
void queue_get(pQueue_t pq,pNode_t* p);

#endif