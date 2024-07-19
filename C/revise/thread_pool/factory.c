#include "factory.h"
int factory_init(Factory_t *pf, int thread_num, int capacity)
{
    bzero(pf, sizeof(Factory_t));
    que_init(&pf->que, capacity);
    int ret = pthread_cond_init(&pf->cond, NULL);
    THREAD_ERR_CHECK(ret, "pthread_cond_init");
    pf->pthid = (pthread_t *)calloc(thread_num, sizeof(pthread_t));
    pf->start_flag = 0;
    return 0;
}

int factory_start(Factory_t *pf, int thread_num)
{
    if (!pf->start_flag)
    {
        for (int i = 0; i < thread_num; i++)
        {
            pthread_create(pf->pthid + i, NULL, thread_handle, pf);
        }
        pf->start_flag = 1;
    }
    return 0;
}

void cleanup(void *p)
{
    pthread_mutex_unlock((pthread_mutex_t *)p);
}
void *thread_handle(void *p)
{
    Factory_t *pf = (Factory_t *)p;
    pQue_t pq = &pf->que;
    pNode_t pcur = NULL;
    int get_task_sucess_flag;
    while (1)
    {
        pthread_mutex_lock(&pq->mutex);
        pthread_cleanup_push(cleanup, &pq->mutex);
        if (0 == pq->que_size)
        {
            pthread_cond_wait(&pf->cond, &pq->mutex); //趁锁
        }
        que_get(pq, &pcur);     //获取新结点指针，不要在加减锁之间malloc
        pthread_cleanup_pop(1); // pthread_mutex_unlock(&pq->mutex);//直接通过pop来解锁
        if (pcur)              //拿到结点
        {
            transfer_file(pcur->new_fd);
            free(pcur);
            pcur = NULL;
        }
    }
}