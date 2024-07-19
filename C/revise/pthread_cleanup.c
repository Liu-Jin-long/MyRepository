#include <func.h>
void cleanup(void *p)
{
    free((char *)p);
    printf("clean up buf\n");
}

void *child_thread(void *p)
{
    printf("I am child thread\n");
    char *buf = (char *)malloc(sizeof(buf));
    pthread_cleanup_push(cleanup, p);
    strcpy(buf, "hello");
    printf("buf=%s\n", buf);
    
    
    return (void*)1;
    pthread_exit((void *)0);
    pthread_cleanup_pop(0);
    
}
int main()
{
    pthread_t pthid;
    pthread_create(&pthid, NULL, child_thread, NULL);
    int retvalue = -1;
    pthread_join(pthid, (void **)&retvalue);
    printf("retvalue=%ld\n", (long)retvalue);
    return 0;
}