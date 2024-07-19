#include <func.h>
int main()
{
    int semid=semget(123,3,IPC_CREAT|0600);
    unsigned short init_val[3]={0,10,1};
    int ret=semctl(semid,0,SETALL,init_val);
    struct sembuf product_p,product_v,empty_p,empty_v,mutex_p,mutex_v;
    product_p.sem_num=0;
    product_p.sem_op=-1;
    product_p.sem_flg=SEM_UNDO;
    product_v.sem_num=0;
    product_v.sem_op=1;
    product_v.sem_flg=SEM_UNDO;

    empty_p.sem_num=1;
    empty_p.sem_op=-1;
    empty_p.sem_flg=SEM_UNDO;
    empty_v.sem_num=1;
    empty_v.sem_op=1;
    empty_v.sem_flg=SEM_UNDO;

    mutex_p.sem_num=2;
    mutex_p.sem_op=-1;
    mutex_p.sem_flg=SEM_UNDO;
    mutex_v.sem_num=2;
    mutex_v.sem_op=1;
    mutex_v.sem_flg=SEM_UNDO;
    int count=0;
    while(1)
    {
        count++;
        printf("-----------------------\n");
        printf("消费者：当前product=%d，empty=%d\n",semctl(semid,0,GETVAL),semctl(semid,1,GETVAL));
        
        semop(semid,&product_p,1);
        semop(semid,&mutex_p,1);
        printf("消费者第%d次消费中。。。\n",count);
        semop(semid,&mutex_v,1);
        semop(semid,&empty_v,1);
        
        printf("消费者：当前product=%d，empty=%d\n",semctl(semid,0,GETVAL),semctl(semid,1,GETVAL));
        sleep(2);
    }
    return 0;

}