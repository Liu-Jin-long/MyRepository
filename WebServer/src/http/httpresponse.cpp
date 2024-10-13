#include "httpresponse.h"
const std::unordered_map<std::string, std::string> HttpResponse::SUFFIX_TYPE = {
    {".html", "text/html"},
    {".xml", "text/xml"},
    {".xhtml", "application/xhtml+xml"},
    {".txt", "text/plain"},
    {".rtf", "application/rtf"},
    {".pdf", "application/pdf"},
    {".word", "application/nsword"},
    {".png", "image/png"},
    {".gif", "image/gif"},
    {".jpg", "image/jpeg"},
    {".jpeg", "image/jpeg"},
    {".au", "audio/basic"},
    {".mpeg", "video/mpeg"},
    {".mpg", "video/mpeg"},
    {".avi", "video/x-msvideo"},
    {".gz", "application/x-gzip"},
    {".tar", "application/x-tar"},
    {".css", "text/css "},
    {".js", "text/javascript "},
};
const std::unordered_map<int, std::string> HttpResponse::CODE_STATUS = {
    {200, "OK"},
    {400, "Bad Request"},
    {403, "Forbidden"},
    {404, "Not Found"},
};

const std::unordered_map<int, std::string> HttpResponse::CODE_PATH = {
    {400, "/400.html"},
    {403, "/403.html"},
    {404, "/404.html"},
};
HttpResponse::HttpResponse()
{
    m_code = -1; // 默认状态码为 -1，表示未设置状态
    m_path = m_srcDir = "";
    m_isKeepAlive = false;
    m_mmapFile = nullptr;
    m_mmapFileStat = {0};
};
HttpResponse::~HttpResponse()
{
    unmapFile();
}
void HttpResponse::init(const std::string &srcDir, std::string &path, bool isKeepAlive, int code)
{
    assert(srcDir != "");
    if (m_mmapFile)
    {
        unmapFile();
    }
    m_code = code;
    m_isKeepAlive = isKeepAlive;
    m_path = path;
    m_srcDir = srcDir;
    m_mmapFile = nullptr;
    m_mmapFileStat = {0};
}
void HttpResponse::makeResponse(Buffer &buffer)
{
    // 请求的资源文件
    if (stat((m_srcDir + m_path).data(), &m_mmapFileStat) < 0 || S_ISDIR(m_mmapFileStat.st_mode))
    {
        m_code = 404; // 如果文件不存在或是目录，设置状态码为404
    }
    else if (!(m_mmapFileStat.st_mode & S_IROTH))
    {
        m_code = 403; // 如果没有读取权限，设置状态码为403
    }
    else if (m_code == -1)
    {
        m_code = 200; // 如果状态码未设置，则设置为200
    }
    errorHtml();
    addStateLine(buffer);
    addHeader(buffer);
    addContent(buffer);
}
char *HttpResponse::file()
{
    return m_mmapFile;
}
size_t HttpResponse::fileLen() const
{
    return m_mmapFileStat.st_size;
}
void HttpResponse::errorHtml()
{
    if (CODE_PATH.count(m_code) == 1)
    {
        m_path = CODE_PATH.find(m_code)->second;
        stat((m_srcDir + m_path).data(), &m_mmapFileStat);
    }
}
void HttpResponse::addStateLine(Buffer &buffer)
{
    // HTTP/1.1 200 OK 结构化响应状态行
    std::string status;
    if (CODE_STATUS.count(m_code) == 1)
    {
        status = CODE_STATUS.find(m_code)->second;
    }
    else
    {
        m_code = 400;
        status = CODE_STATUS.find(m_code)->second;
    }
    buffer.Append("HTTP/1.1 " + std::to_string(m_code) + " " + status + "\r\n");
}
void HttpResponse::addHeader(Buffer &buffer)
{
    buffer.Append("Connection: ");
    if (m_isKeepAlive)
    {
        buffer.Append("keep-alive\r\n");
        buffer.Append("keep-alive: max=6, timeout=120\r\n");
    }
    else
    {
        buffer.Append("close\r\n");
    }
    // 添加其他响应头，比如内容类型
    buffer.Append("Content-type: " + getFileType() + "\r\n");
}
void HttpResponse::addContent(Buffer &buffer)
{
    // 添加响应内容
    int fd = open((m_srcDir + m_path).data(), O_RDONLY);
    if (fd < 0)
    {
        errorContent(buffer, "File NotFound!");
        return;
    }
    LOG_DEBUG("file path %s", (m_srcDir + m_path).data());
    char *file = (char *)mmap(nullptr, m_mmapFileStat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file == (char *)-1)
    {
        errorContent(buffer, "File NotFound!");
        return;
    }
    m_mmapFile = file;
    close(fd);
    buffer.Append("Content-length: " + std::to_string(m_mmapFileStat.st_size) + "\r\n\r\n");
}
void HttpResponse::unmapFile()
{
    if (m_mmapFile)
    {
        munmap(m_mmapFile, m_mmapFileStat.st_size);
        m_mmapFile = nullptr;
    }
}
std::string HttpResponse::getFileType()
{
    std::string::size_type idx = m_path.find_last_of('.');
    if (idx == std::string::npos)
    {
        return "text/plain";
    }
    std::string suffix = m_path.substr(idx);
    if (SUFFIX_TYPE.count(suffix) == 1)
    {
        return SUFFIX_TYPE.find(suffix)->second;
    }
    return "text/plain";
}
void HttpResponse::errorContent(Buffer &buffer, std::string message)
{
    std::string body;
    std::string status;
    // 生成 HTML格式的错误响应
    body += "<html><title>Error</title>";
    body += "<body bgcolor=\"ffffff\">";
    if (CODE_STATUS.count(m_code) == 1)
    {
        status = CODE_STATUS.find(m_code)->second;
    }
    else
    {
        status = "Bad Request";
    }
    body += std::to_string(m_code) + " : " + status + "\n";
    body += "<p>" + message + "</p>";
    body += "<hr><em>TinyWebServer</em></body></html>";
    buffer.Append("Content-length: " + std::to_string(body.size()) + "\r\n\r\n");
    buffer.Append(body);
}