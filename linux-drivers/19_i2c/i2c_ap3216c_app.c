#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <linux/input.h>
#include <string.h>
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
    int fd = open(filename, O_RDWR);
    ERROR_CHECK(fd, -1, "open");
    int ret;
    unsigned short value[3];
    while (1)
    {
        ret = read(fd, &value, sizeof(value));
        ERROR_CHECK(ret, -1, "read");
        printf("lr = %d, als = %d, ps = %d\n", value[0], value[1], value[2]);
        usleep(500000);
    }

    ret = close(fd);
    ERROR_CHECK(ret, -1, "close");
    return 0;
}