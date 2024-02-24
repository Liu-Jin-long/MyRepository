#include "operation.h"
#include "string.h"
#include "thread_pool.h"
#include "md5sum.h"
#include "link_list.h"
//注册
int do_register(state_train_t *p_state_data, int new_fd, char *username)
{
    char *password = NULL;
    password = strchr(p_state_data->buf, ' ');
    state_train_t state_data = {0};
    if (!password)
    {
        printf("客户端发送用户名和密码不符合要求(用户名和密码空格隔开)\n");
        state_data.state = UserNameAndPasswordAreInvalid;
        state_data.data_len = sizeof(int);
        send_n(new_fd, &state_data, state_data.data_len + sizeof(int));
        return -1;
    }
    *password = '\0';
    password += 1;
    char *user_name = p_state_data->buf;
    printf("user_name=%s,password=%s\n", user_name, password);
    //查询用户名是否已经存在
    unsigned char is_exist = 0;
    select_mysql(USER, "user_name", p_state_data->buf, 1, &is_exist);
    if (is_exist)
    {
        printf("用户名已存在\n");
        state_data.state = UserNameAlreadyExists;
        state_data.data_len = sizeof(int);
        send_n(new_fd, &state_data, state_data.data_len + sizeof(int));
        return -1;
    }
    //注册
    char salt[SALT_LEN + 1] = {0};
    generate_random_string(salt, SALT_LEN);
    char salt_and_password[SALT_LEN + strlen(password) + 1];
    strcpy(salt_and_password, salt);
    strcat(salt_and_password, password);
    char md5sum_str[MD5_STR_LEN + 1] = {0};
    md5sum_string(salt_and_password, md5sum_str);
    printf("salt_and_password=%s,md5sum_str=%s\n", salt_and_password, md5sum_str);
    char values[100] = {0};
    sprintf(values, "'%s','%s','%s'", user_name, salt, md5sum_str);
    printf("user_name=%s,md5sum_str=%s,salt=%s,password=%s,values=%s\n", user_name, md5sum_str, salt, password, values);
    insert_mysql(USER, values);
    //回复客户端注册成功
    // printf("用户%s注册成功\n", user_name);
    strcpy(username, user_name);
    state_data.state = RegisteredSuccessfully;
    state_data.data_len = sizeof(int);
    send_n(new_fd, &state_data, state_data.data_len + sizeof(int));
    char log_values[200] = {0};
    sprintf(log_values, "(select id from User where user_name='%s'),'register','success'", username);
    insert_mysql(LOG, log_values);
    return 0;
}
//登录
int do_login(state_train_t *p_state_data, int new_fd, char *username)
{
    char *password = NULL;
    password = strchr(p_state_data->buf, ' ');
    state_train_t state_data = {0};
    if (!password)
    {
        printf("客户端发送用户名和密码不符合要求(用户名和密码空格隔开)\n");
        state_data.state = UserNameAndPasswordAreInvalid;
        state_data.data_len = sizeof(int);
        send_n(new_fd, &state_data, state_data.data_len + sizeof(int));
        return -1;
    }
    *password = '\0';
    password += 1;
    char *user_name = p_state_data->buf;
    //查询用户名是否已存在
    unsigned char is_exist = 0;
    char salt[SALT_LEN + 1] = {0};
    char database_password[MD5_STR_LEN + 1] = {0};
    select_UserInfo(user_name, &is_exist, salt, database_password);
    printf("user_name=%s,password=%s,salt=%s,database_password=%s\n", user_name, password, salt, database_password);
    if (!is_exist)
    {
        printf("用户名不存在\n");
        state_data.state = UserNameDoesNotExist;
        state_data.data_len = sizeof(int);
        send_n(new_fd, &state_data, state_data.data_len + sizeof(int));
        return -1;
    }
    //校验密码
    char salt_and_password[SALT_LEN + strlen(password) + 1];
    sprintf(salt_and_password, "%s%s", salt, password);
    char md5sum_str[MD5_STR_LEN + 1] = {0};
    md5sum_string(salt_and_password, md5sum_str);
    printf("username=%s,salt_and_password=%s,md5sum_str=%s\n", user_name, salt_and_password, md5sum_str);
    if (strcmp(md5sum_str, database_password) != 0)
    {
        printf("密码错误\n");
        state_data.state = PasswordError;
        state_data.data_len = sizeof(int);
        send_n(new_fd, &state_data, state_data.data_len + sizeof(int));
        return -1;
    }
    //登陆成功
    // printf("用户%s登录成功\n", user_name);
    strcpy(username, user_name);
    state_data.state = LoginSuccessfully;
    state_data.data_len = sizeof(int);
    send_n(new_fd, &state_data, state_data.data_len + sizeof(int));
    char log_values[200] = {0};
    sprintf(log_values, "(select id from User where user_name='%s'),'login','success'", username);
    insert_mysql(LOG, log_values);
    return 0;
}
int do_ls(const client_info_t *p_client_info, state_train_t *p_state_data)
{
    state_train_t ls_data = {0};
    ls_data.state = LS;
    int ret = 0;
    if (0 == strcmp(p_state_data->buf, "ls") || 0 == strcmp(p_state_data->buf, "ls ")) //只处理不带参数的ls
    {
        //注意数据查询长度大于state_train_t的buf长度的情况
        ret = select_mysql_for_ls(p_client_info->username, p_client_info->code, ls_data.buf);
        if (-1 == ret)
        {
            printf("code=%d查询失败\n", p_client_info->code);
            ls_data.data_len = sizeof(int) + strlen(ls_data.buf);
            ret = send_n(p_client_info->sfd, &ls_data, ls_data.data_len + sizeof(int));
            SELFFUNC_ERR_CHECK(ret, "send_n")
            return -1;
        }
    }
    else if (0 == strcmp(p_state_data->buf, "ls /"))
    {
        //注意数据查询长度大于state_train_t的buf长度的情况
        ret = select_mysql_for_ls(p_client_info->username, 0, ls_data.buf);
        if (-1 == ret)
        {
            printf("code=%d查询失败\n", p_client_info->code);
            ls_data.data_len = sizeof(int) + strlen(ls_data.buf);
            ret = send_n(p_client_info->sfd, &ls_data, ls_data.data_len + sizeof(int));
            SELFFUNC_ERR_CHECK(ret, "send_n")
            return -1;
        }
    }
    else
    {
        char *dir_name = strchr(p_state_data->buf, ' ');
        dir_name += 1;
        if (strlen(dir_name) > 1 && (dir_name[strlen(dir_name)-1] == '/' || dir_name[strlen(dir_name)-1] == '\\'))
        {
            dir_name[strlen(dir_name)-1] = '\0';
        }
        int dir_code;
        select_mysql_for_cd(p_client_info->username, dir_name, p_client_info->code, 1, &dir_code);
        if (dir_code != -1)
        {
            ret = select_mysql_for_ls(p_client_info->username, dir_code, ls_data.buf);
            if (-1 == ret)
            {
                printf("dir_code=%d查询失败\n", dir_code);
                ls_data.data_len = sizeof(int) + strlen(ls_data.buf);
                ret = send_n(p_client_info->sfd, &ls_data, ls_data.data_len + sizeof(int));
                SELFFUNC_ERR_CHECK(ret, "send_n")
                return -1;
            }
        }
        else
        {
            printf("当前网盘路径不存在%s文件夹\n", dir_name);
            ls_data.data_len = sizeof(int) + strlen(ls_data.buf);
            ret = send_n(p_client_info->sfd, &ls_data, ls_data.data_len + sizeof(int));
            SELFFUNC_ERR_CHECK(ret, "send_n")
            return -1;
        }
    }
    ls_data.data_len = sizeof(int) + strlen(ls_data.buf);
    ret = send_n(p_client_info->sfd, &ls_data, ls_data.data_len + sizeof(int));
    SELFFUNC_ERR_CHECK(ret, "send_n")
    //日志
    char log_values[200] = {0};
    sprintf(log_values, "(select id from User where user_name='%s'),'ls',''", p_client_info->username);
    insert_mysql(LOG, log_values);
    return 0;
}
int do_cd(client_info_t *p_client_info, state_train_t *p_state_data)
{
    //如果有的话去除最后的路径分隔符
    char *dir_name = strchr(p_state_data->buf, ' ');
    dir_name += 1;
    if (strlen(dir_name) > 1 && (dir_name[strlen(dir_name)-1] == '/' || dir_name[strlen(dir_name)-1] == '\\'))
    {
        dir_name[strlen(dir_name)-1] = '\0';
    }
    int code;
    if (0 == strcmp(dir_name, ".")) // cd .
    {
        // memset(p_client_info->path,0,sizeof(p_client_info->path));
        //  strcpy(p_client_info->path,"/");
        //  p_client_info->code=0;
    }
    else if (0 == strcmp(dir_name, "/")) // cd /
    {
        memset(p_client_info->path, 0, sizeof(p_client_info->path));
        strcpy(p_client_info->path, "/");
        p_client_info->code = 0;
    }
    else if (0 == strcmp(dir_name, "..")) // cd ..
    {
        if (0 != strcmp(p_client_info->path, "/") || 0 != p_client_info->code)
        {
            //查询precode并
            select_mysql_for_cd(p_client_info->username, NULL, p_client_info->code, 0, &code);
            p_client_info->code = code;
            if (code == 0)
            {
                memset(p_client_info->path, 0, sizeof(p_client_info->path));
                strcpy(p_client_info->path, "/");
            }
            else
            {
                char *slash = strrchr(p_client_info->path, '/');
                memset(slash, 0, strlen(slash));
            }
        }
    }
    else // cd dir_dirname
    {
        select_mysql_for_cd(p_client_info->username, dir_name, p_client_info->code, 1, &code);
        if (code == -1)
        {
            // cd file_name不处理
            return -1;
        }
        p_client_info->code = code;
        if (0 != strcmp(p_client_info->path, "/"))
        {
            strcat(p_client_info->path, "/");
        }
        strcat(p_client_info->path, dir_name);
    }
    //日志
    char log_values[200] = {0};
    sprintf(log_values, "(select id from User where user_name='%s'),'cd','%s'", p_client_info->username, dir_name);
    insert_mysql(LOG, log_values);
    return 0;
}

