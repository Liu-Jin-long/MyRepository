#include "client_op.h"
#include "md5sum.h"
#include "transfer_file.h"
#include <sys/types.h>
#include <sys/syscall.h>
void *child_pthread_func(void *p_arg)
{
    printf("进入%ld线程处理\n", syscall(SYS_gettid));
    p_client_child_pthread_info p_pthread_info = (p_client_child_pthread_info)p_arg;
    char *filename = strchr(p_pthread_info->buf, ' ');
    *filename = 0;
    filename += 1;
    int ret;
    ret = access(filename, F_OK);
    if (0 == ret && p_pthread_info->state == GETS)
    {
        printf("当前客户端本地路径已存在同名文件\n");
        free(p_pthread_info);
        return (void *)-1;
    }
    else if (ret == -1 && p_pthread_info->state == PUTS)
    {
        printf("当前客户端本地路径不存在该文件\n");
        free(p_pthread_info);
        return (void *)-1;
    }

    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1)
    {
        printf("子线程socket错误\n");
        free(p_pthread_info);
        return (void *)-1;
    }
    ret = connect(sfd, p_pthread_info->p_server_addr, sizeof(struct sockaddr));
    SELFFUNC_ERR_CHECK_RETURN_POINTER(ret, "connect")
    struct stat file_info = {0};

    state_train_t state_data = {0};
    //先发token
    state_data.state = TOKEN;
    strncpy(state_data.buf, p_pthread_info->token, TOKEN_LEN);
    state_data.data_len = sizeof(state_data.state) + strlen(state_data.buf);
    send_n(sfd, &state_data, state_data.data_len + sizeof(int));
    SELFFUNC_ERR_CHECK_RETURN_POINTER(ret, "send_n")
    //接收token是否认证成功
    bzero(&state_data, sizeof(state_data));
    ret = recv_n(sfd, &state_data.data_len, sizeof(state_data.data_len));
    SELFFUNC_ERR_CHECK_RETURN_POINTER(ret, "recv_n")
    ret = recv_n(sfd, &state_data.state, state_data.data_len);
    SELFFUNC_ERR_CHECK_RETURN_POINTER(ret, "recv_n")
    if (state_data.state == TokenAuthenticationFailed)
    {
        printf("Token认证失败\n");
        free(p_pthread_info);
        close(sfd);
        return (void *)-1;
    }
    train_t data = {0};

    if (p_pthread_info->state == PUTS)
    {
        //上传
        // puts md5_file filesize filename
        char md5_file[MD5_STR_LEN + 1] = {0};
        md5sum_file(filename, md5_file);
        bzero(&state_data, sizeof(state_data));
        state_data.state = PUTS;
        //发送md5_file filesize filename
        ret = stat(filename, &file_info);
        SELFFUNC_ERR_CHECK_RETURN_POINTER(ret, "stat")
        sprintf(state_data.buf, "%s %ld %s", md5_file, file_info.st_size, filename);
        state_data.data_len = sizeof(int) + strlen(state_data.buf);
        ret = send_n(sfd, &state_data, state_data.data_len + sizeof(int));
        SELFFUNC_ERR_CHECK_RETURN_POINTER(ret, "send_n")
        //服务器是否已存在该文件而构成秒传条件
        bzero(&state_data, sizeof(state_train_t));
        ret = recv_n(sfd, &state_data.data_len, sizeof(int));
        SELFFUNC_ERR_CHECK_RETURN_POINTER(ret, "recv_n")
        ret = recv_n(sfd, &state_data.state, state_data.data_len);
        SELFFUNC_ERR_CHECK_RETURN_POINTER(ret, "recv_n")
        if (state_data.state == FileAlreadyExistsInTheCurrentNetworkDiskPath)
        {
            printf("当前网盘路径已存在该同名文件\n");
            close(sfd);
            free(p_pthread_info);
            return (void *)-1;
        }
        else if (state_data.state == InstantTransmission)
        {
            printf("秒传\n");
            //传文件名
            strcpy(data.buf, filename);
            data.data_len = strlen(data.buf);
            ret = send_n(sfd, &state_data, data.data_len + sizeof(int));
            SELFFUNC_ERR_CHECK_RETURN_POINTER(ret, "send_n")
            printf("%s文件上传完成！\n", filename);
            close(sfd);
            free(p_pthread_info);
            return (void *)0;
        }
        else if (state_data.state == FileDoesNotExistOnTheServer)
        {
            printf("普通上传\n");
            //传文件名、文件大小和文件内容
            ret = send_file(sfd, filename, 0);
            SELFFUNC_ERR_CHECK_RETURN_POINTER(ret, "send_file")
            printf("%s文件上传完成！\n", filename);
            close(sfd);
            free(p_pthread_info);
            return (void *)0;
        }
        else
        {
            free(p_pthread_info);
            close(sfd);
            return (void *)-1;
        }
    }
    else if (p_pthread_info->state == GETS)
    {
        //下载过成中文件名带download后缀，下完再改回来
        //断点续传
        off_t offset;
        char undownloaded_filename[100] = {0};
        strcpy(undownloaded_filename, filename);
        strcat(undownloaded_filename, BREAKPOINT_RESUME_FILE_SUFFIX);
        ret = stat(undownloaded_filename, &file_info);
        if (0 == ret)
        {
            //未下完的部分文件存在 断点续传
            offset = file_info.st_size;
        }
        else if (-1 == ret)
        {
            offset = 0;
        }
        // gets file_name offset
        bzero(&state_data, sizeof(state_data));
        state_data.state = GETS;
        //发送file_name offset
        sprintf(state_data.buf, "%s %ld", filename, offset);
        state_data.data_len = strlen(state_data.buf) + sizeof(state_data.state);
        ret = send_n(sfd, &state_data, state_data.data_len + sizeof(state_data.data_len));
        SELFFUNC_ERR_CHECK_RETURN_POINTER(ret, "send_n")
        //当前网盘路径是否有该文件
        bzero(&state_data, sizeof(state_data));
        ret = recv_n(sfd, &state_data.data_len, sizeof(int));
        SELFFUNC_ERR_CHECK_RETURN_POINTER(ret, "recv_n")
        ret = recv_n(sfd, &state_data.state, state_data.data_len);
        SELFFUNC_ERR_CHECK_RETURN_POINTER(ret, "recv_n")
        if (state_data.state == FileDoesNotExistsInTheCurrentNetworkDiskPath)
        {
            printf("当前网盘文件夹不存在该文件\n");
            free(p_pthread_info);
            close(sfd);
            return (void *)-1;
        }
        else if (state_data.state == FileAlreadyExistsInTheCurrentNetworkDiskPath)
        {
            //接收文件内容
            ret = recv_file(sfd, undownloaded_filename, offset);
            SELFFUNC_ERR_CHECK_RETURN_POINTER(ret, "recv_file")
            rename(undownloaded_filename, filename);
            printf("下载成功\n");
            free(p_pthread_info);
            close(sfd);
            return (void *)0;
        }
    }
    else
    {
        free(p_pthread_info);
        close(sfd);
        return (void *)-1;
    }
}