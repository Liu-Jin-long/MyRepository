#include <func.h>

//一次设置1个信号量集合中多个信号量的值
int main()
{
    //创建一个信号量集合
    int sem_id=semget(1000,2,IPC_CREAT|0600);//要给读写权限
    ERROR_CHECK(sem_id,-1,"semget");
    unsigned short arr[2]={2,6};
    //给两个信号量同时赋值
    int ret=semctl(sem_id,0,SETALL,arr);
    ERROR_CHECK(ret,-1,"semctl");
    //获取信号量里边的每一个信号量的值
    bzero(arr,sizeof(arr));
    ret=semctl(sem_id,0,GETALL,arr);
    ERROR_CHECK(ret,-1,"semctl");
    printf("信号量0的值=%d,信号量1的值=%d\n",arr[0],arr[1]);
    return 0;
}