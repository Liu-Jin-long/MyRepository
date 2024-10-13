#ifndef __SQLCONNRAII_H__
#define __SQLCONNRAII_H__
#include "sqlconnpool.h"
class SqlConnRAII
{
public:
    //  从连接池申请Sql连接资源
    SqlConnRAII(MYSQL **sql, SqlConnPool *connPool)
    {
        assert(connPool);
        *sql = connPool->GetConn();
        m_sql = *sql;
        m_connPool = connPool;
    }
    //  归还Sql连接资源回连接池
    ~SqlConnRAII()
    {
        if (m_sql)
        {
            m_connPool->FreeConn(m_sql);
        }
    }

private:
    MYSQL *m_sql;
    SqlConnPool *m_connPool;
};
#endif