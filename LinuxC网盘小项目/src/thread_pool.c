#include "thread_pool.h"
#include "operation.h"
int thread_pool_init(thread_pool_t *ptp, int thread_num, int capacity)
{
    bzero(ptp, sizeof(thread_pool_t));
    queue_init(&ptp->queue, capacity);
    int ret = pthread_cond_init(&ptp->cond, NULL);
    THREAD_ERR_CHECK(ret, "pthread_cond_init");
    ptp->pthid = (pthread_t *)calloc(thread_num, sizeof(pthread_t));
    ptp->start_flag = 0;
    return 0;
}

int thread_pool_start(thread_pool_t *ptp, int thread_num)
{
    if (!ptp->start_flag)
    {
        for (int i = 0; i < thread_num; i++)
        {
            pthread_create(ptp->pthid + i, NULL, thread_handle, ptp);
        }
        ptp->start_flag = 1;
    }
    return 0;
}

void cleanup(void *p)
{
    pthread_mutex_unlock((pthread_mutex_t *)p);
}
void *thread_handle(void *p)
{
    thread_pool_t *ptp = (thread_pool_t *)p;
    pQueue_t pq = &ptp->queue;
    pNode_t pcur = NULL;
    int get_task_sucess_flag;
    while (1)
    {
        pthread_mutex_lock(&pq->mutex);
        pthread_cleanup_push(cleanup, &pq->mutex);
        if (0 == pq->queue_size)
        {
            pthread_cond_wait(&ptp->cond, &pq->mutex); // 趁锁
        }
        queue_get(pq, &pcur);   // 获取新结点指针，不要在加减锁之间malloc
        pthread_cleanup_pop(1); // pthread_mutex_unlock(&pq->mutex);//直接通过pop来解锁
        if (pcur)               // 拿到结点
        {
            if (pcur->state == GETS)
            {
                // printf("服务器子线程进入GETS处理\n");
                do_gets(pcur);
            }
            else if (pcur->state == PUTS)
            {
                // printf("服务器子线程进入PUTS处理\n");
                do_puts(pcur);
            }
            close(pcur->sfd);
            free(pcur);
            pcur = NULL;
        }
    }
}