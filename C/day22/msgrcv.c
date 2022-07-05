#include <func.h>


typedef struct MSG
{
    long msgtype;
    char buf[64];
 }MSG;

 int main()
 {
    int msgid=msgget(666,IPC_CREAT|0600);
    ERROR_CHECK(msgid,-1,"msgget");
    MSG msg;
    bzero(&msg,sizeof(msg.buf));
    int ret=msgrcv(msgid,&msg,sizeof(msg.buf),1,0);//msgtpy==0,接收队列中第一个消息；msgtpy>0，接收队列中的第一个类型等于msgtpy的消息；msgtpy<0，接收队列中类型小于或等于msgtpy绝对值的第一个最低类型消息
    ERROR_CHECK(ret,-1,"msgrcv");
    printf("get msgtype=%ld,message=%s\n",msg.msgtype,msg.buf);
    return 0;




 }