#include <func.h>

//sigfunc是我们自定义的信号处理函数
void sigfunc(int signum)
{
    printf("before sleep,sig %d is coming\n",signum);
    sleep(3);
    printf("after sleep,sig %d is coming\n",signum);
}

int main()
{
    //提前设定好2号信号的信号处理行为
    signal(SIGINT,sigfunc);
    signal(SIGQUIT,sigfunc);
    while(1);
    return 0;
}
