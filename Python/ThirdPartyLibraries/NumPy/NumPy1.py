
import numpy as np
import random
import time

a = list()
for i in range(100000000):
    a.append(random.random())
t1 = time.time()
sum1 = sum(a)
t2 = time.time()

b = np.array(a)
t3 = time.time()
sum2 = np.sum(b)
t4 = time.time()
print(t2 - t1, t4 - t3)
# -----------------------------------------------------
# 列表和数组的区别
list1 = [1, 2, 3, 4]
array1 = np.array(list1)
print(type(list1))  # <class 'list'>
print(type(array1))  # <class 'numpy.ndarray'>  N-dimensional array
print(list1)  # [1, 2, 3, 4]
print(array1)  # [1 2 3 4]
# 创建数组的多种方式
# array2 = np.array([1, 2, 3])
array2 = np.array((1, 2, 3))
print(array2)
print(type(array2))
array3 = np.array(range(10))
print(array3)
array4 = np.arange(0, 10, 2)
print(array4)
# ------------------------------------------------------------
# 二维数组
list2 = [[1, 2], [3, 4], [5, 6]]
two_array = np.array(list2)
print(two_array)
# 常用属性
# 获取数组的维度
print(two_array.ndim)
# 形状（行，列）
print(two_array.shape)
# 元素个数
print(two_array.size)
# -------------------------------------------
# 调整数组形状
array5 = np.array([[1, 2, 3], [4, 5, 6]])
array5.shape = (3, 2)
print(array5)
array6 = array5.reshape((6,))
array7 = array5.reshape((6,), order='F')
print(array6)
print(array7)
# 默认情况下 ‘C’ 以行为主的顺序展开， ‘F’ （ Fortran 风格）意味着以列的顺序展开
print(array5.flatten())
print(array5.flatten(order='F'))

# ------------------------------------------------------------------
array8 = np.arange(24)
print(array8)
print(array8.shape)
array9 = array8.reshape((4, 6))
print(array9)
print(array9.shape)
# ---------------------------------------------------------
# 数组转成列表
list_arr9 = array9.tolist()
print(list_arr9)
print(type(list_arr9))

# ---------------------------------------------
# NumPy的数据类型
array10 = np.array([1, 2, 3, 4, 5], dtype=np.int16)
# 数组中每个元素的字节单位长度
print(array10.itemsize)
# 数组数据类型
print(array10.dtype)
# 数据类型转换
array11 = array10.astype(np.int64)
print(array11.dtype)
# 随机生成小数
print(round(random.random(), 2))
array12 = np.array([random.random() for i in range(10)])
print(array12)
print(np.round(array12, 2))
# -----------------------------------------------------------
t1 = np.arange(24).reshape((6, 4))
t2 = np.arange(0, -24, -1).reshape((6, 4))
print(t1 + t2)
print(t1 * t2)
print(t1 / t2)
t3 = np.arange(18).reshape((6, 3))
# 不种形状的多维数组不能计算
# print(t1-t3)
# 行数或者列数相同的一维数组和多维数组可以进行计算
print(t3)
t4 = np.arange(3)
t5 = np.arange(6).reshape((6, 1))
# 行形状相同（会与每一行数组的对应位相操作 )
print(t3 - t4)
# 列形状相同（会与每一个相同维度的数组的对应位相操作
print(t3 - t5)

