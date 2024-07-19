#include "process_pool.h"

//循环接受指定的数据
int recv_n(int sfd, void *buf, int data_len)
{
    char *p = (char *)buf;
    int total = 0, ret;
    while (total < data_len)
    {
        ret = recv(sfd, p + total, data_len - total, 0);
        if (0 == ret)
        {
            return -1;
        }
        total += ret;
    }
    return 0;
}
//循环发送指定的数据
int send_n(int sfd, void *buf, int data_len)
{
    char *p = (char *)buf;
    int total = 0, ret;
    while (total < data_len)
    {
        ret = send(sfd, p + total, data_len - total, 0);
        if (-1 == ret) //对方断开
        {
            return -1;
        }
        total += ret;
    }
    return 0;
}
//循环接受指定的数据并展示下载进度
int recv_n_display(int sfd, void *buf, int data_len)
{
    char *p = (char *)buf;
    int total = 0, last_size = 0, ret = 0;
    int splice_size = data_len / 10000;
    while (total < data_len)
    {
        ret = recv(sfd, p + total, data_len - total, 0);
        if (0 == ret)
        {
            return -1;
        }
        total += ret;
        if(total-last_size>splice_size)
        {
            printf("%6.2lf%%\r", (double)total / data_len);
            fflush(stdout);
            last_size=total;  
        }
    }
    printf("100.00%%\n");
    return 0;
}
