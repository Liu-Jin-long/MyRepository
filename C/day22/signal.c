#include <func.h>

//signal设置信号处理行为，演示不同信号到来时，后来的信号会打断原有信号的信号处理函数效果
//signal设置信号处理行为，演示相同信号到来时，无法打断本信号的效果
void signal_func(int signal_num)//自定义信号处理函数
{
    printf("\n before,signal num is %d\n",signal_num);
    sleep(3);
    printf("\n after,signal num is %d\n",signal_num);

}


//相同信号不能打断自己，连续多次相同信号只会在最初该信号执行完后 只去执行一次
int main()
{
    if(SIG_ERR==signal(SIGINT,signal_func))//2号信号处理改为执行signal_func函数 
    {                                      //第二个参数为SIG_IGN，会忽略前一个参数代表的信号
        printf("signal failed\n");
        return -1;
    }
    if(SIG_ERR==signal(SIGQUIT,signal_func))
    {                                      
        printf("signal failed\n");
        return -1;
    }

    //signal(SIGINT,SIG_DFL);//设置回缺省
    while(1);
    return 0;
}