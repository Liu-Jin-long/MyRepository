#include <func.h>

//删除共享内存时有人正在连接操作
int main()
{
    int shmid;
    shmid=shmget(1000,4096,IPC_CREAT|0600);//获取对应共享内存
    ERROR_CHECK(shmid,-1,"shmget");
    //删除共享内存
    int ret=shmctl(shmid,IPC_RMID,NULL);
    ERROR_CHECK(ret,-1,"shmctl");
    return 0;
}