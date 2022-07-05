#include "func.h"

//循环接收数据，直到接收完所期待的数据量
int recv_n(int sfd,void* buf,int data_len)
{
    char *p=(char*)buf;
    int total=0,ret;
    while(total<data_len)
    {
        ret=recv(sfd,p+total,data_len-total,0);
        total+=ret;
    }
    return 0;
}

//循环发送数据，直到发送完所期待的数据量
int send_n(int sfd,void* buf,int data_len)
{
    char *p=(char*)buf;
    int total=0,ret;
    while(total<data_len)
    {
        ret=send(sfd,p+total,data_len-total,0);
        total+=ret;
    }
    return 0;
}