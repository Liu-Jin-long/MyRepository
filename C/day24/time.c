#include <func.h>
//使用真实计时器，实用计时器，虚拟计时器，统计程序执行时间，在实用计时器及虚拟计时器设定计时后，先睡眠，再让程序处于while（1）

void sigfunc(int signum)
{
    time_t now=time(NULL);
    printf("Now=%s",ctime(&now));
}
int main()
{
   signal(SIGALRM,sigfunc);
   //signal(SIGPROF,sigfunc);
   //signal(SIGVTALRM,sigfunc);


   struct itimerval t;
   bzero(&t,sizeof(t));
   t.it_value.tv_sec=3;
   t.it_interval.tv_sec=2;
   sigfunc(0);
   int ret=setitimer(ITIMER_REAL,&t,NULL);//真实计时器，程序全程运行的实际时间
   //int ret=setitimer(ITIMER_PROF,&t,NULL);//实用计时器，程序处于用户态和内核态消耗时间之和(不包含sleep)
   //int ret=setitimer(ITIMER_VIRTUAL,&t,NULL);//虚拟计时器，程序在用户态时消耗的时间

   //ERROR_CHECK(ret,-1,"setitimer");

   sleep(5);
   while(1);
   return 0;
}