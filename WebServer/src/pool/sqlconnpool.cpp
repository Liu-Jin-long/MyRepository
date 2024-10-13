#include "sqlconnpool.h"
SqlConnPool::SqlConnPool()
{
    m_useCount = 0;
    m_freeCount = 0;
}
SqlConnPool *SqlConnPool::Instance()
{
    static SqlConnPool connPool;
    return &connPool;
}
void SqlConnPool::Init(const char *host, int port, const char *user, const char *password, const char *databaseName, int connCount)
{
    assert(connCount > 0);
    for (int i = 0; i < connCount; i++)
    {
        MYSQL *sql = nullptr;
        sql = mysql_init(sql);
        if (!sql)
        {
            LOG_ERROR("MySql init error!");
            assert(sql);
        }
        sql = mysql_real_connect(sql, host, user, password, databaseName, port, nullptr, 0);
        if (!sql)
        {
            LOG_ERROR("MySql Connect error!");
            mysql_close(sql); // Close this instance to avoid memory leak
        }
        m_connQueue.push(sql);
    }
    m_maxConnCount = connCount;
    // 确保在任意时候，最多只能有m_maxConnCount个线程获取连接
    sem_init(&m_semId, 0, m_maxConnCount);
}
MYSQL *SqlConnPool::GetConn()
{
    MYSQL *sql = nullptr;
    if (m_connQueue.empty())
    {
        LOG_WARN("SqlConnPool busy!");
        return nullptr;
    }
    sem_wait(&m_semId);
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        sql = m_connQueue.front();
        m_connQueue.pop();
    }
    return sql;
}
void SqlConnPool::FreeConn(MYSQL *sql)
{
    assert(sql);
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        m_connQueue.push(sql);
    }
    sem_post(&m_semId);
}
void SqlConnPool::ClosePool()
{
    std::lock_guard<std::mutex> locker(m_mutex);
    while (!m_connQueue.empty())
    {
        auto item = m_connQueue.front();
        m_connQueue.pop();
        mysql_close(item);
    }
    mysql_library_end(); // Clean up MySQL library
}
int SqlConnPool::GetFreeConnCount()
{
    std::lock_guard<std::mutex> locker(m_mutex);
    return m_connQueue.size();
}
SqlConnPool::~SqlConnPool()
{
    ClosePool();
}