# 编写于2022年06月22日17时35分13秒
class Parent():
    def __init__(self, name, *args, **kwargs):
        print('Parent的init开始...')
        self.name = name
        print('Parent的init结束...')


class Son1(Parent):
    def __init__(self, name, age, *args, **kwargs):
        print('Son1的init开始...')
        self.age = age
        super().__init__(name, *args, **kwargs)
        print('Son1的init结束...')


class Son2(Parent):
    def __init__(self, name, height, *args, **kwargs):
        print('Son2的init开始...')
        self.height = height
        super().__init__(name)
        print('Son2的init结束...')


class Grandson(Son1, Son2):
    def __init__(self, name, age, height):
        print('Grandson的init开始...')
        super().__init__(name, age, height)
        print('Grandson的init结束...')


print(Grandson.mro())
grandson = Grandson('liu', 22, 178.5)
print(grandson.name)
print(grandson.age)
print(grandson.height)
