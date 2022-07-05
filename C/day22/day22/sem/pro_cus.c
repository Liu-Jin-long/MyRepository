#include <func.h>

//一次设置1个信号量集合中多个信号量的值
//为了理解计数信号量的优势
//通过计数信号量实现生产者，消费者模型
int main()
{
    //创建一个信号量集合
    int sem_id=semget(1000,2,IPC_CREAT|0600);//要给读写权限
    ERROR_CHECK(sem_id,-1,"semget");
    //0号信号量代表产品个数
    // 1号信号量代表空位个数
    unsigned short arr[2]={0,10};
    //给两个信号量同时赋值
    int ret=semctl(sem_id,0,SETALL,arr);
    ERROR_CHECK(ret,-1,"semctl");
    //父进程是生产者，子进程是消费者
    if(!fork())
    {//消费者
        struct sembuf sopp,sopv;
        sopp.sem_num=0;//产品
        sopp.sem_op=-1;
        sopp.sem_flg=0;
        sopv.sem_num=1;//空位
        sopv.sem_op=1;
        sopv.sem_flg=0;
        while(1)
        {
            printf("我是消费者 产品数目=%d,空位数=%d\n",semctl(sem_id,0,GETVAL),semctl(sem_id,1,GETVAL));
            semop(sem_id,&sopp,1);
            printf("开始消费.....\n");
            semop(sem_id,&sopv,1);
            printf("我是消费者 产品数目=%d,空位数=%d\n",semctl(sem_id,0,GETVAL),semctl(sem_id,1,GETVAL));
            sleep(2);
        }
    }else{//生产者
        struct sembuf sopp,sopv;
        sopp.sem_num=1;//空位
        sopp.sem_op=-1;
        sopp.sem_flg=0;
        sopv.sem_num=0;//产品
        sopv.sem_op=1;
        sopv.sem_flg=0;
        while(1)
        {
            printf("我是生产者 产品数目=%d,空位数=%d\n",semctl(sem_id,0,GETVAL),semctl(sem_id,1,GETVAL));
            semop(sem_id,&sopp,1);
            printf("开始生产.....\n");
            semop(sem_id,&sopv,1);
            printf("我是生产者 产品数目=%d,空位数=%d\n",semctl(sem_id,0,GETVAL),semctl(sem_id,1,GETVAL));
            sleep(1);
        }
    }
}