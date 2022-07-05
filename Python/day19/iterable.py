# 编写于2022年06月20日17时53分52秒

from collections.abc import Iterable, Iterator

print('列表：', isinstance([], Iterable))
print('列表：', isinstance([], Iterator))
print('列表：', isinstance(iter([]), Iterator))
print('元组：', isinstance((), Iterable))
print('元组：', isinstance((), Iterator))
print('元组：', isinstance(iter(()), Iterator))
print('集合：', isinstance(set(), Iterable))
print('集合：', isinstance(set(), Iterator))
print('集合：', isinstance(iter(set()), Iterator))
print('字典：', isinstance({}, Iterable))
print('字典：', isinstance({}, Iterator))
print('字典：', isinstance(iter({}), Iterator))

list1 = [1, 2, 3]
list1_iterator = iter(list1)
print(type(list1_iterator))  # <class 'list_iterator'>
print(next(list1_iterator))
print(next(list1_iterator))
print(list1_iterator.__next__())
# print(list1_iterator.__next__())  # StopIteration