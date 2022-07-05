#include <func.h>
//启动子线程，处于wait状态，主线程signal唤醒子线程
typedef struct 
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}data; 

void* thread_func(void* q)
{
    data* p=(data*)q;
    pthread_mutex_lock(&p->mutex);//wait前后要加锁解锁
    int ret=pthread_cond_wait(&p->cond,&p->mutex);//子线程等待在对应的条件变量上
    pthread_mutex_unlock(&p->mutex);

    printf("child thread ends to wait and start to execute.ret=%d\n",ret);
    pthread_exit(NULL);
}

int main()
{
    data t;
    int ret=pthread_cond_init(&t.cond,NULL);
    THREAD_ERR_CHECK(ret,"pthread_cond_init");
    ret=pthread_mutex_init(&t.mutex,NULL);
    THREAD_ERR_CHECK(ret,"pthread_mutex_init");
    
    pthread_t pth_id;
    pthread_create(&pth_id,NULL,thread_func,(void*)&t);

    sleep(1);
    ret=pthread_cond_signal(&t.cond);//要保证在signal发生在wait之后，否则两线程都一直等待
    THREAD_ERR_CHECK(ret,"pthread_cond_signal");

    ret=pthread_join(pth_id,NULL);
    THREAD_ERR_CHECK(ret,"pthread_join");

    printf("main thread.\n");
    return 0;


}