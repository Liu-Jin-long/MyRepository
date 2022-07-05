#include <func.h>

struct MSG{
    long mtype;
    char buf[64];
};

int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,3);
    int msgid=msgget(1000,IPC_CREAT|0600);
    ERROR_CHECK(msgid,-1,"msgget");
    struct MSG msg;//消息体需要自己定义
    msg.mtype=atoi(argv[1]);//消息类型，主要是为了给不同消息分优先级
    strcpy(msg.buf,argv[2]);
    int ret=msgsnd(msgid,&msg,strlen(msg.buf),0);//发送消息
    ERROR_CHECK(ret,-1,"msgsnd");
    return 0;
}