#include <func.h>


int main()
{
    pid_t pid=fork();
    if(0==pid)
    {
        sleep(2);
        printf("child_process: my_pid=%d,my_ppid=%d\n",getpid(),getppid());
        return 0;
    }else
    {
        pid=wait(NULL);//子进程结束，会给父进程发送SIGCHLD，父进程才会回收子进程资源
        printf("parent_process: mychild_pid=%d,my_pid=%d,my_ppid=%d\n",pid,getpid(),getppid());
        printf("wait pid=%d\n",pid);
        return 0;
    }
}