#include <func.h>

int main()
{
    //创建一个信号量集合
    int sem_id=semget(1000,1,IPC_CREAT|0600);//要给读写权限
    ERROR_CHECK(sem_id,-1,"semget");
    return 0;
}