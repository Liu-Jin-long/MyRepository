# 编写于2022年06月18日14时47分45秒
from multiprocessing import Process
from time import sleep
import os


# def child_proc(*args, **kwargs):
def child_proc(var1, var2, age=None, score=None):
    while True:
        print('子进程pid=%d'.format(os.getpid()))
        # print(args, kwargs)
        print(var1, var2, age, score)
        sleep(1)


if __name__ == '__main__':
    child1 = Process(target=child_proc, name='别名', args=('liu', 178), kwargs={'age': 22, 'score': 100})
    child1.start()
    child1.join(2)  # 主动回收 无参数永久等待 有参数数等待相应时间后这句就没用了，继续往下执行
    print('join2秒后：1')
    print('join2秒后：2')
    print('join2秒后：3')
    child1.terminate()  # 直接终止子进程
    print(f'此时{child1.name}子进程变为zombie进程')
    sleep(10)
    print('父进程sleep10秒后')
    print(f'父进程主动回收{child1.name}子进程')
    child1.join()
    sleep(10)
