#include "work_queue.h"

void queue_init(pQueue_t pq,int que_capacity)
{
    memset(pq,0,sizeof(Queue_t));
    pq->que_capacity=que_capacity;
    pthread_mutex_init(&pq->mutex,NULL);
}

void queue_set(pQueue_t pq,pNode_t pnew)
{
    if(0==pq->que_size)
    {
        pq->head=pnew;
        pq->tail=pnew;
        
    }else
    {
        pq->tail->next=pnew;
        pq->tail=pnew;
    }
    pq->que_size++;
}


void queue_get(pQueue_t pq,pNode_t* p)
{
    if (pq->que_size)
    {
        *p=pq->head;
        pq->head=pq->head->next;
        if(NULL==pq->head)
        {
            pq->tail=NULL;
        }
        pq->que_size--;
    }
   
}