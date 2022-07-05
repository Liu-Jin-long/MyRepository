#include <func.h>
//子线程，主线程，同时对一个变量加2千万，通过加锁，实现最终效果是4千万
#define N 20000000
typedef struct{
    int num;
    pthread_mutex_t mutex;
}data;

void* pthread_func(void* p)
{
    for(int i=0;i<N;i++)
    {
        pthread_mutex_lock(&((data*)p)->mutex);
        ((data*)p)->num++;
        pthread_mutex_unlock(&((data*)p)->mutex);

    }
    return NULL;
}

int main()
{
    pthread_t pth_id;
    data d;
    d.num=0;
    int ret=pthread_mutex_init(&d.mutex,NULL);//初始化锁
    THREAD_ERR_CHECK(ret,"pthread_mutex_init");

    ret=pthread_create(&pth_id,NULL,pthread_func,(void*)&d);
    THREAD_ERR_CHECK(ret,"pthread_create");
    for(int i=0;i<N;i++)
    {
        pthread_mutex_lock(&d.mutex);
        d.num++;
        pthread_mutex_unlock(&d.mutex);
    }
    ret=pthread_join(pth_id,NULL);
    THREAD_ERR_CHECK(ret,"pthread_join");
    printf("result=%d\n",d.num);
    ret=pthread_mutex_destroy(&d.mutex);//销毁锁
    THREAD_ERR_CHECK(ret,"pthread_mutex_destroy");
    return 0;
}
