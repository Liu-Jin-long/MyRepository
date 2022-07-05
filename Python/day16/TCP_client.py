# 编写于2022年06月16日19时38分25秒
from socket import *
import sys

client = socket(AF_INET, SOCK_STREAM)
dest_addr = (sys.argv[1], 2000)
client.connect(dest_addr)
client.send(b'How are you?')
# recv_data = client.recv(10)
# print(recv_data)
while True:
    pass
client.close()
