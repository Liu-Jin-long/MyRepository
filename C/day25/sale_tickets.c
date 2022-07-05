#include <func.h>
//编写火车站买票，两个子线程卖票，确保没有卖负票
typedef struct{
    int tickets;
    pthread_mutex_t mutex;
}data;

void* sale_window1(void *q)
{
    data* d=(data*)q;
    
    while(1)
    {
        pthread_mutex_lock(&d->mutex);
        if(d->tickets>0)
        {
            printf("sale_window1 start to sale tickets. %d\n",d->tickets);
            d->tickets--;
            pthread_mutex_unlock(&d->mutex);
            printf("sale_window1 has finished to sale tickets. %d\n",d->tickets);
        }else{
            pthread_mutex_unlock(&d->mutex);
            break;
        }
    }
}   
 void* sale_window2(void *q)
{
    data* d=(data*)q;
    
    while(1)
    {
        pthread_mutex_lock(&d->mutex);
        if(d->tickets>0)
        {
            printf("sale_window2 start to sale tickets. %d\n",d->tickets);
            d->tickets--;
            pthread_mutex_unlock(&d->mutex);
            printf("sale_window2 has finished to sale tickets.%d\n",d->tickets);
        }else{
            pthread_mutex_unlock(&d->mutex);
            break;
        }
    }   
}
 int main()
 {
    data d;
    d.tickets=20;
    int ret=pthread_mutex_init(&d.mutex,NULL);
    THREAD_ERR_CHECK(ret,"pthread_mutex_init");
    
    pthread_t pth_id[2];
    ret=pthread_create(&pth_id[0],NULL,sale_window1,(void*)&d);
    THREAD_ERR_CHECK(ret,"pthread_create");
    ret=pthread_create(&pth_id[1],NULL,sale_window2,(void*)&d);
    THREAD_ERR_CHECK(ret,"pthread_create");

    ret=pthread_join(pth_id[0],NULL);
    ret=pthread_join(pth_id[1],NULL);

    printf("main thread tickets=%d\n",d.tickets);
    ret=pthread_mutex_destroy(&d.mutex);
    THREAD_ERR_CHECK(ret,"pthread_mutex_destroy");
    return 0;
 }