#include "work_queue.h"
//任务队列初始化
void queue_init(pQueue_t pq, int queue_capacity)
{
    bzero(pq,sizeof(Queue_t));
    pq->queue_capacity=queue_capacity;
    pthread_mutex_init(&pq->mutex,NULL);

}
//任务队列添加任务
void queue_set(pQueue_t pq, pNode_t pnew)
{
    if(NULL==pq->queue_head)
    {
        pq->queue_head=pq->queue_tail=pnew;
    }else{
        pq->queue_tail->pnext=pnew;
        pq->queue_tail=pnew;
    }
    pq->queue_size++;
}
//任务队列取出任务
void queue_get(pQueue_t pq, pNode_t *p)
{
    if(pq->queue_size)
    {
        *p=pq->queue_head;
        pq->queue_head=pq->queue_head->pnext;
        if(NULL==pq->queue_head)
        {
            pq->queue_tail=NULL;
        }
        pq->queue_size--;
    }
}