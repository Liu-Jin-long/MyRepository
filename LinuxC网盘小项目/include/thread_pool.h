#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__
#include "common.h"
#include "work_queue.h"
#include "transfer_file.h"
typedef struct
{
    Queue_t queue;
    pthread_cond_t cond;
    pthread_t *pthid;
    short start_flag; //线程池启动标志
} thread_pool_t;

//线程池初始化
int thread_pool_init(thread_pool_t *pf, int thread_num, int capacity);
//线程池启动
int thread_pool_start(thread_pool_t* pf, int thread_num);
void *thread_handle(void *p);
#endif