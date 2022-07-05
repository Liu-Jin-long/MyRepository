#include <func.h>

#define NUM 10000000
int main()
{
    int shmid;
    shmid=shmget(1000,4096,IPC_CREAT|0600);//第一次创建，第二次相同的key值是获取
    ERROR_CHECK(shmid,-1,"shmget");
    printf("shmid=%d\n",shmid);
    int *p=shmat(shmid,NULL,0);//共享内存连接到本进程内
    ERROR_CHECK(p,(char*)-1,"shmat");
    p[0]=0;
    int i;
    //信号量的创建及初始化
        //创建一个信号量集合
    int sem_id=semget(1000,1,IPC_CREAT|0600);//要给读写权限
    ERROR_CHECK(sem_id,-1,"semget");
    //给信号量的值初始化为1
    int ret=semctl(sem_id,0,SETVAL,1);
    ERROR_CHECK(ret,-1,"semctl");
    struct sembuf sopp,sopv;
    sopp.sem_num=0;//操作的是几号信号量
    sopp.sem_op=-1;//减一操作就是p操作
    sopp.sem_flg=SEM_UNDO;
    sopv.sem_num=0;
    sopv.sem_op=1;//加1操作就是v操作
    sopv.sem_flg=SEM_UNDO;
    time_t start,end;
    //对内存操作产生了并发，没有加锁，造成最终不等于2千万
    start=time(NULL);
    if(!fork())
    {
        for(i=0;i<NUM;i++)
        {
            //加锁，p操作,原子操作
            semop(sem_id,&sopp,1);
            p[0]++;
            semop(sem_id,&sopv,1);
            //解锁，v操作,原子操作
        }
        return 0;
    }else{
        for(i=0;i<NUM;i++)
        {
            //加锁，p操作,原子操作
            semop(sem_id,&sopp,1);
            p[0]++;
            semop(sem_id,&sopv,1);
            //解锁，v操作,原子操作
        }
        wait(NULL);
        end=time(NULL);
        printf("p[0]=%d,use time=%ld\n",p[0],end-start);
        return 0;
    }
}