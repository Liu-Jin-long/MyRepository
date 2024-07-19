#include "factory.h"
int transfer_file(int sfd)
{
    int fd = open(FILENAME, O_RDWR);
    ERROR_CHECK(fd, -1, "open")
    train_t data;
    data.data_len = strlen(FILENAME);
    strcpy(data.buf, FILENAME);
    int ret = send_n(sfd, &data, data.data_len + 4);
    SELFFUNC_ERR_CHECK(ret, "send_n")
    //发送文件大小
    struct stat file_info;
    ret = stat(FILENAME, &file_info);
    ERROR_CHECK(ret, -1, "stat")
    data.data_len = sizeof(file_info.st_size);
    memcpy(data.buf, &file_info.st_size, data.data_len);
    ret = send_n(sfd, &data, data.data_len + 4);
    SELFFUNC_ERR_CHECK(ret, "send_n")

    //发送文件内容
    // void *p = mmap(NULL, file_info.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //文件映射
    // ERROR_CHECK(p, (void *)-1, "mmap")
    // ret = send_n(sfd, p, file_info.st_size);
    // SELFFUNC_ERR_CHECK(ret, "send_n")
    ret = sendfile(sfd, fd, 0, file_info.st_size);
    ERROR_CHECK(ret, -1, "sendfile");
    // while (data.data_len = read(fd, data.buf, sizeof(data.buf)))
    // {
    //     ret = send_n(sfd, &data, data.data_len + 4);
    //     SELFFUNC_ERR_CHECK(ret, "send_n")
    // }
    //文件内容发送完毕标志
    // data.data_len = 0;
    // ret = send_n(sfd, &data, data.data_len + 4);
    // SELFFUNC_ERR_CHECK(ret, "send_n")
    // ret = munmap(p, file_info.st_size);
    // ERROR_CHECK(ret, -1, "mmap")
    return 0;
}
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
        if (total - last_size > splice_size)
        {
            printf("%6.2lf%%\r", (double)total / data_len);
            fflush(stdout);
            last_size = total;
        }
    }
    printf("100.00%%\n");
    return 0;
}