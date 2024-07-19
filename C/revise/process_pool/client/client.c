#include "process_pool.h"
//通过select实现tcp的即时聊天
int main(int argc, char **argv)
{
    ARGS_CHECK(argc, 3);
    int sfd;
    sfd = socket(AF_INET, SOCK_STREAM, 0); //套接口描述符，对应的是一个缓冲区
    ERROR_CHECK(sfd, -1, "socket");
    printf("sfd=%d\n", sfd);
    struct sockaddr_in my_addr;                   // Structure describing an Internet socket address.
    bzero(&my_addr, sizeof(my_addr));             //要清空
    my_addr.sin_family = AF_INET;                 // AF_INET代表ipv4，AF_INET6代表ipv6
    my_addr.sin_addr.s_addr = inet_addr(argv[1]); //点分十进制转为网络字节序
    my_addr.sin_port = htons(atoi(argv[2]));      //端口号转为网络字节序

    //客户端连接服务器
    int ret = connect(sfd, (struct sockaddr *)&my_addr, sizeof(my_addr));
    ERROR_CHECK(ret, -1, "connect");

    char buf[1000] = {0};
    int data_len;
    //接收文件名
    ret = recv_n(sfd, &data_len, 4); //文件名长度
    SELFFUNC_ERR_CHECK(ret, "recv_n")
    ret = recv_n(sfd, buf, data_len); //文件名
    SELFFUNC_ERR_CHECK(ret, "recv_n")
    int fd = open(buf, O_RDWR | O_CREAT, 0666);
    ERROR_CHECK(fd, -1, "open");

    //接收文件大小
    ret = recv_n(sfd, &data_len, 4);
    SELFFUNC_ERR_CHECK(ret, "recv_n")
    off_t file_size;
    ret = recv_n(sfd, &file_size, data_len);
    SELFFUNC_ERR_CHECK(ret, "recv_n")

    //每0.01%打印一次
    // download_size当前下载大小 last_size上一次下载大小 last_size 0.01%打印
    off_t download_size = 0, last_size = 0, splice_size = 0;
    splice_size = file_size / 10000; // 0.01%
    //每秒打印一次
    // time_t now, last;
    // now = last = time(NULL);

    //统计下载时间
    struct timeval start, end;
    gettimeofday(&start, NULL);

    //接收文件大小
    ret = ftruncate(fd, file_size);
    ERROR_CHECK(ret, -1, "ftruncate")
    //接收文件内容
    int pipefd[2];
    pipe(pipefd);
    off_t total = 0, splice_bytes = 0;
    while (total < file_size)
    {
        splice_bytes = splice(sfd, NULL, pipefd[1], NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
        assert(splice_bytes != -1);
        ret = splice(pipefd[0], NULL, fd, NULL, splice_bytes, SPLICE_F_MORE | SPLICE_F_MOVE);
        assert(ret != -1);
        total += ret;
    }

    //------------------------------------------------------------------------------------
    // void *p = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //文件映射
    // ERROR_CHECK(p, (void *)-1, "mmap")
    // ret = recv_n_display(sfd, p, file_size);
    // SELFFUNC_ERR_CHECK(ret, "recv_n")
    // ret = munmap(p, file_size);
    // ERROR_CHECK(p, (void *)-1, "munmap")
    //------------------------------------------------------------------------------------
    // while (1)
    // {
    //     ret = recv_n(sfd, &data_len, 4); //文件内容长度
    //     SELFFUNC_ERR_CHECK(ret, "recv_n")
    //     if (data_len > 0)
    //     {
    //         ret = recv_n(sfd, buf, data_len); //文件内容
    //         SELFFUNC_ERR_CHECK(ret, "recv_n")
    //         write(fd, buf, data_len);
    //         download_size = download_size + data_len;
    //         if (download_size - last_size > splice_size)
    //         {
    //             printf("%6.2lf%%\r", (double)download_size / file_size);
    //             fflush(stdout);
    //             last_size = download_size;
    //         }
    //         // now = time(NULL);
    //         // if (now > last)
    //         // {
    //         //     printf("%6.2lf%%\r", (double)download_size / file_size);
    //         //     fflush(stdout);
    //         //     last = now;
    //         // }
    //     }
    //     else
    //     {
    //         printf("100.00%%\n");
    //         break;
    //     }
    // }
    gettimeofday(&end, NULL);
    printf("下载时间：%ld微秒\n", (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec));
    close(fd);
    close(sfd);
    return 0;
}