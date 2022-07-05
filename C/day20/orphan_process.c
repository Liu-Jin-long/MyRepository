#include <func.h>

//得到孤儿进程
int main()
{
    pid_t pid=fork();
    if(0==pid)
    {
        while(1);//结果子进程一直运行，且父进程pid为1
    }else
    {
        return 0;
    }
}