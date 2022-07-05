# 编写于2022年06月16日19时38分25秒
# 想说就说
from socket import *
import sys
import select

client = socket(AF_INET, SOCK_STREAM)
dest_addr = (sys.argv[1], 2000)
client.connect(dest_addr)
epoll = select.epoll()
epoll.register(sys.stdin.fileno(), select.EPOLLIN)
epoll.register(client.fileno(), select.EPOLLIN)

while True:
    events=epoll.poll(-1,2)
    for fd,event in events:
        if fd==sys.stdin.fileno():
            data=input("请输入：")
            client.send(data.encode('utf8'))
        elif fd==client.fileno():
            data=client.recv(100)
            if not data:
                print('对方已断开！')
                exit()
            print(data.decode('utf8'))
client.close()
