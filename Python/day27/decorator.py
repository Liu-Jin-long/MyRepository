# 编写于2022年06月29日14时22分23秒
import time


def set_func1(f):
    def call_func1():
        start = time.time()
        f()
        end = time.time()
        print(f'执行时间为{end - start}')

    return call_func1


@set_func1
def test1():
    print('-----test-----')
    for i in range(100000):
        pass


test1()


# ---------------------------------------------------
# 被装饰的函数有参数
def set_func2(f):
    def call_func2(a, b):  ##要和f的参数个数保持一致
        print(f'{f.__name__} called at {time.ctime()}')
        print(f'a={a},b={b}')
        f(a, b)

    return call_func2


@set_func2
def test2(a, b):
    print(f'a+b={a + b}')


test2(1, 2)
time.sleep(1)
test2(4, 5)


# --------------------------------------------------
# 被装饰的函数有不定长参数
def set_func3(f):
    print('-----开始装饰-----')

    def call_func3(*args, **kwargs):
        print('-----1-----')
        print('-----2-----')
        # f(args, kwargs)  # 不行 相当于传递了两个参数，一个元组，一个字典
        f(*args, **kwargs)

    return call_func3


@set_func3
def test3(num, *args, **kwargs):
    print(f'test3  num={num},args={args},kwargs={kwargs}')


test3(1)
test3(1, 2)
test3(1, 2, 3)
test3(1, 2, 3, a=4)
test3(1, 2, 3, a=4, b='liu')  # num=1,args=(2, 3),kwargs={'a': 4, 'b': 'liu'}


# -------------------------------------------------------------
# 被装饰的函数有返回值
def set_func4(f):
    def call_func4():
        print(f'{f.__name__} called at {time.ctime()}')
        return f()

    return call_func4


@set_func4
def test4():
    return '---这是test4的返回值---'


print(test4())


# -------------------------------------------------------------
# 装饰器带参数   在原有装饰器的基础上，设置外部变量
def decorator_args(para):
    def set_func5(f):
        print(f'para={para}')

        def call_func5():
            print(f'{f.__name__} called at {time.ctime()},para={para}')
            if para == 1:
                print('---权限级别1---')
            elif para == 2:
                print('---权限级别1---')
            return f()

        return call_func5

    return set_func5


@decorator_args(1)  # 自动替换成set_func5=decorator_args(1)
def test5():
    print('-----test5-----')


test5()


# --------------------------------------------------------
# 类装饰器
class MyDecorator():
    def __init__(self, f):
        print('---初始化---')
        print(f'类装饰器中传入{f.__name__} {time.ctime()}')
        self.f = f

    def __call__(self, *args, **kwargs):
        print('do something...')
        return self.f()


@MyDecorator  # 相当于test6=MyDecorator(test6)
def test6():
    print('-----test6-----')


test6()

# ------------------------------------------------------
# wraps的作用
from functools import wraps


def set_func6(f):
    @wraps(f)  # 让函数显示原来自己的名字和注释说明
    def call_func6(*args, **kwargs):
        '''This is call_func6'''
        print('调用call_func6函数')
        return f()

    return call_func6


@set_func6
def example():
    '''This is example'''
    print('调用example函数')


print(f'example.__name__={example.__name__}\n'
      f'example.__doc__={example.__doc__}')
