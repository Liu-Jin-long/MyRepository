#include <func.h>

int main()
{
    int semid=semget(123,3,IPC_CREAT|0600);
    ERROR_CHECK(semid,-1,"semget");
    unsigned short arr[3]={5,5,1};//full,empty,mutex
    int ret=semctl(semid,0,SETALL,arr);
    ERROR_CHECK(ret,-1,"semctl");

    struct sembuf semop_p,semop_v,mutex_p,mutex_v;
    
    
    mutex_p.sem_num=2;
    mutex_p.sem_op=-1;
    mutex_p.sem_flg=SEM_UNDO;

    mutex_v.sem_num=2;
    mutex_v.sem_op=1;
    mutex_v.sem_flg=SEM_UNDO;

    if(!fork())
    {
        semop_p.sem_num=0;
        semop_p.sem_op=-1;
        semop_p.sem_flg=0;
        semop_v.sem_num=1;
        semop_v.sem_op=1;
        semop_v.sem_flg=0;

        while(1)
        {
            printf("I am consumer,full=%d,empty=%d\n",semctl(semid,0,GETVAL),semctl(semid,1,GETVAL));
            semop(semid,&semop_p,1);

            semop(semid,&mutex_p,1);
            printf("消费中......\n");
            semop(semid,&mutex_v,1);

            semop(semid,&semop_v,1);
            printf("I am consumer,full=%d,empty=%d\n",semctl(semid,0,GETVAL),semctl(semid,1,GETVAL));
            sleep(2);
        }
    }
    else
    {
        
        semop_p.sem_num=0;
        semop_p.sem_op=+1;
        semop_p.sem_flg=0;
        semop_v.sem_num=1;
        semop_v.sem_op=-1;
        semop_v.sem_flg=0;

        while(1)
        {
            printf("I am producer,full=%d,empty=%d\n",semctl(semid,0,GETVAL),semctl(semid,1,GETVAL));
            semop(semid,&semop_v,1);


            semop(semid,&mutex_p,1);
            printf("生产中......\n");
            semop(semid,&mutex_v,1);


            semop(semid,&semop_p,1);
            printf("I am consumer,full=%d,empty=%d\n",semctl(semid,0,GETVAL),semctl(semid,1,GETVAL));
            sleep(1);
        }
    }


}