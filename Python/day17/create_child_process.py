# 编写于2022年06月17日21时50分57秒
import smtplib
from multiprocessing import Process
import time
def child_process():
    while True:
        print('我是子进程')
        time.sleep(1)

if __name__ == '__main__':
    child1=Process(target=child_process)
    child1.start()
    while True:
        print('我是父进程')
        time.sleep(1)

