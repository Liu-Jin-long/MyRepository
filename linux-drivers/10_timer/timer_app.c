#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <linux/ioctl.h>
#define CLOSE_CMD (_IO(0xef, 1))
#define OPEN_CMD (_IO(0xef, 2))
#define SETPERIOD_CMD (_IO(0xef, 3))

#define ARGS_CHECK(argc, num) {if(argc != num){printf("file:%s line:%d  error args\n", __FILE__, __LINE__);return -1;}}
#define ERROR_CHECK(ret, retval, func_name) {if(ret == retval){printf("file:%s line:%d  errno=%d,", __FILE__, __LINE__, errno);fflush(stdout);perror(func_name);return -1;}}
int main(int argc,char** argv)
{
    ARGS_CHECK(argc, 2);
    char *filename=argv[1];
    int fd=open(filename,O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    ssize_t size=0;
    unsigned int cmd;
    int ret;
    unsigned long arg=0;
    char ch;
    while (1)
    {
        printf("请输入命令：1 2 3\n");
        ret=read(STDIN_FILENO, &cmd, sizeof(cmd));
        setbuf(stdin,NULL);
        ERROR_CHECK(size, -1, "read");
        cmd = atoi((char*)&cmd);
        if (cmd == 1)
        {
            cmd = CLOSE_CMD;
        }else if(cmd==2)
        {
           cmd = OPEN_CMD; 
        }else if(cmd==3)
        {
            cmd = SETPERIOD_CMD;
            printf("请输入周期值:\n");
            scanf("%u", &arg);
            setbuf(stdin,NULL);
        }else
        {
            printf("错误输入命令%d\n", cmd);
            continue;
        }
        printf("cmd=%u arg=%u\n", cmd,arg);
        ioctl(fd, cmd, arg);
        ERROR_CHECK(size,-1,"ioctl");
    }
    ret=close(fd);
    ERROR_CHECK(ret,-1,"close");
    return 0;
}