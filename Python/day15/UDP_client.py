# 编写于2022年06月15日13时42分14秒
import socket
import sys
client=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
dest_addr=(sys.argv[1],2000)
client.sendto('Hello World!'.encode('utf8'),dest_addr)
recv_data=client.recvfrom(5)
print(recv_data[0])
client.close()