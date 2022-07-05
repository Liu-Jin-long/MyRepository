# 另一文件from test import *时，以_、__开头的类不能导入
class _Person():
    def __init__(self):
        self.name = 'liu'
        self._age = 10
        self.__height = 180

    def test1(self):
        print('test1')

    def _test1(self):
        print('_test1')

    def __test2(self):
        print('__test2')

    def __str__(self):
        return 'Person实例化'


if __name__ == '__main__':
    p = _Person()
    print(p)
    print(p.name)
    print(p._age)
    # print(p.__height)
    p.test1()
    p._test1()
    # p.__test1()
