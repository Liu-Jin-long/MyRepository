#include <func.h>
//两个进程对一个共享内存各加1000万，查看最后结果

int main()
{
    int N=10000000;
    int shmid=shmget(666,4096,IPC_CREAT|0600);
    ERROR_CHECK(shmid,-1,"shmget");
    int *p=shmat(shmid,NULL,0);
    ERROR_CHECK(p,(char*)-1,"shmid");
    p[0]=0;
    int i;
    if(fork())
    {
        for(i=0;i<N;i++)
        {
            p[0]++;
        }
        wait(NULL);
        printf("%d\n",p[0]);
        return 0;

    }else
    {
       for(i=0;i<N;i++)
        {
            p[0]++;
        } 
        return 0;
    }


}