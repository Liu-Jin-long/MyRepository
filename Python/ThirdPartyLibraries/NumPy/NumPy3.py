
import numpy as np

# 数组的添加
a = np.array([[1, 2, 3], [4, 5, 6]])
print(a)
# 当axis无定义为None时，是横向追加，返回总是一维数组！
print(np.append(a, [7, 8, 9]))
# 输入数组的维度必须匹配否则将生成 ValueError
print('沿轴0添加元素:')
print(np.append(a, [[7, 8, 9]], axis=0))
print('沿轴1添加元素:')
print(np.append(a, [[5, 5], [7, 8]], axis=1))
a = np.array([[1, 2], [3, 4], [5, 6]])
print(a)
# 当axis无定义为None时,在插入前原数组会被先展开成一维数组
print(np.insert(a, 3, [11, 12, 13]))
print('沿轴0广播:')
print(np.insert(a, 1, [11], axis=0))
print('沿轴0广播:')
print(np.insert(a, 1, 22, axis=1))

# 数组的删除
a = np.arange(12).reshape((3, 4))
print(a)
print('未传递axis参数, 在删除之前原数组会被展开成一维数组')
print(np.delete(a, 3))
print('删除一行:')
print(np.delete(a, 1, axis=0))
print('删除一列和多列:')
print(np.delete(a, 1, axis=1))
print(np.delete(a, [1, 2], axis=1))
# 数组的去重
a = np.array([5, 2, 6, 2, 7, 5, 6, 8, 2, 9])
print(a)
print('第一个数组的去重值:')
print(np.unique(a))
print('去重数组的索引数组:')
# return_index：如果为True，返回新数组的各个元素在原数组中的位置（下标），并以数组形式存储
u, indices = np.unique(a, return_index=True)
print(indices)
# return_inverse：如果为True，返回原数组元素在新数组中的位置（下标），并以数组形式存储
u, indices = np.unique(a, return_inverse=True)
print(indices)
print('去重元素的重复数量:')
# return_counts：如果为 true，返回去重后数组中的元素在原数组中的出现次数
u, indices = np.unique(a, return_counts=True)
print(indices)

# numpy的计算
score = np.array([[80, 88], [82, 81], [75, 81]])
print(score)
# 获取所有数据最大值
print(np.max(score))
# 获取指定轴上的数据最大值
print(np.max(score, axis=0))
print(np.max(score, axis=1))
# 数据的比较
# 返回参数一数组中每个元素与参数二相比较大的数
ret = np.maximum([-2, -1, 0, 1, 2], 0)
print(ret)
# 返回参数一数组中每个元素与参数二相比较小的数  [0, 0, 0, 0, 0]
ret = np.minimum([-2, -1, 0, 1, 2], 0)
print(ret)
# 逐一对比返回较大的数
ret = np.maximum([-2, -1, 0, 5, 6], [1, -2, 3, 4, 5])
print(ret)
# 平均值
# 获取所有数据的平均值
ret = np.mean(score)
print(ret)
# 获取每一行的平均值
ret = np.mean(score, axis=0)
print(ret)
# 获取每一列的平均值
ret = np.mean(score, axis=1)
print(ret)
# 前缀和
a = np.array([[1, 2, 3], [4, 5, 6]])
# 传入的参数为axis
print(a.cumsum(0))
# [1, 2, 3] - -----> | 1 | 2 | 3 |
# [4, 5, 6] - -----> | 1 + 4 | 2 + 5 | 3 + 6 |
print(a.cumsum(1))
# [1, 2, 3] - -----> | 1 | 2 + 1 | 3 + 2 + 1 |
# [4, 5, 6] - -----> | 4 | 4 + 5 | 4 + 5 + 6 |
# 最大值和最小值在数组中轴的下标索引
ret1 = np.argmin(score, axis=0)
ret2 = np.min(score, axis=0)
print(ret1, ret2)
ret1 = np.argmax(score, axis=1)
ret2 = np.max(score, axis=1)
print(ret1, ret2)
# 标准差
ret = np.std(score)
print(ret)
# 每一列的标准差
ret = np.std(score, axis=0)
print(ret)
# 极值  最大值和最小值的差
ret = np.ptp(score)
print(ret)
ret = np.ptp(score, axis=0)
print(ret)
print('-' * 20)

