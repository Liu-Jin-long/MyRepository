#include "func.h"
int main()
{
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
    printf("再次sleep开始\n");
    sleep(5);
    printf("再次sleep结束\n");
    return 0;

}