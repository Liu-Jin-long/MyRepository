# 编写于2022年06月18日14时57分01秒
from multiprocessing import Process, Queue
import time


# q = Queue(3)
# q.put('消息1')
# q.put('消息2')
# q.put("消息3")
# print(q.full())
# print(q.empty())
# print(q.get())
# print(q.get())
# print(q.get())
# # print(q.get(block=False))  # 为空直接抛出异常
# # print(q.get())  # 阻塞
# # print(q.get(block=True, timeout=2))  # 为空timeout后抛出异常


def writer(q: Queue):
    for i in range(5):
        print(f'写入{i}')
        q.put(i)
        time.sleep(1)


def reader(q: Queue):
    while True:
        if not q.empty():
            data = q.get()
            print(f'读出{data}')
            time.sleep(1)
        else:
            break


if __name__ == '__main__':
    q = Queue()  # 无参数默认最大
    pw = Process(target=writer, args=(q,))
    pw.start()
    time.sleep(1)
    pr = Process(target=reader, args=(q,))
    pr.start()
    pw.join()
    pr.join()