# 数组的拼接
a = np.array([[1, 2], [3, 4]])
b = np.array([[5, 6], [7, 8]])
# 沿轴0拼接两个数组
ret = np.concatenate((a, b), axis=0)
print(ret)
# 沿轴1拼接两个数组
ret = np.concatenate((a, b), axis=1)
print(ret)
# 根据轴进行整体堆叠
ret = np.stack((a, b), axis=0)  # 增加轴1，原有轴1内容不变，变为轴2
print(ret)
ret = np.stack((a, b), axis=1)  # 注意
print(ret)
# 矩阵垂直拼接  vertical
a = [[0, 1, 2, 3, 4, 5], [6, 7, 8, 9, 10, 11]]
b = [[12, 13, 14, 15, 16, 17], [18, 19, 20, 21, 22, 23]]
ret = np.vstack((a, b))
print(ret)
# 矩阵水平拼接  horizontal
a = [[0, 1, 2, 3, 4, 5], [6, 7, 8, 9, 10, 11]]
b = [[12, 13, 14, 15, 16, 17], [18, 19, 20, 21, 22, 23]]
ret = np.hstack((a, b))
print(ret)
print('-' * 20)

# 数组的分割
a = np.arange(9).reshape((3, 3))
print(a)
# indices_or_sections：是一个整数，就用该数平均切分，如果是一个数组，为沿轴切分的位置（左开右闭）
#  axis默认为0，横向切分
print(np.split(a, 3))
ret1, ret2, ret3 = np.split(a, 3, 1)
print(ret1, ret2, ret3)
# 水平分割数组，通过指定要返回的相同形状的数组数量来拆分原数组
harr = np.floor(10 * np.random.random((2, 6)))
print(harr)
print(np.hsplit(harr, 3))  # 竖着切
# 垂直轴分割数组  将一个数组垂直拆分为多个子数组（逐行）  等价于 'split' with 'axis=0'（默认）
varr = np.arange(16).reshape(4, 4)
print(varr)
print(np.vsplit(varr, 4))  # 横着切
print('-' * 20)

# 数组中nan和inf
# nan = NaN = NAN
# nan和任何值计算都为nan
n = np.nan
print(np.nan == np.nan)  # False
print(np.nan == 0)  # False
# Inf = inf = infty = Infinity = PINF
i = np.inf
print(type(n))  # <class 'float'>
print(type(i))  # <class 'float'>

a = np.arange(24, dtype=float).reshape(4, 6)
a[3, 4] = np.nan
print(a)
# 返回非零值的个数
print(np.count_nonzero(a))
print(a != a)
# 得到数组a中nan的个数
print(np.count_nonzero(a != a))
# 将数组a中的nan替换成0
a[np.isnan(a)] = 0
print(a)
print('-' * 20)
# 处理数组中 nan
a = np.arange(24).reshape(4, 6).astype('float')
a[1, 3:] = np.nan
print(a)
# for i in a:
#     print(i)
#     for j in i:
#         print(j)

# 遍历每一列，然后判断每一列是否有nan
for i in range(a.shape[1]):
    temp_col = a[:, i]
    nan_num = np.count_nonzero(temp_col != temp_col)
    if nan_num:
        # 取出该列不为nan的数据
        temp_col_not_nan = temp_col[temp_col == temp_col]
        # 将该列中存在的nan值替换为该列的平均值
        print('列平均值：', np.average(temp_col_not_nan))
        temp_col[np.isnan(temp_col)] = np.mean(temp_col_not_nan)
print(a)
print('-' * 20)

# 二维数组的转置
a = np.arange(12).reshape(3, 4)
print(a)
# 数组的转置
print(a.T)
# 反转或置换数组的轴
print(np.transpose(a))
print('-' * 20)

a = np.ones((3, 4, 5, 6))
print(a.shape)
# 交换数组的两个轴
b = a.swapaxes(1, 3)
print(b.shape)
b = a.swapaxes(2, 0)
print(b.shape)
# 轴滚动
b = np.rollaxis(a, 2, 0)
print(b.shape)
# 当'start<=axis'时，轴回滚直到它位于这个位置。当'start>axis'时，轴会滚动直到它位于该位置之前
b = np.rollaxis(a, 0, 2)
print(b.shape)
print('-' * 20)
# 将数组的轴移动到新位置，其他轴保持原来的顺序
b = np.moveaxis(a, 2, 0)
print(b.shape)
b = np.moveaxis(a, [1, 2], [-1, -2])
print(b.shape)
