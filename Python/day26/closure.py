# 编写于2022年06月28日18时21分55秒

def func1(a, b):
    def expr(x):
        return a * x + b

    return expr


expression = func1(1, 1)  # expression是闭包expr的入口地址
# print(expression(1))
# print(expression(2))
# print(expression(3))


# ------------------------------------------------
x = 1
y = 2


def func2(y=20):
    x = 10

    def clos1():
        # nonlocal声明的变量不是局部变量,也不是全局变量,而是外部嵌套函数内的变量
        # nonlocal声明的变量只对局部起作用，离开封装函数，那么该变量就无效
        # nonlocal只能在封装函数中使用，变量在外部函数先进行声明
        nonlocal x
        nonlocal y
        print('----- x={},y={}'.format(x, y))
        x *= 10
        y *= 10
        print('+++++ x={},y={}'.format(x, y))

    print(f'func2 x={x},y={y}')
    return clos1


test = func2()
test()


# -------------------------------------------------

def func3(function):
    global x
    print(f'x={x}')
    x += 1

    def clos2():
        print('add something to do')
        print('-----权限验证-----')

        function()

    return clos2


@func3  # 这里@会自动做 new_function=func3(new_function)
def new_function():
    print('---原函数---')


# new_function=func3(new_function)
new_function()


# ----------------------------------------------------
def add_first(fun):
    print('---开始装饰权限1的功能---')

    def call_func(*args, **kwargs):
        print("---权限验证1---")
        return fun(*args, **kwargs)

    return call_func


def add_second(fun):
    print('---开始装饰权限2的功能---')

    def call_func(*args, **kwargs):
        print("---权限验证2---")
        return fun(*args, **kwargs)

    return call_func


@add_first  # 后装饰  先执行闭包    猜测是？test=add_first(@add_second def test():)
@add_second  # 先装饰  后执行闭包    猜测是？test=add_first(add_second(test))
def test():
    print('---test---')


test()


# ----------------------------------------------------

# 应用案例
def makeBold(f):
    def wrapped():
        return '<b>' + f() + '</b>'

    return wrapped


def makeItalic(f):
    def wrapped():
        return '<i>' + f() + '</i>'

    return wrapped


@makeBold
@makeItalic
def f():
    return 'Hello World!'


print(f())
