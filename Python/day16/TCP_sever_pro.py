# 编写于2022年06月16日19时35分56秒
# 想说就说
from socket import *
import sys
import select

sever = socket(AF_INET, SOCK_STREAM)
local_addr = ('', 2000)
sever.bind(local_addr)
sever.listen(10)  # __backlog是sever的缓冲区大小 sever套接字可以接受连接的客户端最大数量
new_socket1, client1_addr = sever.accept()  # 从sever套接字的缓冲区取出客户端连接请求 缓冲区为空(无连接请求)会堵塞

epoll = select.epoll()
epoll.register(sys.stdin.fileno(), select.EPOLLIN)
epoll.register(new_socket1.fileno(), select.EPOLLIN)
# print(sys.stdin.fileno())
# print(sys.stdin.fileno(new_socket1.fileno()))
# print(epoll.fileno())
while True:
    events = epoll.poll(-1, 2)
    for fd, event in events:
        if fd == sys.stdin.fileno():
            data = input('请输入：')
            new_socket1.send(data.encode('utf8'))
        elif fd == new_socket1.fileno():
            data = new_socket1.recv(100)
            if not data:
                print("对方已断开！")
                exit()
            print(data.decode('utf8'))

new_socket1.close()
sever.close()
