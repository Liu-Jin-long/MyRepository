#include <func.h>
void *pthread_func(void *p)
{
    int *q = (int *)p;
    *q = 100;
    printf("子线程：%d\n", *q);
}
int main()
{
    pthread_t pthid;
    int i = 10;
    pthread_create(&pthid, NULL, pthread_func, &i);
    sleep(1);
    printf("主线程：%d\n", i);
    return 0;
}