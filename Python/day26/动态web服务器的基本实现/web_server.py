#!/usr/bin/python3
import socket
import re
import multiprocessing
import time


# web_server类似于apache（Nginx），mini_frame类似于django

class WSGIServer():
    def __init__(self, port, documents_root, app):
        self.tcp_server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.tcp_server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.tcp_server_socket.bind(('', port))
        self.tcp_server_socket.listen(128)
        self.documents_root = documents_root
        # 设置web框架（django）可以调用的函数(对象)
        self.app = app

    def service_client(self, new_socket):
        """为客户端返回数据"""
        request = new_socket.recv(1024).decode('utf8')
        request_lines = request.splitlines()

        # GET /index.html HTTP/1.1
        file_name = ''
        ret = re.match(r'[^/]+(/[^ ]*)', request_lines[0])
        if ret:
            file_name = ret.group(1)
            if file_name == '/':
                file_name = '/index.html'

        # 返回http格式的数据，给浏览器
        if not file_name.endswith('.py'):
            try:
                f = open(self.documents_root + file_name, 'rb')
            except:
                response = 'HTTP/1.1 404 NOT FOUND\r\n'
                response += '\r\n'
                response += '-----file not found-----'
                new_socket.send(response.encode('utf8'))
            else:
                html_content = f.read()
                f.close()
                # 准备发送给浏览器的数据---header
                response = 'HTTP/1.1 200 OK\r\n'
                response += '\r\n'
                # 准备发送给浏览器的数据 - --body
                response += '这是HTML的body'
                new_socket.send(response.encode('utf8'))
                new_socket.send(html_content)
        else:
            # 如果是以.py结尾，那么就认为是动态资源的请求
            envir = dict()
            envir['PATH_INFO'] = file_name
            # 传入回调函数给application
            body = self.app(envir, self.set_response_header)
            header = 'HTTP/1.1 %s\r\n' % self.status
            header += "Content-Type: text/html; charset=utf-8\r\n"
            # Content-Length是编码后的字节流的长度
            header += 'Content-Length: %d\r\n' % len(body.encode('utf8'))
            # 解析头部
            for temp in self.headers:
                header += '%s:%s\r\n' % (temp[0], temp[1])
            header += '\r\n'
            response = header + body
            new_socket.send(response.encode('utf8'))
        new_socket.close()

    def run_forever(self):
        '''用来完成整体控制'''
        while True:
            new_socket, client_addr = self.tcp_server_socket.accept()
            p = multiprocessing.Process(target=self.service_client, args=(new_socket,))
            p.start()
            new_socket.close()
        self.tcp_server_socket.close()

    def set_response_header(self, status, headers):
        self.status = status
        # web服务器可以设置header，mini_frame也可以设置header
        self.headers = [('server', 'mini_web v5')]
        self.headers += headers


import sys


def main():
    """控制整体，创建一个web 服务器对象，然后调用这个对象的run_forever方法运行"""
    '''控制web服务器整体'''
    if len(sys.argv) == 3:
        port = sys.argv[1]
        if port.isdigit():
            port = int(port)
        # 获取web服务器需要动态资源时，访问的web框架名字
        web_frame_module_app_name = sys.argv[2]
    else:
        # windows下：python web_server.py 7890 mini_frame:application
        print('运行方式如: python3 xxx.py 7890 my_web_frame_name:application')
        return
    # 读取配置文件
    with open('web_server.conf') as f:
        conf_info = eval(f.read())
    g_static_document_root = conf_info['static_path']
    g_dynamic_document_root = conf_info['dynamic_path']
    # 将动态路径即存放py文件的路径，添加到path中以便能找到
    sys.path.append(g_dynamic_document_root)

    ret = re.match(r'([^:]*):(.*)', web_frame_module_app_name)
    if ret:
        # 获取模块名
        web_frame_module_name = ret.group(1)
        # 获取可以调用web框架的应用名称
        app_name = ret.group(2)
    # 导入web框架的主模块
    web_frame_module = __import__(web_frame_module_name)
    # 获取模块中可以直接调用的函数(对象)
    app = getattr(web_frame_module, app_name)
    print('HTTP服务器使用的port:%s' % port)
    WSGI_sever = WSGIServer(port, g_static_document_root, app)
    WSGI_sever.run_forever()


if __name__ == '__main__':
    main()
