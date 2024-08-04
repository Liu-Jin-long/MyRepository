#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <poll.h>
#include <sys/ioctl.h>
#define SELECT
// #define POLL
// #define FD_NUM 1

#define ARGS_CHECK(argc, num)                                            \
    {                                                                    \
        if (argc != num)                                                 \
        {                                                                \
            printf("file:%s line:%d  error args\n", __FILE__, __LINE__); \
            return -1;                                                   \
        }                                                                \
    }
#define ERROR_CHECK(ret, retval, func_name)                                  \
    {                                                                        \
        if (ret == retval)                                                   \
        {                                                                    \
            printf("file:%s line:%d  errno=%d,", __FILE__, __LINE__, errno); \
            fflush(stdout);                                                  \
            perror(func_name);                                               \
            return -1;                                                       \
        }                                                                    \
    }
int main(int argc, char **argv)
{
    ARGS_CHECK(argc, 2);
    char *filename = argv[1];
    int fd = open(filename, O_RDWR | O_NONBLOCK);
    ERROR_CHECK(fd, -1, "open");
    unsigned char key_value;
    int ret;
#ifdef SELECT
    fd_set read_fdset;
    struct timeval timeout;
#endif
#ifdef POLL
    struct pollfd pool_fds[FD_NUM];
    pool_fds[0].fd = fd;
    pool_fds[0].events = POLLIN;
    int fd_num = FD_NUM;
    int timeout = 2000; // 毫秒
#endif
    while (1)
    {
#ifdef SELECT
        FD_ZERO(&read_fdset);
        FD_SET(fd, &read_fdset);
        timeout.tv_sec = 2;
        timeout.tv_usec = 0;
        ret = select(fd + 1, &read_fdset, NULL, NULL, &timeout);
        //printf("ret=%d\n",ret);
#endif
#ifdef POLL
        ret = poll(pool_fds, fd_num, timeout);
#endif
        if (ret < 0)
        {
#ifdef SELECT
            printf("select error!\n");
#endif
#ifdef POLL
            printf("poll error!\n");
#endif
        }
        else if (ret == 0)
        {
            printf("超时！\n");
        }
        else
        {
            //printf("ret>0 ret=%d\n",ret);
#ifdef SELECT
            if (FD_ISSET(fd, &read_fdset))
            {
#endif
#ifdef POLL
                for (int i = 0; i < fd_num; i++)
                {
                    if (pool_fds[i].fd == fd || pool_fds[i].events & POLLIN)
                    {

#endif
                        //printf("应用读\n");
                        ret = read(fd, &key_value, sizeof(key_value));
                        if (ret < 0)
                        {
                            printf("应用程序中read系统调用返回值小于0时:ret=%d\n");
                        }
                        else if (ret == EAGAIN)
                        {
                            printf("ret==EAGAIN\n");
                        }
                        else
                        {
                            printf("key value = %#X\n", key_value);
                        }
#ifdef POLL
                    }
#endif
                }
            }
        }
        ret = close(fd);
        ERROR_CHECK(ret, -1, "close");
        return 0;
    }