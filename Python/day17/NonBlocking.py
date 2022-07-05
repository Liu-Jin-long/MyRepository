# 编写于2022年06月17日17时53分55秒
from socket import *

sever = socket(AF_INET, SOCK_STREAM)
# 重用对应地址和端口
sever.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
sever.bind(('192.168.5.30', 2000))
sever.listen(10)
sever.setblocking(False)  # 修改缓冲区属性为非阻塞
new1_socket = None

while True:
    try:
        new1_socket, client_addr = sever.accept()
    except Exception as e:
        # print(f'accept:{e}')
        pass
    if new1_socket:
        new1_socket.setblocking(False)
        try:
            data = new1_socket.recv(100)
            if not data:
                print('对方断开')
                # exit(0)
            print(data.decode('utf-8'))
        except Exception as e:
            print(f'recv:{e}')
