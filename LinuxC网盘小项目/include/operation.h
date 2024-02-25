#ifndef __OPERATION_H__
#define __OPERATION_H__
#include"mysql_op.h"
#include"common.h"
#include"work_queue.h"
int do_register(state_train_t* p_state_data,int new_fd,char* username);
int do_login(state_train_t* p_state_data,int new_fd,char* username);
int do_ls(const client_info_t* p_client_info,state_train_t* p_state_data);
int do_cd(client_info_t* p_client_info,state_train_t* p_state_data);
int do_pwd(const client_info_t* p_client_info);
int do_mkdir(const client_info_t *p_client_info, state_train_t *p_state_data);
int do_rm(const client_info_t* p_client_info,state_train_t* p_state_data);
int do_puts(pNode_t p);
int do_gets(pNode_t p);
#endif