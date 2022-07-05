# 编写于2022年06月20日17时23分31秒
import threading
from threading import Thread, Lock

g_num = 0


def thread_do(mutex):
    global g_num
    for i in range(1000000):
        mutex.acquire()
        g_num += 1
        mutex.release()


def main():
    mutex = Lock()
    th1 = Thread(target=thread_do, args=(mutex,))
    th1.start()
    th2 = Thread(target=thread_do, args=(mutex,))
    th2.start()

    print(threading.enumerate())  # 返回当前存活的线程列表
    th1.join()
    th2.join()
    print(g_num)


if __name__ == '__main__':
    main()
