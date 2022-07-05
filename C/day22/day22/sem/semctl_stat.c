#include <func.h>

//获取信号量集合的属性信息，并设置权限
int main()
{
    int sem_id=semget(1000,1,IPC_CREAT|0600);//要给读写权限
    ERROR_CHECK(sem_id,-1,"semget");
    struct semid_ds buf;//用来存储信号量集合的属性信息
    int ret=semctl(sem_id,0,IPC_STAT,&buf);
    ERROR_CHECK(ret,-1,"semctl");
    printf("cuid=%d,mode=%o,nsems=%ld\n",\
    buf.sem_perm.cuid,buf.sem_perm.mode,buf.sem_nsems);
    buf.sem_perm.mode=0666;
    ret=semctl(sem_id,0,IPC_SET,&buf);
    ERROR_CHECK(ret,-1,"semctl1");
    return 0;
}