#include <func.h>
//sigaction实现信号不断重入的执行效果(来回打断的效果)

void signal_func(int signal_num,siginfo_t *p,void *p1)//自定义信号处理函数
{
    printf("\n before,signal num is %d\n",signal_num);
    sleep(3);
    printf("\n after,signal num is %d\n",signal_num);

}

int main()
{
    struct sigaction action;
    bzero(&action,sizeof(action));//一定要清空
    action.sa_sigaction=signal_func;
    action.sa_flags=SA_SIGINFO|SA_NODEFER;//SA_SIGINFO(常用)表明signal_func生效,SA_NODEFER可以让其他信号（包括相同信号）打断自己，打断后不断压栈(类似递归)，过程中所有信号都能得到执行
    int ret=sigaction(SIGINT,&action,NULL);
    ERROR_CHECK(ret,-1,"sigaction");
    ret=sigaction(SIGQUIT,&action,NULL);
    ERROR_CHECK(ret,-1,"sigaction");

    while(1);
    return 0;
}