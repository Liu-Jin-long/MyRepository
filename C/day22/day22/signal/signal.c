#include <func.h>

void sigfunc(int signum);

int main()
{
    //提前设定好2号信号的信号处理行为
    if(SIG_ERR == signal(SIGINT,sigfunc))
    {
        printf("signal failed\n");
        return -1;
    }
    while(1);
    // int i;
    // scanf("%d",&i);
    // printf("get=%d\n",i);
    return 0;
}
//sigfunc是我们自定义的信号处理函数
void sigfunc(int signum)
{
    printf("sig %d is coming\n",signum);
}