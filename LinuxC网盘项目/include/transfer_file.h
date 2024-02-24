#ifndef __TRANSFER_FILE_H__
#define __TRANSFER_FILE_H__
#define _GNU_SOURCE
#include <fcntl.h>
int send_file(int sfd, const char *file_name, off_t offset);
int recv_file(int sfd, const char *file_name, off_t offset);
int recv_n(int sfd, void *buf, int data_len);
int send_n(int sfd, void *buf, int data_len);
int recv_n_display(int sfd, void *buf, int data_len);
#endif