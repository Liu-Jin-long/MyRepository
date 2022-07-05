#include <func.h>
//两个进程对一个共享内存各加1000万，通过pv操作，实现最终结果是2千万

int main()
{
    int N=10000000;
    int shmid=shmget(666,4096,IPC_CREAT|0600);
    ERROR_CHECK(shmid,-1,"shmget");
    printf("shmid=%d\n",shmid);

    int *p=shmat(shmid,NULL,0);
    ERROR_CHECK(p,(char*)-1,"shmid");
    p[0]=0;

    int semid=semget(666,1,IPC_CREAT|0600);//信号量集合id,要有读写权限
    ERROR_CHECK(semid,-1,"semget");
    

    int ret=semctl(semid,0,SETVAL,1);//最后一位为可变类型参数，表示初始化的值
    ERROR_CHECK(ret,-1,"semgctl");
    ret=semctl(semid,0,GETVAL);
    ERROR_CHECK(ret,-1,"semctl");
    printf("semid=%d\n",semid);

    struct sembuf semop_p,semop_v;
    semop_p.sem_num=0;//操作的信号量的(集合)数组下标
    semop_p.sem_op=-1;//减1操作就是p操作
    semop_p.sem_flg=SEM_UNDO;

    semop_v.sem_num=0;
    semop_v.sem_op=1;//加1操作就是v操作
    semop_v.sem_flg=SEM_UNDO;

    int i;
    if(!fork())
    {
        for(i=0;i<N;i++)
        {
            semop(semid,&semop_p,1);
            p[0]++;
            semop(semid,&semop_v,1);
        }
        return 0;

    }
    else
    {
       for(i=0;i<N;i++)
        {
            semop(semid,&semop_p,1);
            p[0]++;
            semop(semid,&semop_v,1);
        } 
        wait(NULL);
        printf("p[0]=%d\n",p[0]);
        return 0;
    }


}