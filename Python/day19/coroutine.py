# 编写于2022年06月20日20时11分50秒
import random
import time
from greenlet import greenlet
import gevent
from gevent import monkey

# 有耗时操作时需要将其替换为gevent中已实现的模块  自动将python的一些标准模块替换成gevent框架
monkey.patch_all()


# 实现简单协程
def work1():
    while True:
        print("work1")
        yield
        time.sleep(0.5)


def work2():
    while True:
        print("work2")
        yield
        time.sleep(0.5)


def implement_simple_coroutines():
    w1 = work1()
    w2 = work2()
    while True:
        next(w1)
        next(w2)


# 使用greenlet协程库编写协程
def work3():
    while True:
        print('work3')
        gr2.switch()
        time.sleep(0.5)


def work4():
    while True:
        print('work4')
        gr1.switch()
        time.sleep(0.5)


# 使用gevent编写协程
def f(n):
    for i in range(n):
        print(gevent.getcurrent(), '-----', i)
        # gevent.sleep(1)  # 模拟耗时操作 自动调度其它协程运行
        time.sleep(random.random())


if __name__ == '__main__':
    # implement_simple_coroutines()

    gr1 = greenlet(work3)
    gr2 = greenlet(work4)
    gr1.switch()  # 切换到gr1开始运行  自行切换

    # g1 = gevent.spawn(f, 5)
    # g2 = gevent.spawn(f, 5)
    # g3 = gevent.spawn(f, 5)
    # g1.join()
    # g2.join()
    # g3.join()

    gevent.joinall([gevent.spawn(f, 10), gevent.spawn(f, 10), gevent.spawn(f, 10)])
