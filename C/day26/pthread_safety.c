#include <func.h>
//编写代码，通过ctime感受线程安全
void* thread_func(void* q)
{
    time_t now;
    time(&now);
    char* p=ctime(&now);
    char t[100];
    ctime_r(&now,t);
    printf("child  p=%p ctime=%s",p,p);
    printf("child  t=%p ctime_r=%s\n",t,t);
    sleep(3);
    printf("child  p=%p second_ctime=%s",p,p);
    printf("child  t=%p second_ctime_r=%s\n",t,t);

    pthread_exit(NULL);
}
//ctime不是线程安全函数,p指向的是静态局部变量
//ctime_r是线程安全函数
int main()
{
    
    pthread_t pth_id;
    int ret=pthread_create(&pth_id,NULL,thread_func,NULL);
    THREAD_ERR_CHECK(ret,"pthread_create");
    sleep(1);

    time_t now;
    time(&now);
    char t[100];
    char* p=ctime(&now);
    ctime_r(&now,t);
    printf("main  p=%p ctime=%s",p,p);
    printf("main  t=%p ctime_r=%s\n",t,t);
    ret=pthread_join(pth_id,NULL);
    THREAD_ERR_CHECK(ret,"pthread_join");

    return 0;

}