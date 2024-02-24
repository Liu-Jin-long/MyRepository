#include "transfer_file.h"
#include "common.h"
#include <sys/mman.h>
#include <assert.h>

int send_file(int sfd, const char *filename, off_t offset)
{
    int fd = open(filename, O_RDONLY);
    ERROR_CHECK(fd, -1, "open")
    train_t data = {0};
    int ret;
    // //发送文件名
    // data.data_len = strlen(filename);
    // strcpy(data.buf, filename);
    // ret = send_n(sfd, &data, data.data_len + sizeof(int));
    // SELFFUNC_ERR_CHECK(ret, "send_n")
    //发送文件总大小
    struct stat file_info;
    ret = stat(filename, &file_info);
    ERROR_CHECK(ret, -1, "stat")
    data.data_len = sizeof(file_info.st_size);
    memcpy(data.buf, &file_info.st_size, data.data_len);
    ret = send_n(sfd, &data, data.data_len + sizeof(int));
    SELFFUNC_ERR_CHECK(ret, "send_n")
    printf("发送文件file_size=%ld,offset=%ld,file_info.st_size-offset=%ld\n", file_info.st_size, offset, file_info.st_size - offset);
    // //普通方式发送文件内容
    // off_t total = 0;
    // ret = lseek(fd, offset, SEEK_SET);
    // ERROR_CHECK(ret, -1, "lseek")
    // while (total < file_info.st_size - offset)
    // {
    //     bzero(&data, sizeof(data));
    //     data.data_len = read(fd, data.buf, sizeof(data.buf));
    //     ret = send_n(sfd, &data, data.data_len + sizeof(int));
    //     SELFFUNC_ERR_CHECK(ret, "send_n")
    //     total += data.data_len;
    // }
    // mmap方式发送文件内容
    void *p = mmap(NULL, file_info.st_size, PROT_READ, MAP_SHARED, fd, 0); //文件映射
    ERROR_CHECK(p, (void *)-1, "mmap")
    char *p_offset_p = (char *)p + offset;
    ret = send_n(sfd, p_offset_p, file_info.st_size - offset);
    SELFFUNC_ERR_CHECK(ret, "send_n")
    ret = munmap(p, file_info.st_size); //解除映射
    ERROR_CHECK(ret, -1, "munmap")
    // // sendfile方式发送文件内容
    // ret = sendfile(sfd, fd, &offset, file_info.st_size - offset);
    // ERROR_CHECK(ret, -1, "sendfile");
    return 0;
}
int recv_file(int sfd, const char *file_name, off_t offset)
{
    train_t data = {0};
    int ret;
    // //接收文件名
    // ret = recv_n(sfd, &data.data_len, sizeof(data.data_len));
    // SELFFUNC_ERR_CHECK(ret, "recv_n")
    // ret = recv_n(sfd, data.buf, data.data_len);
    // SELFFUNC_ERR_CHECK(ret, "recv_n")
    int fd = open(file_name, O_RDWR | O_CREAT, 0666); //测试发现只有O_WRONLY|O_CREAT会导致mmap权限不够被拒绝
    ERROR_CHECK(fd, -1, "open")
    //接收文件大小
    bzero(&data, sizeof(data));
    ret = recv_n(sfd, &data.data_len, sizeof(data.data_len));
    SELFFUNC_ERR_CHECK(ret, "recv_n")
    off_t file_size;
    ret = recv_n(sfd, &file_size, data.data_len);
    SELFFUNC_ERR_CHECK(ret, "recv_n")
    off_t recv_file_size = file_size - offset;
    printf("接收文件file_size=%ld,offset=%ld,file_size-offset=%ld\n", file_size, offset, recv_file_size);
    ret = ftruncate(fd, file_size);
    ERROR_CHECK(ret, -1, "ftruncate")
    // //普通方式接受文件内容
    // off_t total = 0;
    // ret = lseek(fd, offset, SEEK_SET);
    // ERROR_CHECK(ret, -1, "lseek")
    // while (total < recv_file_size)
    // {
    //     bzero(&data, sizeof(data));
    //     ret = recv_n(sfd, &data.data_len, sizeof(data.data_len));
    //     SELFFUNC_ERR_CHECK(ret, "recv_n")
    //     ret = recv_n(sfd, data.buf, data.data_len);
    //     SELFFUNC_ERR_CHECK(ret, "recv_n")
    //     write(fd, data.buf, data.data_len);
    //     total += data.data_len;
    // }
    // mmap方式接收文件内容
    void *p = mmap(NULL, file_size, PROT_WRITE, MAP_SHARED, fd, 0);
    ERROR_CHECK(p, (void *)-1, "mmap")
    char *p_offset_p = (char *)p + offset;
    ret = recv_n(sfd, p_offset_p, recv_file_size);
    SELFFUNC_ERR_CHECK(ret, "recv_n")
    ret = munmap(p, file_size);
    ERROR_CHECK(ret, -1, "munmap")
    // // splice方式接收文件内容
    // off_t total = 0, splice_bytes = 0;
    // int pipe_fd[2];
    // pipe(pipe_fd);
    // while (total <recv_file_size)
    // {
    //     splice_bytes = splice(sfd, NULL, pipe_fd[1], NULL, 32 * 1024, SPLICE_F_MORE | SPLICE_F_MOVE);
    //     assert(splice_bytes != -1);
    //     ret = splice(pipe_fd[0], NULL, fd, &offset, splice_bytes, SPLICE_F_MORE | SPLICE_F_MOVE);
    //     assert(ret != -1);
    //     total += ret;
    // }
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