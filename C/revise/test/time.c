// #include <func.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
//使用真实计时器，实用计时器，虚拟计时器，统计程序执行时间，在实用计时器及虚拟计时器设定计时后，先睡眠，再让程序处于while（1）

void sigfunc(int signum)
{
    time_t now = time(NULL);
    printf("Now=%s", ctime(&now));
}
int main()
{
    // signal(SIGALRM,sigfunc);
    // signal(SIGPROF,sigfunc);
    signal(SIGVTALRM, sigfunc);
    struct itimerval t;
    bzero(&t, sizeof(t));
    t.it_value.tv_sec = 3;
    t.it_interval.tv_sec = 2;
    time_t now = time(NULL);
    printf("Now=%s", ctime(&now));
    // int ret=setitimer(ITIMER_REAL,&t,NULL);//真实计时器，程序全程运行的实际时间
    // int ret=setitimer(ITIMER_PROF,&t,NULL);//实用计时器，程序处于用户态和内核态消耗时间之和(不包含sleep、系统调用阻塞)
    int ret = setitimer(ITIMER_VIRTUAL, &t, NULL); //虚拟计时器，程序在用户态时消耗的时间

    // ERROR_CHECK(ret,-1,"setitimer");
    char buf[20] = {0};
    read(STDIN_FILENO, buf, sizeof(buf));
    printf("接收到：%s", buf);
    printf("开始sleep 5s\n");
    time_t start = time(NULL);
    sleep(5);
    time_t end = time(NULL);
    printf("sleep 5s后, 耗时%lds 现在时间%s", end - start, ctime(&end));
    while (1)
        ;
    return 0;
}