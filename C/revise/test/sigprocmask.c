// #include "func.h"
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
int main()
{
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set,SIGINT);
    sigprocmask(SIG_BLOCK,&set,NULL);
    signal(SIGINT,SIG_IGN);
    printf("关键代码开始：\n");
    sleep(5);
    sigset_t pending;
    sigemptyset(&pending);
    sigpending(&pending);
    if(sigismember(&pending,SIGINT))
    {
        printf("%d is pending!\n",SIGINT);
    }else{
        printf("%d is not pending!\n",SIGINT);
    }
    printf("关键代码结束：\n");
    sigprocmask(SIG_UNBLOCK,&set,NULL);
    printf("再次sleep开始\n");
    sleep(5);
    sigemptyset(&pending);
    sigpending(&pending);
    if(sigismember(&pending,SIGINT))
    {
        printf("%d is pending!\n",SIGINT);
    }else{
        printf("%d is not pending!\n",SIGINT);
    }
    printf("再次sleep结束\n");
    return 0;

}