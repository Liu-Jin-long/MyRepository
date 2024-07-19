#include "func.h"
int main(int argc, char **argv)
{
    ARGS_CHECK(argc, 3)
    int fd_r = open(argv[1], O_RDONLY);
    ERROR_CHECK(fd_r, -1, "open_r")
    int fd_w = open(argv[2], O_WRONLY);
    ERROR_CHECK(fd_w, -1, "open_w")
    printf("I am chat1.\n");
    char buf[128] = {0};
    fd_set rfdset;
    int read_ret, ret;
    while (1)
    {
        FD_ZERO(&rfdset);
        FD_SET(STDIN_FILENO, &rfdset);
        FD_SET(fd_r, &rfdset);
        ret = select(fd_r + 1, &rfdset, NULL, NULL, NULL);
        if (FD_ISSET(STDIN_FILENO, &rfdset))
        {
            memset(buf, 0, sizeof(buf));
            read_ret = read(STDIN_FILENO, buf, sizeof(buf));
            printf("发送%d个字符\n", read_ret);
            if (read_ret == 0)
            {
                printf("chat is broken!\n");
                break;
            }
            write(fd_w, buf, strlen(buf) - 1);
        }
        if (FD_ISSET(fd_r, &rfdset))
        {
            memset(buf, 0, sizeof(buf));
            read_ret = read(fd_r, buf, sizeof(buf));
            if (read_ret == 0)
            {
                printf("chat is broken!\n");
                break;
            }
            printf("buf = %s\n", buf);
        }
    }
        close(fd_w);
        close(fd_r);
        return 0;
}