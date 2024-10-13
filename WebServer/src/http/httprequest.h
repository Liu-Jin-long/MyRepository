#ifndef __HTTPREQUEST_H__
#define __HTTPREQUEST_H__
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <regex>
#include <errno.h>
#include <mysql/mysql.h> //mysql
#include "../buffer/buffer.h"
#include "../log/log.h"
#include "../pool/sqlconnpool.h"
#include "../pool/sqlconnRAII.h"
// HTTP请求: HTTP 协议通过请求和响应的形式进行通信，请求由请求行、请求头和请求体组成。
// 请求行: 包含 HTTP 方法、请求路径和 HTTP 版本。
// 请求头: 包含各种信息，如Content-Type、Connection等。
// 请求体: 在POST请求中用于传输数据。
class HttpRequest
{
public:
    enum PARSE_STATE
    {
        REQUEST_LINE,
        HEADERS,
        BODY,
        FINISH,
    };
    enum HTTP_CODE
    {
        NO_REQUEST,
        GET_REQUEST,
        BAD_REQUEST,
        NO_RESOURSE,
        FORBIDDENT_REQUEST,
        FILE_REQUEST,
        INTERNAL_ERROR,
        CLOSED_CONNECTION,
    };
    HttpRequest() { init(); };
    ~HttpRequest() = default;
    void init();
    bool parse(Buffer &buffer);
    std::string path() const;
    std::string &path();
    std::string method() const;
    std::string version() const;
    std::string getPost(const std::string &key) const;
    std::string getPost(const char *key) const;
    bool isKeepAlive() const;

private:
    bool parseRequestLine(const std::string &line);
    void parseHeader(const std::string &line);
    void parseBody(const std::string &line);
    void parsePath();
    void parsePost();
    void parseFormUrlencoded();
    static bool UserVerity(const std::string &name, const std::string &pwd, bool isLogin);

    PARSE_STATE m_parseState;
    std::string m_method, m_path, m_version, m_body;
    std::unordered_map<std::string, std::string> m_header;
    std::unordered_map<std::string, std::string> m_post;
    static const std::unordered_set<std::string> DEFAULT_HTML;
    static const std::unordered_map<std::string, int> DEFAULT_HTML_TAG;
    static int converHex(char c);
};

#endif