# 编写于2022年06月17日21时21分03秒
import struct
from socket import *

client = socket(AF_INET, SOCK_STREAM)
client.connect(('192.168.5.30', 2000))
# 约定用4个字节表示长度
file_name_len = client.recv(4)
file_name = client.recv(struct.unpack('I', file_name_len)[0])  # unpack返回的是一个元组
fd = open(file_name.decode('utf-8') + '副本', 'wb')
file_content_len = client.recv(4)
file_content = client.recv(struct.unpack('I', file_content_len)[0])

fd.write(file_content)
fd.close()
client.close()
