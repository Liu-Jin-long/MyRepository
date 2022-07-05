#include <func.h>
//创建消息队列，发送消息，接收消息，删除消息队列


typedef struct MSG
{
    long msgtype;
    char buf[64];
}MSG;
int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,3);
    int msgid=msgget(666,IPC_CREAT|0600);
    ERROR_CHECK(msgid,-1,"msgget")
    MSG msg;
    msg.msgtype=atoi(argv[1]);//不同消息类型
    strcpy(msg.buf,argv[2]);
    int ret=msgsnd(msgid,&msg,strlen(msg.buf),0);
    ERROR_CHECK(ret,-1,"msgsnd");
    return 0;

}