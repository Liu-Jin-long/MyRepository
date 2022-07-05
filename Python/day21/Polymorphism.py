# 编写于2022年06月22日16时14分14秒
class MiniOS():
    '''MiniOS操作系统类'''
    def __init__(self,name):
        self.name=name
        self.app_list=[]
    def __str__(self):
        return f'{self.name}已安装的软件列表：{self.app_list}'
    def install_app(self,app):
        if app.name in self.app_list:
            print(f'已经安装了{app.name},无需再次安装')
        else:
            app.install()
            self.app_list.append(app.name)
class App():
    def __init__(self,name,verson):
        self.name=name
        self.verson=verson
    def __str__(self):
        return f'{self.name}的当前版本{self.verson}'
    def install(self):
        print(f'{self.name}{self.verson}开始安装...')

class PyCharm(App):
    def install(self):
        print('正在解压安装程序...')
        super().install()
class Edge(App):
    def install(self):
        print(f'正在解压安装程序...')
        super().install()
Linux=MiniOS('Linux')
print(Linux)
pycharm=PyCharm('PyCharm','1.0')
edge=Edge('Edge','2.0')
Linux.install_app(pycharm)
Linux.install_app(edge)
Linux.install_app(edge)
print(Linux)


