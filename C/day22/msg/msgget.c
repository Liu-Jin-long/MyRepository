#include <func.h>

int main()
{
    //创建一个消息队列
    int msgid=msgget(1000,IPC_CREAT|0600);
    ERROR_CHECK(msgid,-1,"msgget");
    return 0;
}