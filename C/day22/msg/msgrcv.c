#include <func.h>

struct MSG{
    long mtype;
    char buf[64];
};

int main()
{
    int msgid=msgget(1000,IPC_CREAT|0600);
    ERROR_CHECK(msgid,-1,"msgget");
    struct MSG msg;
    bzero(&msg,sizeof(msg));
    int ret=msgrcv(msgid,&msg,sizeof(msg.buf),1,0);//接收消息
    ERROR_CHECK(ret,-1,"msgrcv");
    printf("get mtype=%ld,msg=%s\n",msg.mtype,msg.buf);
    return 0;
}