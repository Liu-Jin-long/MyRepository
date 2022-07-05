# 编写于2022年06月22日15时45分07秒
import copy

# copy.copy()对于可变类型，会进行浅拷贝
a = [1, 2]
b = [7, 8]
c = copy.copy(a)
print('a', id(a))
print('c', id(c))
d = [a, b]
e = copy.copy(d)  # 只copy最外层，内层仍是引用
f=copy.deepcopy(d)#递归copy所有层次
print('d', id(d))
print('d[0]', id(d[0]))
print('e', id(e))
print('e[0]', id(e[0]))
print('f', id(f))
print('f[0]', id(f[0]))
tuple1 = (1, 2)
tuple2 = copy.copy(tuple1)
tuple3 = copy.deepcopy(tuple1)#对于不可变类型，无论是浅拷贝还是深拷贝都仅仅是copy指向引用
print('tuple1', id(tuple1))
print('tuple2', id(tuple2))
print('tuple3', id(tuple3))

