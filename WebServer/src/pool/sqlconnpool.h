#ifndef __SQLCONNPOOL_H__
#define __SQLCONNPOOL_H__
#include <mysql/mysql.h>
#include <string>
#include <queue>
#include <mutex>
#include <semaphore.h>
#include <thread>
#include "../log/log.h"
class SqlConnPool
{
public:
    static SqlConnPool *Instance();
    MYSQL *GetConn();
    void FreeConn(MYSQL *conn);
    int GetFreeConnCount();
    void Init(const char *host, int port, const char *user, const char *password, const char *databaseName, int connCount);
    void ClosePool();

private:
    SqlConnPool();
    ~SqlConnPool();
    int m_maxConnCount;
    int m_useCount;
    int m_freeCount;
    std::queue<MYSQL *> m_connQueue;
    std::mutex m_mutex;
    //连接池中最大可用的数据库连接数。确保在任意时候，最多只能有m_maxConnCount个线程获取连接
    sem_t m_semId;
};
#endif