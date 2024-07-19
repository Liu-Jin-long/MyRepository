#ifndef __WORK_QUE_H__
#define __WORK_QUE_H__
#include "head.h"
typedef struct tag_node
{
    int new_fd;
    struct tag_node *pnext;
} Node_t, *pNode_t;
typedef struct
{
    pNode_t que_head, que_tail;
    int que_capacity;
    int que_size;
    pthread_mutex_t mutex;
} Que_t, *pQue_t;
void que_init(pQue_t pq, int que_capacity);
void que_set(pQue_t pq, pNode_t pnew);
void que_get(pQue_t pq, pNode_t *p);

#endif