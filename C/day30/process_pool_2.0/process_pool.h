#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/time.h>
#include <strings.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/epoll.h>
#define ARGS_CHECK(argc,num) {if(argc!=num) {printf("error args\n");return -1;}}
#define ERROR_CHECK(ret,retval,func_name) {if(ret==retval) {printf("errno=%d,",errno);fflush(stdout);perror(func_name);return -1;}}
#define THREAD_ERR_CHECK(ret,func_name) {if(ret!=0) {printf("%s failed,%d %s\n",func_name,ret,strerror(ret));return -1;}}
#define SELFFUNC_ERR_CHECK(ret,func_name) {if(ret!=0) {printf("%s failed\n",func_name);return -1;}}


typedef struct
{
    pid_t pid;//父进程每个子进程的pid
    int fd;//管道(子进程的对端)的一端
    short busy;//代表子进程的状态，1代表忙碌，0代表非忙碌
}process_data_t;


//创造多个子进程 
int make_child_process(process_data_t* p_manage,int process_num);
//子进程代码流程
int child_handle(int fd);
//socket,bind,listen的初始化
int tcp_init(int* socket_fd,char* ip,char* port);
//注册fd并监听是否可读
int epoll_add(int epfd,int fd);
//发送描述符
int send_fd(int sfd,int fd);
//接收描述符
int recv_fd(int sfd,int *fd);