#include <func.h>
//创建一个子线程，传入数值1，在子线程中能够获取并打印，子线程退出，返回数值2，主线程通过pthread_join获取等待子线程结束并获取子线程的退出值并打印

void* pthread_func(void* p)
{
    //printf("child thread,get=%d\n",*((int*)p));
    printf("child thread,get=%ld\n",(long)p);
    
    //return (void*)2;
    pthread_exit((void*)2);

}

int main()
{
    pthread_t pth_id;
    //int i=1;
    //int ret=pthread_create(&pth_id,NULL,pthread_func,(void*)&i);
    long i=1;
    int ret=pthread_create(&pth_id,NULL,pthread_func,(void*)i);
    THREAD_ERR_CHECK(ret,"pthread_create");

    long thread_return;//定义一个long型，不能是int型
    ret=pthread_join(pth_id,(void**)&thread_return);//用指针传递一个整型数据 *(&thread_return)=(void*)2
    printf("child_thread return value=%ld\n",thread_return);

    return 0;
    
}