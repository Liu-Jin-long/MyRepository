#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <linux/input.h>
#include<string.h> 
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
    struct input_event key_event;
    int ret;
    while (1)
    {
        memset(&key_event, 0, sizeof(struct input_event));
        ret = read(fd, &key_event, sizeof(key_event));
        if (ret < 0)
        {
            printf("read error!\n");
            return -1;
        }
        else
        {
            switch (key_event.type)
            {
            case EV_SYN:
                break;
            case EV_KEY:
                if (key_event.code < BTN_MISC)
                {
                    printf("key %d %s\n", key_event.code, key_event.value ? "press" : "release");
                }
                else
                {
                    printf("button %d %s\n", key_event.code, key_event.value ? "press" : "release");
                }
                break;
            case EV_REL:
                break;
            case EV_ABS:
                break;
            case EV_MSC:
                break;
                /*......*/
            }
        }
    }
    ret = close(fd);
    ERROR_CHECK(ret, -1, "close");
    return 0;
}