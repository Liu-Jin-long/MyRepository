#include<func.h>
struct MSG{
    long mtype;
    char buf[64];
}msg;
int main()
{
    int msgid=msgget((key_t)123,0600|IPC_CREAT);
    if(fork()==0)
    {
        bzero(&msg,sizeof(msg));
        msg.mtype=1;
        strcpy(msg.buf,"hello world");
        msgsnd(msgid,&msg,sizeof(msg),0);
        sleep(3);
        printf("3秒后\n");
        //msgctl(msgid,IPC_RMID,NULL);
        exit(1);
    }
    else{
        sleep(5);
        bzero(&msg,sizeof(msg));
        msgrcv(msgid,&msg,sizeof(msg),1,0);
        printf("5秒后\n");
        msgctl(msgid,IPC_RMID,NULL);
        puts(msg.buf);

        wait(NULL);
        exit(0);
    }
}