#include "work_que.h"
void que_init(pQue_t pq, int que_capacity)
{
    bzero(pq,sizeof(Que_t));
    pq->que_capacity=que_capacity;
    pthread_mutex_init(&pq->mutex,NULL);

}
void que_set(pQue_t pq, pNode_t pnew)
{
    if(NULL==pq->que_head)
    {
        pq->que_head=pq->que_tail=pnew;
    }else{
        pq->que_tail->pnext=pnew;
        pq->que_tail=pnew;
    }
    pq->que_size++;
}
void que_get(pQue_t pq, pNode_t *p)
{
    if(pq->que_size)
    {
        *p=pq->que_head;
        pq->que_head=pq->que_head->pnext;
        if(NULL==pq->que_head)
        {
            pq->que_tail=NULL;
        }
        pq->que_size--;
    }
}