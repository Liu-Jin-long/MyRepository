# 编写于2022年06月20日17时38分10秒
import time
from threading import Thread, Lock


class MyThread1(Thread):
    def run(self):
        mutex1.acquire()
        print(self.name + '获得mutex1锁')
        time.sleep(1)
        mutex2.acquire()
        print(self.name + '获得mutex2锁')
        mutex2.release()
        mutex1.release()


class MyThread2(Thread):
    def run(self):
        mutex2.acquire()
        print(self.name + '获得mutex2锁')

        mutex1.acquire()
        print(self.name + '获得mutex1锁')
        mutex1.release()
        mutex2.release()


if __name__ == '__main__':
    mutex1 = Lock()
    mutex2 = Lock()
    th1 = MyThread1()
    th2 = MyThread2()
    th1.start()
    th2.start()
