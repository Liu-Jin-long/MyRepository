# 编写于2022年06月20日21时40分07秒
import gevent

def add():
    global num
    for i in range(1000000):
        num+=1
num=0
t1=gevent.spawn(add)
t2=gevent.spawn(add)
t1.join()
t2.join()
print(num)