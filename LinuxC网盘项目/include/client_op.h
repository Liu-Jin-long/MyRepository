#ifndef __CLIENT_OP_H__
#define __CLIENT_OP_H__
#include "common.h"
//断点续传文件后缀
#define BREAKPOINT_RESUME_FILE_SUFFIX ".download"
typedef struct{
    const struct sockaddr *p_server_addr;
    int state;
    char buf[DATA_LEN];
    char token[TOKEN_LEN];
}client_child_pthread_info_t,*p_client_child_pthread_info;

void* child_pthread_func(void* p_arg);
#endif
