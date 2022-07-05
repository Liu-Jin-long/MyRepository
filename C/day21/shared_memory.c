#include <func.h>
//新建共享内存，连接，然后往共享内存写入How are you,然后另外一个进程连接共享内存，读取数据并打印显示,删除共享内存

int main()
{
    int shmid;//约定一个key值，得到同一shmid。   key值是一个与共享内存段相关联的参数
    shmid=shmget(888,4096,IPC_CREAT|0600);//没有这个key值就创建，有就返回这个
    ERROR_CHECK(shmid,-1,"shmid");
    printf("shmid=%d",shmid);
    char *p=shmat(shmid,NULL,0);
    strcpy(p,"How are you?");
    return 0;

}