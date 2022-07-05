# 编写于2022年06月18日15时18分49秒
from multiprocessing.pool import Pool
import os, time, random


def worker(task):
    start = time.time()
    print('%s开始工作，pid=%d' %(task, os.getpid()))
    time.sleep(random.random() * 2)  # 随机返回[0.0,1.0]范围内的一个浮点数
    end = time.time()
    print('%s结束工作，pid=%d，耗时%.2f' %(task, os.getpid(), end - start))


if __name__ == '__main__':
    pool = Pool(3)  # 创建进程数
    for i in range(10):
        pool.apply_async(worker, (i,))  # i任务分配给子进程处理
    print('start')
    pool.close()  # 关闭进程池，关闭后apply_async不再接受新的任务请求
    # pool.terminate()  # 立即关闭进程池和终止所有子进程
    print('close')
    pool.join()  # 等待进程池中所有子进程完成队列里的所有任务 必须放在进程池关闭后
    print('end')
