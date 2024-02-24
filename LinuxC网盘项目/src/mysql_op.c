#include "mysql_op.h"
int init_mysql(MYSQL **connection)
{
    const char *server_host = "127.0.0.1";
    const char *user = "root";
    const char *password = "123";
    const char *database = "NetworkDiskDatabase";
    *connection = mysql_init(NULL);
    if (!mysql_real_connect(*connection, server_host, user, password, database, 0, NULL, 0))
    {
        // printf("Error connecting to database:%s\n", mysql_error(*connection));
        mysql_close(*connection);
        return 0;
    }
    else
    {
        // printf("Mysql database connection has been successfully established.\n");
        return 0;
    }
}

int insert_mysql(int enum_table_name, char *values)
{
    MYSQL *connection;
    int ret = init_mysql(&connection);
    if (-1 == ret)
    {
        return -1;
    }
    char *format = "insert into %s(%s) values(%s);";
    char table_name[20] = {0};
    char fields[100] = {0};
    switch (enum_table_name)
    {
    case USER:
        strcpy(table_name, "User");
        strcpy(fields, "user_name,salt,password");
        break;
    case USERFILE:
        strcpy(table_name, "UserFile");
        strcpy(fields, "precode,file_name,file_type,owner_id,md5sum");
        break;
    case LOG:
        strcpy(table_name, "Log");
        strcpy(fields, "user_id,command,operation");
        break;
    case FILEPOOL:
        strcpy(table_name, "FilePool");
        strcpy(fields, "file_size,md5sum");
        break;
    default:
        printf("file%s line%d: \"enum_table_name=%d\" parameter error!\n", __FILE__, __LINE__, enum_table_name);
        return -1;
        break;
    }
    char insert_sql[200] = {0};
    sprintf(insert_sql, format, table_name, fields, values);
    printf("insert_sql: %s\n", insert_sql);
    ret = mysql_query(connection, insert_sql);
    if (ret)
    {
        printf("insert failedd: %s\n", mysql_error(connection));
        mysql_close(connection);
        return -1;
    }
    else
    {
        printf("insert success,insert row=%ld\n", (long)mysql_affected_rows(connection));
        mysql_close(connection);
        return 0;
    }
}
//表名 where条件字段 条件值 条件值是否是字符串 查找表中是否存在
int select_mysql(int enum_table_name, char *field, char *field_value, unsigned char field_is_str, unsigned char *is_exist)
{
    MYSQL *connection = NULL;
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    int ret = init_mysql(&connection);
    if (-1 == ret)
    {
        return -1;
    }
    char *format = NULL;
    if (field_is_str)
    {
        format = "select * from %s where %s='%s';";
    }
    else
    {
        format = "select * from %s where %s=%s;";
    }
    char table_name[20] = {0};
    switch (enum_table_name)
    {
    case USER:
        strcpy(table_name, "User");
        break;
    case USERFILE:
        strcpy(table_name, "UserFile");
        break;
    case LOG:
        strcpy(table_name, "Log");
        break;
    case FILEPOOL:
        strcpy(table_name, "FilePool");
        break;
    default:
        printf("%s %d: \"enum_table_name=%d\" parameter error!\n", __FILE__, __LINE__, enum_table_name);
        return -1;
        break;
    }
    char select_sql[200] = {0};
    sprintf(select_sql, format, table_name, field, field_value);
    printf("select_sql: %s\n", select_sql);
    ret = mysql_query(connection, select_sql);
    if (ret)
    {
        printf("select failedd: %s\n", mysql_error(connection));
        mysql_close(connection);
        return -1;
    }
    printf("select success,affected rows=%ld\n", (long)mysql_affected_rows(connection));
    if (NULL == (res = mysql_use_result(connection)))
    {
        printf("mysql_use_result() failed: %s\n", mysql_error(connection));
        mysql_close(connection);
        return -1;
    }
    if ((row = mysql_fetch_row(res)) != NULL)
    {
        *is_exist = 1;
    }
    else
    {
        *is_exist = 0;
    }
    mysql_free_result(res);
    mysql_close(connection);
    return 0;
}

