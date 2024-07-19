#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include<time.h>
void sig_hand(int signo)
{
    printf("%d is coming!\n",signo);
}
int main()
{
    signal(SIGALRM,sig_hand);
    long start=time(NULL);
    alarm(3);
    printf("before pause\n");
    //pause();
    sleep(5);
    printf("finish pause\n");
    printf("耗时：%ld\n",time(NULL)-start);
    return 0;
}