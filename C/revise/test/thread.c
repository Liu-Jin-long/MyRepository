#include <pthread.h>
#include<stdio.h>
#include<malloc.h>
#include <unistd.h>
void cleanup(void* p)
{
    free(p);
    printf("p已被free\n");
}
void* handle_func(void* pArgs)
{
    char* p=(char*)malloc(10);
    pthread_cleanup_push(cleanup,p);
    printf("子线程得到参数%ld\n",(long)pArgs);
    for(int i=0;i<10;i++)
    {
        printf("子线程第%d次循环\n",i);
        sleep(1);
    }
    pthread_exit((void*)3);
    pthread_cleanup_pop(0);
}
int main()
{
    pthread_t pth_id;
    pthread_create(&pth_id,NULL,handle_func,(void*)666);
    for(int i=0;i<5;i++)
    {
        printf("主线程第%d次循环\n",i);
        sleep(1);
        if(i==3)
        {
            pthread_cancel(pth_id);
            printf("pth_id %ld 已被取消\n",pth_id);
        }
    }
    long pth_ret;
    pthread_join(pth_id,(void**)&pth_ret);
    printf("子线程返回值%ld\n",pth_ret);
    return 0;

}