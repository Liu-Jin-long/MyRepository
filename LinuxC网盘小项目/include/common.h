#ifndef __COMMON_H__
#define __COMMON_H__
#include "config.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <pthread.h>
//收发数据结构体buf大小
#define DATA_LEN 1024
//维持连接的token大小
#define TOKEN_LEN 64
enum Operation
{
    REGISTER,
    LOGIN,
    LS,
    CD,
    RM,
    PWD,
    MKDIR,
    PUTS,
    GETS,
    TOKEN,
};
enum State{
    UserNameAlreadyExists=50,//注册时用户名已存在
    UserNameAndPasswordAreInvalid,//用户名和密码不符合要求
    UserNameDoesNotExist,//用户名不存在
    PasswordError,//密码错误
    RegisteredSuccessfully,//注册成功
    LoginSuccessfully,//登陆成功
    TokenAuthenticationSucceeded,//Token验证成功
    TokenAuthenticationFailed,//Token验证失败
    InstantTransmission,//秒传
    FileDoesNotExistsInTheCurrentNetworkDiskPath,//当前网盘路径不存在该同名文件
    FileAlreadyExistsInTheCurrentNetworkDiskPath,//当前网盘路径已存在该同名文件
    FileDoesNotExistOnTheServer,//服务器不存在该文件
};
//接收和发送数据
typedef struct
{
    int data_len;   //标识数据部的有效长度
    char buf[DATA_LEN]; //数据部
} train_t;
//客户端发送的命令操作信息
typedef struct
{
    int data_len;//标识数据部buf有效长度(包括operation大小)
    int state;//状态码 可以是Operation或State
    char buf[DATA_LEN];
} state_train_t;
//服务器维护的客户端信息
typedef struct {
    int sfd;//客户端主线程socket
    int code;//当前所在路径文件夹的code，code为正，0代表是/根
    char username[64];//用户名
    char path[64];//当前路径
    char token[TOKEN_LEN];//维持连接的token
}client_info_t;

#define ARGS_CHECK(argc, num) {if(argc != num){printf("file:%s line:%d  error args\n", __FILE__, __LINE__);return -1;}}
#define ERROR_CHECK(ret, retval, func_name) {if(ret == retval){printf("file:%s line:%d  errno=%d,", __FILE__, __LINE__, errno);fflush(stdout);perror(func_name);return -1;}}
#define THREAD_ERR_CHECK(ret, func_name) {if(ret != 0){printf("file:%s line:%d  %s failed!!!,%d %s\n", __FILE__, __LINE__, func_name, ret, strerror(ret));return -1;}}
#define SELFFUNC_ERR_CHECK(ret, func_name) {if(ret != 0){printf("file:%s line:%d  %s failed!!!\n", __FILE__, __LINE__, func_name);return -1;}}
#define SELFFUNC_ERR_CHECK_RETURN_POINTER(ret, func_name) {if(ret != 0){printf("file:%s line:%d  %s failed!!!\n", __FILE__, __LINE__, func_name);return (void*)-1;}}
#endif