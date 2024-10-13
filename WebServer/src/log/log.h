#ifndef __LOG_H__
#define __LOG_H__
#include <mutex>
#include <string>
#include <thread>
#include <sys/time.h>
#include <stdarg.h>
#include <assert.h>
#include <sys/stat.h>
#include "blockqueue.h"
#include "../buffer/buffer.h"
class Log
{
public:
    enum Level
    {
        UNKNOW = 0, // 起手先来个未知级别兜底
        DEBUG = 1,  // 调试级别
        INFO = 2,   // 普通信息级别
        WARN = 3,   // 警告信息
        ERROR = 4,  // 错误信息
        FATAL = 5,  // 灾难级信息
    };
    void init(Log::Level level = Log::Level::UNKNOW, const char *path = "./log", const char *logfile_suffix = ".log", int MaxQueueCapacity = 1024);
    static Log *Instance();
    static void FlushLogThread(); // m_writeThread执行函数
    void write(Log::Level level, const char *format, ...);
    void flush(); // 唤醒阻塞在m_deque的m_writeThread
    Log::Level GetLevel();
    void SetLevel(Log::Level level);
    bool IsOpen() { return m_isOpen; }

private:
    Log();
    void AppendLogLevelTitle(Log::Level level);
    virtual ~Log();
    void AsyncWrite();

private:
    static const int LOG_PATH_LEN = 256;
    static const int LOG_NAME_LEN = 256;
    static const int MAX_LINES = 50000;
    const char *m_path;
    const char *m_logfile_suffix;
    int m_maxLines;
    int m_lineCount;
    int m_toDay; // 日志当前日期，用于是否创建新日期的日志文件
    bool m_isOpen;
    Buffer m_buffer; // 1、写日志内容先写到m_buffer中
    Log::Level m_level;
    bool m_isAsync;
    FILE *m_fp;
    std::unique_ptr<BlockQueue<std::string>> m_deque; // 2、再根据是否异步将日志内容放入阻塞队列中
    std::unique_ptr<std::thread> m_writeThread;       // 3、flush唤醒m_writeThread取阻塞队列中的节点刷新到FILE缓冲中
    std::mutex m_mutex;
};

#define LOG_BASE(level, format, ...)                                                                  \
    do                                                                                                \
    {                                                                                                 \
        Log *log = Log::Instance();                                                                   \
        if (log->IsOpen() && log->GetLevel() <= level)                                                \
        {                                                                                             \
            log->write(level, "<%s:%s:%d> " format, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
            log->flush();                                                                             \
        }                                                                                             \
    } while (0);                                                                                      \
// C/C++连续多个字符串会自动拼接成一个，这里不能连着写，空格隔开才行
// __VA_ARGS__前面加上##的作用是：当可变参数的个数为0时，这里的##可以把前面多余的","去掉,否则会编译出错。
#define LOG_UNKNOW(format, ...)                             \
    do                                                      \
    {                                                       \
        LOG_BASE(Log::Level::UNKNOW, format, ##__VA_ARGS__) \
    } while (0);
#define LOG_DEBUG(format, ...)                             \
    do                                                     \
    {                                                      \
        LOG_BASE(Log::Level::DEBUG, format, ##__VA_ARGS__) \
    } while (0);
#define LOG_INFO(format, ...)                             \
    do                                                    \
    {                                                     \
        LOG_BASE(Log::Level::INFO, format, ##__VA_ARGS__) \
    } while (0);
#define LOG_WARN(format, ...)                             \
    do                                                    \
    {                                                     \
        LOG_BASE(Log::Level::WARN, format, ##__VA_ARGS__) \
    } while (0);
#define LOG_ERROR(format, ...)                             \
    do                                                     \
    {                                                      \
        LOG_BASE(Log::Level::ERROR, format, ##__VA_ARGS__) \
    } while (0);
#define LOG_FATAL(format, ...)                             \
    do                                                     \
    {                                                      \
        LOG_BASE(Log::Level::FATAL, format, ##__VA_ARGS__) \
    } while (0);
#endif
// #define LOG_UNKNOW(format, ...)  do{LOG_BASE(Log::Level::UNKNOW, format, ##__VA_ARGS__)} while (0);
// #define LOG_DEBUG(format, ...)  do{LOG_BASE(Log::Level::DEBUG, format, ##__VA_ARGS__)} while (0);
// #define LOG_INFO(format, ...)  do{LOG_BASE(Log::Level::INFO, format, ##__VA_ARGS__)} while (0);
// #define LOG_WARN(format, ...)  do{LOG_BASE(Log::Level::WARN, format, ##__VA_ARGS__)} while (0);
// #define LOG_ERROR(format, ...)  do{LOG_BASE(Log::Level::ERROR, format, ##__VA_ARGS__)} while (0);
// #define LOG_FATAL(format, ...)  do{LOG_BASE(Log::Level::FATAL, format, ##__VA_ARGS__)} while (0);