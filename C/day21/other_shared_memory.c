#include <func.h>

int main()
{
    int shmid;
    shmid=shmget(888,4096,IPC_CREAT|0600);
    char *p=shmat(shmid,NULL,0);
    ERROR_CHECK(p,(char*)-1,"shmat");
    printf("shmid=%d",shmid);
    printf("shared memory data=%s",p);
    int ret=shmctl(shmid,IPC_RMID,NULL);
    ERROR_CHECK(ret,-1,"shmctl");
    
    return 0;
}