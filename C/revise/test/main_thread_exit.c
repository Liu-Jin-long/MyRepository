#include <stdio.h>
#include<string.h>
#include<pthread.h>
#include <unistd.h>
#include<stdlib.h>
void* grandson_thread(void* v)
{
    sleep(1);
    printf("grandson_thread,%ld\n",*(long*)v);
    long* s=(long*)v;
    *s=777;
    sleep(2);
    printf("grandson_thread即将退出\n");
    pthread_exit((void*)333);
}
void* child_thread(void* v)
{
    printf("child_thread,%ld\n",*(long*)v);
    sleep(1);
    pthread_t pid;
    int ret =pthread_create(&pid,NULL,grandson_thread,v);
    printf("child_thread return (void*)222\n");
    //exit(222);
    //return (void*)222;
    pthread_exit((void*)222);
    printf("child_thread 已经return \n");
    //pthread_exit((void*)222);
}


int main()
{
    long source=666;
    pthread_t pid;
    int ret =pthread_create(&pid,NULL,child_thread,(void*)&source);
    sleep(2);
     printf("主线程准备 pthread_exit((void*)111)\n");
    //pthread_exit((void*)111);
    return 111;
    printf("主线程已经 pthread_exit((void*)111) source=%ld\n",source);
    return 111;
}