int select_UserInfo(char *user_name, unsigned char *is_exist, char *salt, char *password)
{
    MYSQL *connection = NULL;
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    int ret = init_mysql(&connection);
    if (-1 == ret)
    {
        return -1;
    }
    char *format = "select salt,password from User where user_name='%s';";
    char select_sql[200] = {0};
    sprintf(select_sql, format, user_name);
    printf("select_sql: %s\n", select_sql);
    ret = mysql_query(connection, select_sql);
    if (ret)
    {
        printf("select failedd: %s\n", mysql_error(connection));
        mysql_close(connection);
        return -1;
    }
    // printf("select success,affected rows=%ld\n", (long)mysql_affected_rows(connection));
    if (NULL == (res = mysql_use_result(connection)))
    {
        printf("mysql_use_result() failed: %s\n", mysql_error(connection));
        mysql_close(connection);
        return -1;
    }
    if ((row = mysql_fetch_row(res)) != NULL)
    {
        *is_exist = 1;
        strcpy(salt, row[0]);
        strcpy(password, row[1]);
    }
    else
    {
        *is_exist = 0;
    }
    mysql_free_result(res);
    mysql_close(connection);
    return 0;
}
int select_insert_FilePool(off_t file_size, char *md5_file, unsigned char *is_exist, unsigned char is_select)
{
    MYSQL *connection = NULL;
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    int ret = init_mysql(&connection);
    if (-1 == ret)
    {
        return -1;
    }
    char *format;
    char sql[200] = {0};
    if (is_select)
    {
        format = "select id from FilePool where file_size=%ld and md5sum='%s';";
        sprintf(sql, format, file_size, md5_file);
        printf("select_sql: %s\n", sql);
    }
    else
    {
        format = "delete from FilePool where file_size=%ld and md5sum='%s';";
        sprintf(sql, format, file_size, md5_file);
        printf("delete_sql: %s\n", sql);
    }
    ret = mysql_query(connection, sql);
    if (ret)
    {
        printf("sql failedd: %s\n", mysql_error(connection));
        mysql_close(connection);
        return -1;
    }
    // printf("select success,affected rows=%ld\n", (long)mysql_affected_rows(connection));
    if (NULL == (res = mysql_use_result(connection)))
    {
        printf("mysql_use_result() failed: %s\n", mysql_error(connection));
        mysql_close(connection);
        return -1;
    }
    if ((row = mysql_fetch_row(res)) != NULL)
    {
        *is_exist = 1;
    }
    else
    {
        *is_exist = 0;
    }
    mysql_free_result(res);
    mysql_close(connection);
    return 0;
}
int select_mysql_for_ls(const char *username, const int code, char *buf)
{
    MYSQL *connection = NULL;
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    int ret = init_mysql(&connection);
    if (-1 == ret)
    {
        return -1;
    }
    char *format = "select file_name,file_type from UserFile where owner_id=(select id from User where user_name='%s') and precode=%d;";
    char select_sql[200] = {0};
    sprintf(select_sql, format, username, code);
    printf("select_sql: %s\n", select_sql);
    ret = mysql_query(connection, select_sql);
    if (ret)
    {
        printf("select failedd: %s\n", mysql_error(connection));
        mysql_close(connection);
        return -1;
    }
    // printf("select success,affected rows=%ld\n", (long)mysql_affected_rows(connection));
    if (NULL == (res = mysql_use_result(connection)))
    {
        printf("mysql_use_result() failed: %s\n", mysql_error(connection));
        mysql_close(connection);
        return -1;
    }
    char tmp[100] = {0};
    sprintf(tmp, "%-10s\t%-10s\n", "文件名", "文件类型");
    strcat(buf, tmp);
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        memset(tmp, 0, sizeof(tmp));
        sprintf(tmp, "%-10s\t%-s\n", row[0], row[1]);
        strcat(buf, tmp);
    }
    mysql_free_result(res);
    mysql_close(connection);
    return 0;
}
int delete_mysql_for_rm(char *filename, const int code, const char *username)
{
    MYSQL *connection = NULL;
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    int ret = init_mysql(&connection);
    if (-1 == ret)
    {
        return -1;
    }
    char *format = "select id_code,file_type from UserFile where owner_id=(select id from User where user_name='%s') and precode=%d and file_name='%s';";
    char select_sql[200] = {0};
    sprintf(select_sql, format, username, code, filename);
    printf("select_sql: %s\n", select_sql);
    ret = mysql_query(connection, select_sql);
    if (ret)
    {
        printf("select failedd: %s\n", mysql_error(connection));
        mysql_close(connection);
        return -1;
    }
    printf("select success,affected rows=%ld\n", (long)mysql_affected_rows(connection));
    if (NULL == (res = mysql_store_result(connection)))
    {
        printf("mysql_store_result() failed: %s\n", mysql_error(connection));
        mysql_close(connection);
        return -1;
    }
    memset(select_sql, 0, sizeof(select_sql));
    char *delete_sql = select_sql;
    char *delete_sql_format = "delete from UserFile where owner_id=(select id from User where user_name='%s') and id_code='%d';";
    if ((row = mysql_fetch_row(res)) != NULL)
    {
        if (0 == strcmp(row[1], "d"))
        {
            delete_folders_recursively(&connection, atoi(row[0]), username);
        }
        sprintf(delete_sql, delete_sql_format, username, atoi(row[0]));
        ret = mysql_query(connection, delete_sql);
        printf("delete_mysql_for_rm  delete_sql: %s\n", delete_sql);
        if (ret)
        {
            printf("delete failedd: %s\n", mysql_error(connection));
            mysql_free_result(res);
            mysql_close(connection);
            return -1;
        }
    }
    mysql_free_result(res);
    mysql_close(connection);
    return 0;
}
//递归删除文件夹内的所有文件
int delete_folders_recursively(MYSQL **conn, const int code, const char *username)
{
    MYSQL *connection = *conn;
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    char *format = "select id_code,file_type from UserFile where owner_id=(select id from User where user_name='%s') and precode=%d;";
    char select_sql[200] = {0};
    sprintf(select_sql, format, username, code);
    printf("select_sql: %s\n", select_sql);
    int ret = mysql_query(connection, select_sql);
    if (ret)
    {
        printf("select failedd: %s\n", mysql_error(connection));
        mysql_close(connection);
        return -1;
    }
    // printf("select success,affected rows=%ld\n", (long)mysql_affected_rows(connection));
    if (NULL == (res = mysql_store_result(connection)))
    {
        printf("mysql_store_result() failed: %s\n", mysql_error(connection));
        mysql_close(connection);
        return -1;
    }
    memset(select_sql, 0, sizeof(select_sql));
    char *delete_sql = select_sql;
    char *delete_sql_format = "delete from UserFile where owner_id=(select id from User where user_name='%s') and id_code='%d';";
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        if (0 == strcmp(row[1], "d"))
        {
            delete_folders_recursively(conn, atoi(row[0]), username);
        }
        sprintf(delete_sql, delete_sql_format, username, atoi(row[0]));
        ret = mysql_query(connection, delete_sql);
        printf("delete_mysql_for_rm  delete_sql: %s\n", delete_sql);
        if (ret)
        {
            printf("delete failedd: %s\n", mysql_error(connection));
            mysql_free_result(res);
            mysql_close(connection);
            return -1;
        }
    }
    mysql_free_result(res);
    return 0;
}
int select_mysql_for_cd(const char *username, const char *filename, const int code, const unsigned char is_string, int *const p_code)
{
    MYSQL *connection = NULL;
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    int ret = init_mysql(&connection);
    if (-1 == ret)
    {
        return -1;
    }
    char select_sql[200] = {0};
    char *format;
    if (is_string)
    {
        format = "select id_code,file_type from UserFile where owner_id=(select id from User where user_name='%s') and precode=%d and file_name='%s';";
        sprintf(select_sql, format, username, code, filename);
    }
    else
    {
        format = "select precode,file_type from UserFile where owner_id=(select id from User where user_name='%s') and id_code=%d;";
        sprintf(select_sql, format, username, code);
    }
    printf("select_sql: %s\n", select_sql);
    ret = mysql_query(connection, select_sql);
    if (ret)
    {
        printf("select failedd: %s\n", mysql_error(connection));
        mysql_close(connection);
        return -1;
    }
    // printf("select success,affected rows=%ld\n", (long)mysql_affected_rows(connection));
    if (NULL == (res = mysql_use_result(connection)))
    {
        printf("mysql_use_result() failed: %s\n", mysql_error(connection));
        mysql_close(connection);
        return -1;
    }
    if ((row = mysql_fetch_row(res)) != NULL)
    {
        if (0 == strcmp(row[1], "d"))
        {
            *p_code = atoi(row[0]);
        }
        else
        {
            *p_code = -1;
        }
    }
    else
    {
        *p_code = -1;
    }
    mysql_free_result(res);
    mysql_close(connection);
    return 0;
}
//检查当前用户当前网盘路径是否存在该文件
int select_mysql_for_current_path_find_file(const char *username, const char *filename, const int code, unsigned char *is_exist, char *md5_file)
{
    MYSQL *connection = NULL;
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    int ret = init_mysql(&connection);
    if (-1 == ret)
    {
        return -1;
    }
    char select_sql[200] = {0};
    char *format;

    format = "select md5sum from UserFile where owner_id=(select id from User where user_name='%s') and precode=%d and file_name='%s' and file_type='f';";
    sprintf(select_sql, format, username, code, filename);
    printf("select_sql: %s\n", select_sql);
    ret = mysql_query(connection, select_sql);
    if (ret)
    {
        printf("select failedd: %s\n", mysql_error(connection));
        mysql_close(connection);
        return -1;
    }
    // printf("select success,affected rows=%ld\n", (long)mysql_affected_rows(connection));
    if (NULL == (res = mysql_use_result(connection)))
    {
        printf("mysql_use_result() failed: %s\n", mysql_error(connection));
        mysql_close(connection);
        return -1;
    }
    if ((row = mysql_fetch_row(res)) != NULL)
    {
        *is_exist = 1;
        if (md5_file != NULL)
        {
            strcpy(md5_file, row[0]);
        }
    }
    else
    {
        *is_exist = 0;
    }
    mysql_free_result(res);
    mysql_close(connection);
    return 0;
}