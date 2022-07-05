#include <func.h>
//创建一个子线程，主线程cancel子线程

void* pthread_func(void* p)
{
    printf("child thread\n");
    sleep(1);

    printf("you can't see me.\n");
    return NULL;
}
int main()
{
    pthread_t pth_id;
    int ret=pthread_create(&pth_id,NULL,pthread_func,NULL);
    THREAD_ERR_CHECK(ret,"pthread_create");

    ret=pthread_cancel(pth_id);
    THREAD_ERR_CHECK(ret,"pthread_cancel");

    ret=pthread_join(pth_id,NULL);//等待子线程
    THREAD_ERR_CHECK(ret,"pthread_join");

    printf("I'm main thread.\n");
    return 0;


}