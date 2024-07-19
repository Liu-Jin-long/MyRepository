#include "func.h"
// monitor.c监控：增加程序鲁棒性
int main()
{
    while (1)
    {
        if (0 == fork())
        {
            while (1); // child_handle()子进程task处理函数
            return 0;
        }
        else
        {
            int status;
            wait(&status);
            if (WIFEXITED(status))
            {
                //子进程正常退出
                exit(0); //父进程退出
            }
            else
            {
                //子进程异常退出 再次循环创建子进程（拉起）
                printf("记录日志，以便定位异常退出问题\n");
            }
        }
    }
}