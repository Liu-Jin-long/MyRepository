
import numpy as np

# 数组中的轴
# a1 = np.array([[1, 2, 3], [4, 5, 6]])
# print(a1)
# print(np.sum(a1, axis=0))
# print(np.sum(a1, axis=1))
# print(np.sum(a1))
# print('-' * 20)
# a2 = np.arange(27).reshape((3, 3, 3))
# print(a2)
# print('-' * 20)
# print(np.sum(a2, axis=0))
# print('-' * 20)
# # [ 0  1  2]+[ 3  4  5]+[ 6  7  8]
# print(np.sum(a2, axis=1))
# print('-' * 20)
# # 0 + 1 + 2, 3 + 4 + 5, 6 + 7 + 8
# print(np.sum(a2, axis=2))
# print('-' * 20)
# -----------------------------------------------------
# # 数组的索引和切片
# # 一维数组的操作方法
# a3 = np.arange(10)
# print(a3)
# print(a3[2])
# print(a3[:8:2])
# # 多维数组的操作方法
# a4 = np.arange(24).reshape((4, 6))
# print(a4)
# print('-' * 20)
# # 取一行
# print(a4[1])
# print('-' * 20)
# # 取连续的多行
# print(a4[1:])
# print('-' * 20)
# # 取连续的多行 每行所有列元素
# print(a4[1:3, :])
# print('-' * 20)
# # 取不连续的多行
# print(a4[[0, 2, 3]])
# print('-' * 20)
# # 取不连续的多行 每行所有列元素
# print(a4[[0, 2, 3], :])
# print('-' * 20)
# # 取一列
# print(a4[:, 1])
# print('-' * 20)
# # 不连续的多列
# print(a4[:, :4])
# print('-' * 20)
# # 取不连续的多列
# print(a4[:, [0, 2, 3]])
# # 取具体下标的某一元素
# print(a4[2, 3])
# print('-' * 20)
# # 取具体下标的多个元素  横坐标列表，纵坐标列表
# print(a4[[1, 2, 3], [2, 3, 4]])
# --------------------------------------------------
# 数组中的数值修改
a5 = np.arange(24).reshape((4, 6))
# 修改某一行的值
a5[1, :] = 0
print(a5)
# 修改某一列的值
a5[:, 1] = 0
print(a5)
# 修改连续多行
a5[1:3, :] = 1
print(a5)
# 修改连续多列
a5[:, 2:4] = 1
print(a5)
# 修改多行多列
a5[1:3, 2:4] = 0
print(a5)
# 修改多个不相邻的点  横坐标列表，纵坐标列表
a5[[1, 2, 3], [2, 3, 4]] = 666
print(a5[[1, 1], [3, 3]])
print(a5)
# 根据条件修改
a5[a5 < 3] = 3
print(a5)
# 通过逻辑判断  np.logical_and &   np.logical_or |   np.logical_not ~
a5[(a5 > 5) & (a5 < 50)] = 0
print(a5)
a5[(a5 > 4) | (a5 == 0)] = 1
print(a5)
a5[~(a5 > 3)] = 0
print(a5)
# clip(num1,num2)  小于num1的元素置为num1,大于num2的元素置为num2
a5 = a5.clip(2, 3)
print(a5)
print('-' * 20)
# 三目运算 np.where(condition, x, y) 满足条件(condition)输出x，不满足输出y
score = np.array([[80, 88], [82, 81], [75, 81]])
ret = np.where(score > 80, True, False)
print(ret)
print('-' * 50)
#################################################################
# numpy的注意点copy和view
a = np.ones((3, 4))
print(a)
b = a
b[1:, :2] = np.nan
# a和b相互影响
print(a)
print(b)
print('-' * 50)
# 对a的切片b，仍相互影响
b = a[1:, :2]
b[:] = 0
print(a)
print(b)
# 视图的操作，一种切片，会创建新的对象a，但是a的数据完全由b保管，相互影响
a = b[:]
print('-' * 50)
# a和b互不影响
b = a.copy()
print(a)
b[:] = np.nan
print(a)
print(b)
