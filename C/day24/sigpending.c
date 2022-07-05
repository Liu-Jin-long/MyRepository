#include <func.h>
//sigaction实现2号信号，屏蔽3号信号的执行效果，同时在2号信号的信号处理流程中，判断3号信号是否被挂起
void signal_func(int signal_num,siginfo_t *p,void *p1)//自定义信号处理函数
{
    printf("\n before,signal num is %d\n",signal_num);
    sleep(3);
    sigset_t pending;
    sigpending(&pending);
    
    if(sigismember(&pending,SIGQUIT))
    {
        printf("SIGQUIT is pending.\n");
    }else{
        printf("SIGQUIT isn't pending.\n");
    }
    printf("\n after,signal num is %d\n",signal_num);

}

int main()
{
    struct sigaction act;
    bzero(&act,sizeof(act));
    act.sa_sigaction=signal_func;
    act.sa_flags=SA_SIGINFO;
    sigemptyset(&act.sa_mask);//清空掩码集合
    sigaddset(&act.sa_mask,SIGQUIT);
    int ret=sigaction(SIGINT,&act,NULL);
    ERROR_CHECK(ret,-1,"sigaction");
    while(1);
    return 0;


}