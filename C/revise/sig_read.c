#include "func.h"
int signal_hand(int signo)
{
    printf("start %d\n", signo);
    sleep(3);
    printf("end %d\n", signo);
}
int main()
{
    signal(SIGINT,SIG_IGN);
    // struct sigaction act;
    // act.sa_flags=SA_SIGINFO;
    // act.sa_handler=signal_hand;
    // int ret=sigaction(SIGINT,&act,NULL);
    // ERROR_CHECK(ret,-1,"sigaction")
    char buf[128]={0};
    int ret=read(STDIN_FILENO,buf,sizeof(buf));
    ERROR_CHECK(ret,-1,"read")
    printf("%s",buf);
    return 0;
}