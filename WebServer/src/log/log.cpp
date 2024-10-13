#include "log.h"
Log::Log()
{
    m_lineCount = 0;
    m_isAsync = false;
    m_writeThread = nullptr;
    m_deque = nullptr;
    m_toDay = 0;
    m_fp = nullptr;
}

Log::Level Log::GetLevel()
{
    std::lock_guard<std::mutex> locker(m_mutex);
    return m_level;
}
void Log::SetLevel(Log::Level level)
{
    std::lock_guard<std::mutex> locker(m_mutex);
    m_level = level;
}
void Log::init(Log::Level level, const char *path, const char *logfile_suffix, int MaxQueueCapacity)
{
    m_isOpen = true;
    m_level = level;
    if (MaxQueueCapacity > 0)
    {
        m_isAsync = true;
        if (!m_deque)
        {
            std::unique_ptr<BlockQueue<std::string>> newDeque(new BlockQueue<std::string>);
            m_deque = std::move(newDeque);

            std::unique_ptr<std::thread> newThread(new std::thread(FlushLogThread));
            m_writeThread = std::move(newThread);
        }
    }
    else
    {
        m_isAsync = false;
    }
    m_lineCount = 0;
    time_t timer = time(nullptr);
    struct tm sysTime = {0};
    localtime_r(&timer, &sysTime);
    m_path = path;
    m_logfile_suffix = logfile_suffix;
    char filename[LOG_NAME_LEN] = {0};
    snprintf(filename, LOG_NAME_LEN - 1, "%s/%04d_%02d_%02d%s", m_path, sysTime.tm_year + 1900, sysTime.tm_mon + 1, sysTime.tm_mday, m_logfile_suffix);
    m_toDay = sysTime.tm_mday;
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        m_buffer.RetrieveAll();
        if (m_fp)
        {
            flush();
            fclose(m_fp);
            m_fp = nullptr;
        }
        m_fp = fopen(filename, "a");
        if (m_fp == nullptr)
        {
            mkdir(m_path, 0777);
            m_fp = fopen(filename, "a");
        }
        assert(m_fp != nullptr);
    }
}
void Log::write(Log::Level level, const char *format, ...)
{
    struct timeval now = {0};
    gettimeofday(&now, nullptr);
    time_t tSec = now.tv_sec;
    struct tm sysTime = {0};
    localtime_r(&tSec, &sysTime);
    // 日志日期变化 日志行数溢出
    if (m_toDay != sysTime.tm_mday || (m_lineCount && (m_lineCount % MAX_LINES) == 0))
    {

        char newfile[LOG_NAME_LEN] = {0};
        char tail[36] = {0};
        snprintf(tail, sizeof(tail), "%04d_%02d_%02d", sysTime.tm_year + 1900, sysTime.tm_mon + 1, sysTime.tm_mday);
        if (m_toDay != sysTime.tm_mday)
        {
            snprintf(newfile, LOG_NAME_LEN - 72, "%s/%s%s", m_path, tail, m_logfile_suffix);
            m_toDay = sysTime.tm_mday;
            m_lineCount = 0;
        }
        else
        {
            snprintf(newfile, LOG_NAME_LEN - 72, "%s/%s-%d%s", m_path, tail, m_lineCount / MAX_LINES, m_logfile_suffix);
        }
        printf("创建newfile is %s \n", newfile);
        std::lock_guard<std::mutex> locker(m_mutex);
        flush();
        fclose(m_fp);
        m_fp = fopen(newfile, "a");
        assert(m_fp != nullptr);
    }
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_lineCount++;
        int n = snprintf(m_buffer.BeginWrite(), 128, "%d-%02d-%02d %02d:%02d:%02d.%06ld ",
                         sysTime.tm_year + 1900, sysTime.tm_mon + 1, sysTime.tm_mday, sysTime.tm_hour, sysTime.tm_min, sysTime.tm_sec, now.tv_usec);
        m_buffer.AddWritePos(n);
        AppendLogLevelTitle(level);

        va_list valist;
        va_start(valist, format);
        int m = vsnprintf(m_buffer.BeginWrite(), m_buffer.WritableBytes(), format, valist);
        va_end(valist);

        m_buffer.AddWritePos(m);
        m_buffer.Append("\n\0", 2);

        // 使用缓冲区的异步刷新
        if (m_isAsync && m_deque && !m_deque->full())
        {
            m_deque->push_back(m_buffer.RetrieveAllToStr());
        }
        else // 同步刷新
        {
            fputs(m_buffer.Peek(), m_fp);
        }
        m_buffer.RetrieveAll(); // 清除缓冲区内容
    }
}
void Log::AppendLogLevelTitle(Log::Level level)
{
    switch (level)
    {
    case Log::UNKNOW:
        m_buffer.Append("[unknow]: ", 10);
        break;
    case Log::DEBUG:
        m_buffer.Append("[debug] : ", 10);
        break;
    case Log::INFO:
        m_buffer.Append("[info]  : ", 10);
        break;
    case Log::WARN:
        m_buffer.Append("[warn]  : ", 10);
        break;
    case Log::ERROR:
        m_buffer.Append("[error] : ", 10);
        break;
    case Log::FATAL:
        m_buffer.Append("[fatal] : ", 10);
        break;
    }
}
void Log::flush()
{
    if (m_isAsync)
    {
        m_deque->flush();
    }
    // 调用fflush()会将FILE缓冲区中的内容写到stream所指的文件中去.若stream为NULL，则会将所有打开的文件进行数据同步。
    fflush(m_fp);
}
void Log::AsyncWrite()
{
    std::string str = "";
    while (m_deque->pop(str))
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        printf("日志线程:%s", str.data());
        fputs(str.c_str(), m_fp);
    }
}
Log *Log::Instance()
{
    static Log instance;
    return &instance;
}
void Log::FlushLogThread()
{
    Log::Instance()->AsyncWrite();
}
Log::~Log()
{
    if (m_writeThread && m_writeThread->joinable())
    {
        while (!m_deque->empty())
        {
            printf("~Log m_deque->flush()\n");
            m_deque->flush();
        }
        m_deque->close();
        printf("join前\n");
        m_writeThread->join();
        printf("join后\n");
    }
    if (m_fp)
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        flush(); // 缓冲区内容刷新同步到FILE缓冲中
        fclose(m_fp);
    }
}