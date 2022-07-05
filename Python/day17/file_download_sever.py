# 编写于2022年06月17日20时51分31秒
from socket import *
import time
import struct

sever = socket(AF_INET, SOCK_STREAM)
sever.bind(('', 2000))
sever.listen(10)
new1_socker, client_addr = sever.accept()
print(client_addr)
file_name = 'ReadMe.txt'
new1_socker.send(struct.pack('I', len(file_name)))
new1_socker.send(file_name.encode('utf-8'))

fd = open(file_name, 'rb')
content_bytes = fd.read()
new1_socker.send(struct.pack('I', len(content_bytes)))  # 'III',1,2,3 可以一起传多个
new1_socker.send(content_bytes)
fd.close()
time.sleep(3)
new1_socker.close()
sever.close()
