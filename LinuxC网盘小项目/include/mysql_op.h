#ifndef __MYSQL_OP_H__
#define __MYSQL_OP_H__
#include "config.h"
#include <mysql/mysql.h>
#include <string.h>
#include <stdio.h>
enum TableName{
    USER,
    USERFILE,
    LOG,
    FILEPOOL,
};
int init_mysql(MYSQL **connection);
int insert_mysql(int enum_table_name, char* values);
int select_mysql(int enum_table_name,char* field,char* field_value,unsigned char field_is_str,unsigned char* is_exist);
int select_UserInfo(char *user_name, unsigned char *is_exist, char* salt,char* password);
int select_mysql_for_ls(const char* username,const int code,char* buf);
int delete_mysql_for_rm(char* filename,const int code,const char* username);
int delete_folders_recursively(MYSQL **conn,const int code,const char* username);
int select_mysql_for_cd(const char* username,const char* filename,const int code,const unsigned char is_string,int* const p_code);
int select_mysql_for_current_path_find_file(const char* username,const char* filename,const int code,unsigned char* is_exist,char* md5_file);
int select_insert_FilePool(off_t file_size, char *md5_file, unsigned char *is_exist,unsigned char is_select);
#endif