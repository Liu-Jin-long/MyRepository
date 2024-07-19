#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
void signal_hand(int signo)
{
    printf("start %d\n", signo);
    sleep(3);
    printf("end %d\n", signo);
}
int main()
{
    int ret;
    signal(SIGINT,signal_hand);
    // struct sigaction act;
    // bzero(&act, sizeof(act));
    // act.sa_flags = SA_SIGINFO;
    // act.sa_sigaction=signal_hand;
    //ret = sigaction(SIGQUIT, &act, NULL);
    //ret=sigaction(SIGINT,&act,NULL);
    char buf[128]={0};
    ret=read(STDIN_FILENO,buf,sizeof(buf));
    printf("%sjjjj%d",buf,ret);
    return 0;
}
