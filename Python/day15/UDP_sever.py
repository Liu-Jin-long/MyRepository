# 编写于2022年06月15日13时41分45秒
from socket import *
sever=socket(AF_INET,SOCK_DGRAM)
local_addr=('',2000)
sever.bind(local_addr)
recv_data=sever.recvfrom(1024)
print(recv_data[0].decode('utf8'))
print(recv_data[1])
sever.sendto("I'm sever".encode('utf8'),recv_data[1])
sever.close()