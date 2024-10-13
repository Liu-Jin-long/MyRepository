#include "httprequest.h"
const std::unordered_set<std::string> HttpRequest::DEFAULT_HTML{
    "/index",
    "/register",
    "/login",
    "/welcome",
    "/video",
    "/picture",
};
const std::unordered_map<std::string, int> HttpRequest::DEFAULT_HTML_TAG{
    {"/register.html", 0},
    {"/login.html", 1},
};
void HttpRequest::init()
{
    m_method = m_path = m_version = m_body = "";
    m_parseState = PARSE_STATE::REQUEST_LINE;
    m_header.clear();
    m_post.clear();
}
bool HttpRequest::isKeepAlive() const
{
    if (m_header.count("Connection") == 1)
    {
        return m_header.find("Connection")->second == "keep-alive" && m_version == "1.1";
    }
    return false;
}
bool HttpRequest::parse(Buffer &buffer)
{
    const char CRLF[] = "\r\n";
    if (buffer.ReadableBytes() <= 0)
    {
        return false;
    }
    while (buffer.ReadableBytes() && m_parseState != PARSE_STATE::FINISH)
    {
        const char *lineEnd = std::search(buffer.Peek(), buffer.BeginWriteConst(), CRLF, CRLF + strlen(CRLF));
        std::string line(buffer.Peek(), lineEnd);
        switch (m_parseState)
        {
        case PARSE_STATE::REQUEST_LINE:
            if (!parseRequestLine(line))
            {
                return false;
            }
            parsePath();
            break;
        case PARSE_STATE::HEADERS:
            parseHeader(line);
            if (buffer.ReadableBytes() <= strlen(CRLF))
            {
                m_parseState = PARSE_STATE::FINISH;
            }
            break;
        case PARSE_STATE::BODY:
            parseBody(line);
            break;
        default:
            break;
        }
        if (lineEnd == buffer.BeginWrite())
        {
            break;
        }
        buffer.RetrieveUntil(lineEnd + strlen(CRLF));
    }
    LOG_DEBUG("[%s], [%s], [%s]", m_method.c_str(), m_path.c_str(), m_version.c_str());
    return true;
}
void HttpRequest::parsePath()
{
    if (m_path == "/")
    {
        m_path = "/index.html";
    }
    else
    {
        for (auto &item : DEFAULT_HTML)
        {
            if (item == m_path)
            {
                m_path += ".html";
                break;
            }
        }
    }
}
// 请求行解析 使用正则表达式从请求行中解析出 方法、路径和HTTP版本
bool HttpRequest::parseRequestLine(const std::string &line)
{
    std::regex pattern("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");
    std::smatch subMatch;
    if (regex_match(line, subMatch, pattern))
    {
        m_method = subMatch[1];
        m_path = subMatch[2];
        m_version = subMatch[3];
        m_parseState = PARSE_STATE::HEADERS;
        return true;
    }
    LOG_ERROR("RequestLine Error");
    return false;
}
void HttpRequest::parseHeader(const std::string &line)
{
    std::regex pattern("^([^:]*): ?(.*)$");
    std::smatch subMatch;
    if (regex_match(line, subMatch, pattern))
    {
        m_header[subMatch[1]] = subMatch[2];
    }
    else
    {
        m_parseState = PARSE_STATE::BODY;
    }
}
void HttpRequest::parseBody(const std::string &line)
{
    m_body = line;
    parsePost();
    m_parseState = PARSE_STATE::FINISH;
    LOG_DEBUG("Body:%s, len:%d", line.c_str(), line.size());
}
int HttpRequest::converHex(char c)
{
    if (c >= 'A' && c <= 'F')
    {
        return c - 'A' + 10;
    }
    if (c >= 'a' && c <= 'f')
    {
        return c - 'a' + 10;
    }
    return c;
}
// POST Body数据解析 解析URL编码的表单数据（application/x-www-form-urlencoded格式）
void HttpRequest::parsePost()
{
    if (m_method == "POST" && m_header["Content-Type"] == "application/x-www-form-urlencoded")
    {
        parseFormUrlencoded();
        if (DEFAULT_HTML_TAG.count(m_path))
        {
            int tag = DEFAULT_HTML_TAG.find(m_path)->second;
            LOG_DEBUG("Tag:%d", tag);
            if (tag == 0 || tag == 1)
            {
                bool isLogin = (tag == 1);
                if (UserVerity(m_post["username"], m_post["password"], isLogin))
                {
                    m_path = "/welcome.html";
                }
                else
                {
                    m_path = "/error.html";
                }
            }
        }
    }
}
void HttpRequest::parseFormUrlencoded()
{
    if (m_body.size() == 0)
    {
        return;
    }
    std::string key, value;
    int n = m_body.size();
    int i, j = 0, num;
    char c;
    for (i = 0; i < n; i++)
    {
        c = m_body[i];
        switch (c)
        {
        case '=':
            key = m_body.substr(j, i - j);
            j = i + 1;
            break;
        case '+':
            m_body[i] = ' ';
            break;
        case '%':
            num = converHex(m_body[i + 1]) * 16 + converHex(m_body[i + 2]);
            m_body[i + 2] = num % 10 + '0';
            m_body[i + 1] = num / 10 + '0';
            i += 2;
            break;
        case '&':
            value = m_body.substr(j, i - j);
            j = i + 1;
            m_post[key] = value;
            LOG_DEBUG("%s = %s", key.c_str(), value.c_str());
            break;
        default:
            break;
        }
    }
    assert(j <= i);
    if (m_post.count(key) == 0 && j < i)
    {
        value = m_body.substr(j, i - j);
        m_post[key] = value;
    }
}
bool HttpRequest::UserVerity(const std::string &name, const std::string &pwd, bool isLogin)
{
    if (name == "" || pwd == "")
    {
        return false;
    }
    LOG_INFO("Verify name:%s pwd:%s", name.c_str(), pwd.c_str());
    MYSQL *sql;
    SqlConnRAII(&sql, SqlConnPool::Instance());
    assert(sql);
    bool flag = false;
    unsigned int j = 0;
    char order[256] = {0};
    MYSQL_FIELD *fields = nullptr;
    MYSQL_RES *res = nullptr;
    if (!isLogin)
    {
        flag = true;
    }
    snprintf(order, 256, "SELECT username, password FROM user WHERE username='%s' LIMIT 1", name.c_str());
    LOG_DEBUG("%s", order);
    if (mysql_query(sql, order))
    {
        mysql_free_result(res);
        return false;
    }
    res = mysql_store_result(sql);
    j = mysql_num_fields(res);
    fields = mysql_fetch_fields(res);
    while (MYSQL_ROW row = mysql_fetch_row(res))
    {
        LOG_DEBUG("MYSQL ROW: %s %s", row[0], row[1]);
        std::string password(row[1]);
        // 登录
        if (isLogin)
        {
            if (pwd == password)
            {
                flag = true;
            }
            else
            {
                flag = false;
                LOG_DEBUG("pwd error!");
            }
        }
        else
        {
            flag = false;
            LOG_DEBUG("user used!");
        }
    }
    mysql_free_result(res);
    // 注册且用户名未被使用
    if (!isLogin && flag == true)
    {
        LOG_DEBUG("regirster!");
        bzero(order, sizeof(order));
        snprintf(order, sizeof(order), "INSERT INTO user(username, password) VALUES('%s','%s')", name.c_str(), pwd.c_str());
        LOG_DEBUG("%s", order);
        if (mysql_query(sql, order))
        {
            LOG_DEBUG("Insert error!");
            flag = false;
        }
        flag = true;
    }
    SqlConnPool::Instance()->FreeConn(sql);
    LOG_DEBUG("UserVerify success!!");
    return flag;
}
std::string HttpRequest::path() const
{
    return m_path;
}
std::string &HttpRequest::path()
{
    return m_path;
}
std::string HttpRequest::method() const
{
    return m_method;
}
std::string HttpRequest::version() const
{
    return m_version;
}
std::string HttpRequest::getPost(const std::string &key) const
{
    assert(key != "");
    if (m_post.count(key) == 1)
    {
        return m_post.find(key)->second;
    }
    return "";
}
std::string HttpRequest::getPost(const char *key) const
{
    assert(key != nullptr);
    if (m_post.count(key) == 1)
    {
        return m_post.find(key)->second;
    }
    return "";
}
