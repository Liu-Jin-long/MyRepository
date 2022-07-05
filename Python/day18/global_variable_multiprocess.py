from multiprocessing import Process

list1 = [1, 2, 3]  # 全局变量


def child_process():
    list1.append(10)
    print(f'子进程list1={list1}')  # 子进程list1=[1, 2, 3, 10]


if __name__ == '__main__':
    p = Process(target=child_process)
    p.start()
    list1.pop()
    print(f'父进程list1={list1}')  # 父进程list1=[1, 2]
