#include <func.h>
//模拟火车站买票，两个子线程卖票，一个子线程放票
typedef struct{
    int tickets;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
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
            if(0==d->tickets)
            {
                pthread_cond_signal(&d->cond);
            }
            pthread_mutex_unlock(&d->mutex);
            printf("sale_window1 has finished to sale tickets. %d\n",d->tickets);
        }else{
            pthread_mutex_unlock(&d->mutex);
            break;
        }
        sleep(1);
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
            if(0==d->tickets)
            {
                pthread_cond_signal(&d->cond);
            }
            pthread_mutex_unlock(&d->mutex);
            printf("sale_window2 has finished to sale tickets.%d\n",d->tickets);
        }else{
            pthread_mutex_unlock(&d->mutex);
            break;
        }
        sleep(1);
    }   
}

void* add_adjust_window3(void *q)
{
    data* p=(data*)q;
    pthread_mutex_lock(&p->mutex);
    if(p->tickets>0) pthread_cond_wait(&p->cond,&p->mutex);
    p->tickets=10;
    pthread_mutex_unlock(&p->mutex);
    printf("window3 has added 10 tickets.\n");
}
 int main()
 {
    data d;
    d.tickets=20;
    int ret=pthread_mutex_init(&d.mutex,NULL);
    THREAD_ERR_CHECK(ret,"pthread_mutex_init");
    
    ret=pthread_cond_init(&d.cond,NULL);
    THREAD_ERR_CHECK(ret,"pthread_cond_init");

    pthread_t pth_id[3];
    
    ret=pthread_create(&pth_id[2],NULL,add_adjust_window3,(void*)&d);
    THREAD_ERR_CHECK(ret,"pthread_create");
    sleep(1);
    ret=pthread_create(&pth_id[0],NULL,sale_window1,(void*)&d);
    THREAD_ERR_CHECK(ret,"pthread_create");
    ret=pthread_create(&pth_id[1],NULL,sale_window2,(void*)&d);
    THREAD_ERR_CHECK(ret,"pthread_create");
    

    ret=pthread_join(pth_id[0],NULL);
    THREAD_ERR_CHECK(ret,"pthread_join");
    ret=pthread_join(pth_id[1],NULL);
    THREAD_ERR_CHECK(ret,"pthread_join");
    ret=pthread_join(pth_id[2],NULL);
    THREAD_ERR_CHECK(ret,"pthread_join");


    printf("main thread tickets=%d\n",d.tickets);
    ret=pthread_mutex_destroy(&d.mutex);
    THREAD_ERR_CHECK(ret,"pthread_mutex_destroy");
    return 0;
 }