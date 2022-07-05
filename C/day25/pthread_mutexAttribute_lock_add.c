#include <func.h>
//通过设置线程锁属性，用mutex实现两个进程各加2千万，最终实现4千万
typedef struct{
    int num;
    pthread_mutex_t mutex;
}data;

int main()
{
    int N=20000000;
    data t;
    int shmid=shmget(666,4096,IPC_CREAT|0600);
    ERROR_CHECK(shmid,-1,"shmget");
    printf("shmid=%d\n",shmid);

    data *p=shmat(shmid,NULL,0);
    ERROR_CHECK(p,(char*)-1,"shmid");
    p->num=0;
    pthread_mutexattr_t mutex_attr;//定义一个锁属性
    pthread_mutexattr_init(&mutex_attr);//初始化所属性
    pthread_mutexattr_setpshared(&mutex_attr,PTHREAD_PROCESS_SHARED);
    int ret=pthread_mutex_init(&p->mutex,&mutex_attr);

    int i;
    if(!fork())
    {
        for(i=0;i<N;i++)
        {
            pthread_mutex_lock(&p->mutex);
            p->num++;
            pthread_mutex_unlock(&p->mutex);
        }
        return 0;

    }
    else
    {
       for(i=0;i<N;i++)
        {
            pthread_mutex_lock(&((data*)p)->mutex);
            p->num++;
            pthread_mutex_unlock(&((data*)p)->mutex);
        } 
        wait(NULL);
        printf("result=%d\n",p->num);
        return 0;
    }


}