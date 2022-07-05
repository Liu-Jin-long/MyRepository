# 编写于2022年06月16日19时35分56秒

from socket import *

sever = socket(AF_INET, SOCK_STREAM)
local_addr = ('', 2000)
sever.bind(local_addr)
sever.listen(10)  # __backlog是sever的缓冲区大小 sever套接字可以接受连接的客户端最大数量
new_socket1, client1_addr = sever.accept()  # 从sever套接字的缓冲区取出客户端连接请求 缓冲区为空(无连接请求)会堵塞
recv_data = new_socket1.recv(5)
print(recv_data)
new_socket1.send(b"I'm fine.")
recv_data = new_socket1.recv(5)
print(recv_data)
new_socket1.close()
sever.close()
