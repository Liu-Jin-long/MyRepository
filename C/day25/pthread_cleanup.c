#include <func.h>
//创建一个子线程，cancel子线程，在子线程中push两个清理函数，感受清理函数的执行顺序
void cleanup(void *p)
{
    free(p);
    printf("cleanup function executed.\n");
}

void* pthread_func(void* p)
{
    char* q=(char*)malloc(64);
    pthread_cleanup_push(cleanup,q);
    printf("malloc\n");
    
    sleep(1);

    printf("free will start.\n");
    //pthread_exit(NULL);//线程退出时会检查清理函数栈内是否有清理函数
    pthread_cleanup_pop(0);//pop(1)弹出清理函数并执行，pop(0)只弹出清理函数而不执行

    return NULL;
}
int main()
{
    pthread_t pth_id;
    int ret=pthread_create(&pth_id,NULL,pthread_func,NULL);
    THREAD_ERR_CHECK(ret,"pthread_create");

    // ret=pthread_cancel(pth_id);//取消线程时会检查清理函数栈内是否有清理函数
    // THREAD_ERR_CHECK(ret,"pthread_cancel");

    ret=pthread_join(pth_id,NULL);//等待子线程
    THREAD_ERR_CHECK(ret,"pthread_join");

    printf("I'm main thread.\n");
    return 0;


}