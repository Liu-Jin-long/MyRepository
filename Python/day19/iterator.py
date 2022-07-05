# 编写于2022年06月20日18时30分28秒
# 可迭代对象通过__iter__方法向我们提供一个迭代器，
# 我们在迭代一个可迭代对象的时候，实际上就是先获取该对象提供的一个迭代器，然后通过这个迭代器来依次获取对象中的每一个数据.
from collections.abc import Iterator, Iterable


class MyList():
    def __init__(self):
        self.comtainer = []

    def add(self, item):
        self.comtainer.append(item)

    # isinstance(a,Iterable)是否可迭代通过判断有没有__iter__方法
    def __iter__(self):
        myiterator = MyIterator(self)
        return myiterator


# 迭代器协议（iterator protocol）是指要实现对象的iter()和next()方法（注意：Python3 要实现 next() 方法）
# 其中，iter()通过调用__iter__方法返回迭代器对象本身，next()方法通过调用__next__返回容器的下一个元素，在没有后续元素时抛出StopIteration异常。
class MyIterator():
    def __init__(self, mylist):
        self.mylist = mylist
        self.current = 0

    def __next__(self):
        if self.current < len(self.mylist.comtainer):
            item = self.mylist.comtainer[self.current]
            self.current += 1
            return item
        else:
            # 越界抛出异常
            raise StopIteration

    # 规定对迭代器调用iter()时，返回类型必须是迭代器
    def __iter__(self):
        return self


if __name__ == '__main__':
    mylist = MyList()
    print(isinstance(mylist, Iterable))  # True
    print(isinstance(mylist, Iterator))  # False
    myiterator = MyIterator(mylist)
    print(isinstance(myiterator, Iterable))  # True
    print(isinstance(myiterator, Iterator))  # True

    mi = mylist.add(1)
    mi = mylist.add(2)
    mi = mylist.add(3)

    mylist_iterator = iter(mylist)
    # print(next(mylist_iterator))
    # print(next(mylist_iterator))
    # print(mylist_iterator.__next__())
    # # print(mylist_iterator.__next__())  # StopIteration
    print('*' * 50)
    for i in mylist:
        print(i)
    print('-' * 50)
    # or item in Iterable循环的本质就是先通过iter()函数获取可迭代对象Iterable的迭代器，
    # 然后对获取到的迭代器不断调用next()方法来获取下一个值并将其赋值给item,当遇到StopIteration的异常后循环结束
    for i in mylist_iterator:
        print(i)
