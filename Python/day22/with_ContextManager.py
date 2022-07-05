# 编写于2022年06月23日21时23分02秒
def func():
    with open('file', 'r', encoding='utf8') as f:
        f.write('liu')


class File():
    def __init__(self, filename, mode):
        self.filename = filename
        self.mode = mode

    def __enter__(self):
        print('进入with')
        self.f = open(self.filename, self.mode, encoding='utf8')
        return self.f

    def __exit__(self, exc_type, exc_val, exc_tb):
        print('退出with')
        self.f.close()


from contextlib import contextmanager
# yield之前的语句在__enter__方法中执行，yield之后的语句在__exit__方法中执行。紧跟在yield后面的值是函数的返回值
@contextmanager
def my_file(path, mode):
    f = open(path, mode, encoding='utf8')
    yield f
    f.close()


if __name__ == '__main__':
    # func()
    with File('file', 'w') as f:
        f.write('TGU')
    with my_file('file', 'w') as f:
        f.write('the simplest context manager')
