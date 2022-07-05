# include <func.h>
//通过fork创建子进程，让父子进程都打印pid，都处于while(1),然后用ps进程查看，查看完毕，用kill命令杀掉
int main()
{
    pid_t pid=fork();
    if(0==pid)
    {
        printf("child_process: my_pid=%d,my_ppid=%d\n",getpid(),getppid());
        while(1);
        return 0;
    }else
    {
        // sleep(1);
        printf("parent_process: mychild_pid=%d,my_pid=%d,my_ppid=%d\n",pid,getpid(),getppid());
        while(1);
        
        return 0;
    }
    
}
