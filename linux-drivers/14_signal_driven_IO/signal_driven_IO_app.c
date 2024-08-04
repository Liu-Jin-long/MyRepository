#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
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

static int fd;
void sighandler(int signal_num)
{
    if (signal_num == SIGIO)
    {
        printf("SIGIO reach!\n");
        unsigned char key_value;
        read(fd, &key_value, sizeof(key_value));
        printf("key value=%#X!\n", key_value);
    }
}

int main(int argc, char **argv)
{
    ARGS_CHECK(argc, 2);
    int flags;
    char *filename = argv[1];
    fd = open(filename, O_RDWR);
    ERROR_CHECK(fd, -1, "open");
    int ret;
    if (SIG_ERR == signal(SIGIO, sighandler))
    {
        printf("signal error!\n");
        return -1;
    }
    fcntl(fd, F_SETOWN, getpid());
    flags = fcntl(fd, F_GETFD);
    fcntl(fd, F_SETFL, flags | FASYNC);
    while (1)
        ;
    ret = close(fd);
    ERROR_CHECK(ret, -1, "close");
    return 0;
}