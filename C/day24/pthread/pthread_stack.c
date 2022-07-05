#include <func.h>
//创建一个子线程，在子线程中使用main的栈空间，改变数据，主线程打印

void* pthread_func(void *q)
{
    int* j=(int*)q;
    *j=1;
    printf("child thread\n");
}
int main()
{
    pthread_t thread;
    int i=666;

    int ret=pthread_create(&thread,NULL,pthread_func,&i);//第四个参数是给pthread_func传递的参数
    if(ret!=0)//成功返回0，失败返回错误码
    {
        printf("pthread_create failed");
        return -1;
    }
    usleep(1);
    printf("main threaad\n");
    printf("gets=%d\n",i);
    return 0;

}