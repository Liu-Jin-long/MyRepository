#include <func.h>
//通过sigprocmask阻塞2号信号，睡眠5秒后，解除阻塞，2号信号得到执行；在睡眠后，解除阻塞之前，通过sigpending检测是否有信号挂起

//先判断阻塞，在判断忽略（忽略会直接删除而不会挂起）
int main()
{
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask,SIGINT);
    int ret=sigprocmask(SIG_BLOCK,&mask,NULL);
    ERROR_CHECK(ret,-1,"sigprocmask");
    printf("（全程阻塞），关键代码开始\n");
    sleep(5);
    sigset_t pending;
    sigemptyset(&pending);
    sigpending(&pending);//拿出内核中正在挂起的信号
    if(sigismember(&pending,SIGINT))
    {
        printf("SIGINT is pending\n");
    }
    else{
        printf("SIGINT isn't pending\n");
    }

    printf("（全程阻塞），关键代码结束\n");
    
    
    ret=sigprocmask(SIG_UNBLOCK,&mask,NULL);//解除阻塞
    ERROR_CHECK(ret,-1,"sigprocmask");
    return 0;



}