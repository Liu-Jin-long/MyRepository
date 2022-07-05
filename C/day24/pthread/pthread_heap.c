#include <func.h>
//创建一个子线程，在子线程中使用堆空间，改变数据，主线程打印
void* pthread_func(void *q)
{
    strcpy(q,"hello");
    printf("child thread\n");
}
int main()
{
    pthread_t thread;
    char* p=(char*)malloc(64);

    int ret=pthread_create(&thread,NULL,pthread_func,p);//第四个参数是给pthread_func传递的参数
    if(ret!=0)//成功返回0，失败返回错误码
    {
        printf("pthread_create failed");
        return -1;
    }
    usleep(1);
    printf("main threaad\n");//防止子线程还没有创建成功，主线程就关闭了标准输出
    printf("gets=%s\n",p);
    return 0;

}