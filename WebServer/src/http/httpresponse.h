#ifndef __HTTPRESPONSE_H__
#define __HTTPRESPONSE_H__

#include <unordered_map>
#include <fcntl.h>    // open
#include <unistd.h>   // close
#include <sys/stat.h> // stat
#include <sys/mman.h> // mmap, munmap
#include "../buffer/buffer.h"
#include "../log/log.h"

class HttpResponse
{
public:
    HttpResponse();
    ~HttpResponse();
    void init(const std::string &srcDir, std::string &path, bool isKeepAlive = false, int code = -1);
    void makeResponse(Buffer &buffer);
    void unmapFile();
    char *file();
    size_t fileLen() const;
    void errorContent(Buffer &buffer, std::string message);
    int code() const { return m_code; }

private:
    void addStateLine(Buffer &buffer);
    void addHeader(Buffer &buffer);
    void addContent(Buffer &buffer);
    void errorHtml();
    std::string getFileType();
    int m_code;
    bool m_isKeepAlive;
    std::string m_path;
    std::string m_srcDir;
    char *m_mmapFile;
    struct stat m_mmapFileStat;
    // 文件后缀与类型映射
    static const std::unordered_map<std::string, std::string> SUFFIX_TYPE;
    // 状态码与消息映射
    static const std::unordered_map<int, std::string> CODE_STATUS;
    // 状态码与错误页面路径映射
    static const std::unordered_map<int, std::string> CODE_PATH;
};
#endif