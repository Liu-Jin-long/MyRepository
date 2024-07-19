#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
// #include<func.h>
int count=0;
void signal_hand(int signo,siginfo_t* pSignInfo,void* pReserved)
{
    count++;
    printf("第%d次  start %d号信号 \n", count,signo);
    sleep(2);
    printf("第%d次  end %d号信号 \n", count, signo);
}
int main()
{
    int ret;

    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset,SIGQUIT);
    sigaddset(&sigset,SIGINT);
    sigprocmask(SIG_BLOCK,&sigset,NULL);

    struct sigaction act;
    bzero(&act, sizeof(act));
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction=signal_hand;
    ret = sigaction(SIGQUIT, &act, NULL);
    //ret = sigaction(SIGINT, &act, NULL);
    // sigemptyset(&act.sa_mask);
    // sigaddset(&act.sa_mask,SIGQUIT);
    int i=0;
    sigset_t pending_mask;
    bzero(&pending_mask, sizeof(act));
    while(1)
    {
        sigpending(&pending_mask);
        if(sigismember(&pending_mask,SIGINT))
        {
            printf("SIGINT is pending!\n");
        }
        if(sigismember(&pending_mask,SIGQUIT))
        {
            printf("SIGQUIT is pending!\n");
        }
        if(i==3)
        {
            printf("不再阻塞SIGQUIT\n");
            sigemptyset(&sigset);
            sigaddset(&sigset,SIGQUIT);
            sigprocmask(SIG_UNBLOCK,&sigset,NULL);
        }else if(i==5){
            printf("不再阻塞SIGINT\n");
            sigemptyset(&sigset);
            sigaddset(&sigset,SIGINT);
            sigprocmask(SIG_UNBLOCK,&sigset,NULL);
        };
        sleep(2);
        i++;
    };
    return 0;

}