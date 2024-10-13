/*
 * @Author       : mark
 * @Date         : 2020-06-20
 * @copyleft Apache 2.0
 */ 
#include "../src/log/log.h"
#include "../src/pool/threadpool.h"
#include <features.h>

#if __GLIBC__ == 2 && __GLIBC_MINOR__ < 30
#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)
#endif

void TestLog() {
    int cnt = 0, level = 0;
    Log::Instance()->init(static_cast<Log::Level>(level), "./testlog1", ".log", 0);
    for(level = 3; level >= 0; level--) {
        Log::Instance()->SetLevel(static_cast<Log::Level>(level));
        for(int j = 0; j < 10000; j++ ){
            for(int i = 0; i < 4; i++) {
                LOG_BASE(static_cast<Log::Level>(i),"%s 111111111 %d ============= ", "Test", cnt++);
            }
        }
    }
    cnt = 0;
    Log::Instance()->init(static_cast<Log::Level>(level), "./testlog2", ".log", 5000);
    for(level = 0; level < 4; level++) {
        Log::Instance()->SetLevel(static_cast<Log::Level>(level));
        for(int j = 0; j < 10000; j++ ){
            for(int i = 0; i < 4; i++) {
                LOG_BASE(static_cast<Log::Level>(i),"%s 222222222 %d ============= ", "Test", cnt++);
            }
        }
    }
}

void ThreadLogTask(int i, int cnt) {
    for(int j = 0; j < 10000; j++ ){
        LOG_BASE(static_cast<Log::Level>(i),"PID:[%04d]======= %05d ========= ", gettid(), cnt++);
    }
}

void TestThreadPool() {
    Log::Instance()->init(static_cast<Log::Level>(0), "./testThreadpool", ".log", 5000);
    ThreadPool threadpool(6);
    for(int i = 0; i < 18; i++) {
        threadpool.addTask(std::bind(ThreadLogTask, i % 4, i * 10000));
    }
    getchar();
}

int main() {
    TestLog();
    TestThreadPool();
}