int do_pwd(const client_info_t *p_client_info)
{
    state_train_t pwd_data = {0};
    pwd_data.state = PWD;
    strcpy(pwd_data.buf, p_client_info->path);
    pwd_data.data_len = sizeof(int) + strlen(pwd_data.buf);
    int ret = send_n(p_client_info->sfd, &pwd_data, sizeof(int) + pwd_data.data_len);
    SELFFUNC_ERR_CHECK(ret, "send_n")
    //日志
    char log_values[200] = {0};
    sprintf(log_values, "(select id from User where user_name='%s'),'pwd',''", p_client_info->username);
    insert_mysql(LOG, log_values);
    return 0;
}
int do_rm(const client_info_t *p_client_info, state_train_t *p_state_data)
{
    char *filename = strchr(p_state_data->buf, ' ');
    if (filename == NULL)
    {
        printf("rm格式错误\n"); //客户端负者简单检查格式
        return -1;
    }
    filename += 1;
    int ret = delete_mysql_for_rm(filename, p_client_info->code, p_client_info->username);
    SELFFUNC_ERR_CHECK(ret, "delete_mysql_for_rm")
    //日志
    char log_values[200] = {0};
    sprintf(log_values, "(select id from User where user_name='%s'),'rm','%s'", p_client_info->username, filename);
    insert_mysql(LOG, log_values);
    return 0;
}
int do_mkdir(const client_info_t *p_client_info, state_train_t *p_state_data)
{
    char *dir_name = strchr(p_state_data->buf, ' ');
    if (dir_name == NULL)
    {
        printf("rm格式错误\n"); //客户端负者简单检查格式
        return -1;
    }
    dir_name += 1;
    if (0 == strcmp(dir_name, ".") || 0 == strcmp(dir_name, "..") || 0 == strcmp(dir_name, "/"))
    {
        // mkdir .或..或/不处理
        return -1;
    }
    //如果有的话去除最后的路径分隔符
    if (dir_name[strlen(dir_name)] == '/' || dir_name[strlen(dir_name)] == '\\')
    {
        dir_name[strlen(dir_name)] = '\0';
    }
    char *format = "%d,'%s','d',(select id from User where user_name='%s'),'0'";
    char values[200] = {0};
    sprintf(values, format, p_client_info->code, dir_name, p_client_info->username);
    int ret = insert_mysql(USERFILE, values);
    SELFFUNC_ERR_CHECK(ret, "insert_mysql")
    //日志
    char log_values[200] = {0};
    sprintf(log_values, "(select id from User where user_name='%s'),'mkdir','%s'", p_client_info->username, dir_name);
    insert_mysql(LOG, log_values);
    return 0;
}
int do_puts(pNode_t p)
{
    //客户端上传
    int ret;
    state_train_t state_data = {0};
    train_t data = {0};
    char *md5_file = p->buf;
    char *tmp_file_size = strchr(p->buf, ' ');
    *tmp_file_size = 0;
    tmp_file_size += 1;
    char *file_name = strchr(tmp_file_size, ' ');
    *file_name = 0;
    file_name += 1;
    off_t file_size = atol(tmp_file_size);
    unsigned char is_exist = 0;
    ret = select_mysql_for_current_path_find_file(
        p->p_client_info->username, file_name, p->p_client_info->code, &is_exist, NULL);
    SELFFUNC_ERR_CHECK(ret, "select_mysql_for_current_path_find_file")
    if (is_exist)
    {
        printf("当前网盘路径已存在该同名文件\n");
        state_data.state = FileAlreadyExistsInTheCurrentNetworkDiskPath;
        state_data.data_len = sizeof(state_data.state);
        ret = send_n(p->sfd, &state_data, state_data.data_len + sizeof(state_data.data_len));
        SELFFUNC_ERR_CHECK(ret, "send_n")
        return -1;
    }
    ret = select_insert_FilePool(file_size, md5_file, &is_exist, 1);
    SELFFUNC_ERR_CHECK(ret, "select_insert_FilePool")

    if (is_exist)
    {
        //秒传
        printf("秒传\n");
        state_data.state = InstantTransmission;
        state_data.data_len = sizeof(state_data.state);
        ret = send_n(p->sfd, &state_data, state_data.data_len + sizeof(state_data.data_len));
        SELFFUNC_ERR_CHECK(ret, "send_n")
    }
    else
    {
        //普通传输
        printf("普通上传\n");
        state_data.state = FileDoesNotExistOnTheServer;
        state_data.data_len = sizeof(state_data.state);
        ret = send_n(p->sfd, &state_data, state_data.data_len + sizeof(state_data.data_len));
        SELFFUNC_ERR_CHECK(ret, "send_n")
        char path_filename[100] = {0};
        sprintf(path_filename, "%s%s", FILE_POOL_PATH, md5_file);
        ret = recv_file(p->sfd, path_filename, 0);
        SELFFUNC_ERR_CHECK(ret, "recv_file")
        //插入FilePool表
        char values_[200] = {0};
        sprintf(values_, "%ld,'%s'", file_size, md5_file);
        ret = insert_mysql(FILEPOOL, values_);
        SELFFUNC_ERR_CHECK(ret, "insert_mysql")
    }
    //插入UserFile表
    char *format = "%d,'%s','%s',(select id from User where user_name = '%s'),'%s'";
    char values[200] = {0};
    sprintf(values, format, p->p_client_info->code, file_name, "f", p->p_client_info->username, md5_file);
    ret = insert_mysql(USERFILE, values);
    SELFFUNC_ERR_CHECK(ret, "insert_mysql")
    char log_values[200] = {0};
    sprintf(log_values, "(select id from User where user_name='%s'),'puts','%s'", p->p_client_info->username, file_name);
    insert_mysql(LOG, log_values);
    return 0;
}
int do_gets(pNode_t p)
{
    //客户端下载
    char *p_offset = strchr(p->buf, ' ');
    *p_offset = 0;
    off_t offset = atol(p_offset + 1);
    char *file_name = p->buf;
    state_train_t state_data = {0};
    int ret;
    unsigned char is_exist = 0;
    char md5_file[MD5_STR_LEN + 1] = {0};
    select_mysql_for_current_path_find_file(p->p_client_info->username, file_name, p->p_client_info->code, &is_exist, md5_file);
    state_data.data_len = sizeof(state_data.state);
    if (!is_exist)
    {
        //当前路径不存在该文件,直接结束该任务
        state_data.state = FileDoesNotExistsInTheCurrentNetworkDiskPath;
        ret = send_n(p->sfd, &state_data, state_data.data_len + sizeof(state_data.data_len));
        SELFFUNC_ERR_CHECK(ret, "send_n")
        return -1;
    }
    else
    {
        state_data.state = FileAlreadyExistsInTheCurrentNetworkDiskPath;
        ret = send_n(p->sfd, &state_data, state_data.data_len + sizeof(state_data.data_len));
        SELFFUNC_ERR_CHECK(ret, "send_n")
        //从文件池中寻找并发送文件
        char path_filename[100] = {0};
        sprintf(path_filename, "%s%s", FILE_POOL_PATH, md5_file);
        ret = send_file(p->sfd, path_filename, offset);
        SELFFUNC_ERR_CHECK(ret, "send_file")
        char log_values[200] = {0};
        sprintf(log_values, "(select id from User where user_name='%s'),'gets','%s'", p->p_client_info->username, file_name);
        insert_mysql(LOG, log_values);
        return 0;
    }
}
