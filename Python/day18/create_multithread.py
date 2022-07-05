# 编写于2022年06月18日15时57分55秒
from threading import Thread
import time

def SayHello():
    while True:
        print('Hello')
        time.sleep(2)
if __name__ == '__main__':
    for i in range(5):
        t=Thread(target=SayHello())
        t.start()