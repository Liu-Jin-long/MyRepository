# 加油！！！
import pandas as pd
import numpy as np

# pandas的对齐运算
se1 = pd.Series(range(10, 20), index=range(10))
se2 = pd.Series(range(20, 25), index=range(5))
print(se1)
print(se2)
# Series的对齐运算
se3 = se1 + se2
print(se3)  # 缺失填充数据默认是NaN
print(se3[6])
print(se1.add(se2, fill_value=0))  # 未对齐的数据将和填充值做运算
# DataFrame的对齐运算
df1 = pd.DataFrame(np.ones((2, 2)), columns=['a', 'b'])
df2 = pd.DataFrame(np.ones((3, 3)), columns=['a', 'b', 'c'])
print(df2)
print(df2.dtypes)
print(df1 - df2)
print(df1.sub(df2, fill_value=0))  # 未对齐的数据将和填充值做运算
print('-' * 50)

# pandas的函数应用
df_obj = pd.DataFrame(np.random.randn(5, 4) - 1)
print(df_obj)
print(np.abs(df_obj))
# 使用apply默认作用在列上
print(df_obj.apply(lambda x: x.max()))
# apply作用在行上
print(df_obj.apply(lambda x: x.max(), axis=1))
# 使用applymap应用到每个数据
print(df_obj.applymap(lambda x: '%0.2f' % x))
print('-' * 50)

# 索引排序
# Series
print(np.random.randint(5, size=5))  # size是形状
series_obj = pd.Series(range(10, 15), index=np.random.randint(5, size=5))
print(series_obj.index)
# 返回一个新的排好索引的Series
print(series_obj.sort_index())
print(series_obj)
# 存在该索引会把所有该相同索引的值取出来，不存在该索引会报错
# print(series_obj.loc[3])
# print(series_obj.loc[1:3]) # loc索引值不唯一时直接报错
print(series_obj.iloc[0:3])
# 默认用的位置索引
print(series_obj[0:3])
# DataFrame
df_obj = pd.DataFrame(np.random.randn(5, 5),
                      index=np.random.randint(5, size=5),
                      columns=np.random.randint(5, size=5))
print(df_obj)

# 轴0是行索引排序
print(df_obj.sort_index(axis=0, ascending=False))
# 轴1是列索引排序
print(df_obj.sort_index(axis=1, ascending=False))
print('-' * 50)

# 按值排序
# Series
series_obj = pd.Series(np.random.randint(5, 10, size=5))
print(series_obj)
print(series_obj.sort_values(ascending=True))
# DataFrame
df_obj = pd.DataFrame(np.random.randint(0, 100, size=(6, 4)))
print(df_obj)
print(df_obj.values)
# 按轴0排序，by后是列名
print(df_obj.sort_values(by=2, axis=0, ascending=False))
# 按轴1排序，by后行索引名
print(df_obj.sort_values(by=1, axis=1, ascending=False))
print('-' * 50)

# 处理缺失数据
print(np.random.randn(3))
df_obj = pd.DataFrame([np.random.randn(3), [1., 2., np.nan],
                       [np.nan, 4., np.nan], [1., 2., 3.]])
print(df_obj)
# 返回是否存在缺失值的DataFrame
print(df_obj.isnull())
# 删除缺失数据 包括存在缺失值的行或列
print(df_obj.dropna())
print(df_obj.dropna(axis=1))
# inplace=True原地修改
df_obj.dropna(inplace=True)
print(df_obj)
# 填充缺失数据
# 给0列的空值填为-100，按特征（按列）去填充
df_obj = pd.DataFrame([np.random.randn(3), [1., 2., np.nan],
                       [np.nan, 4., np.nan], [1., 2., 3.]])
print(df_obj.iloc[:, 0].fillna(-100.))
print(df_obj.fillna(0))
print('-' * 50)

# 层级索引（hierarchical indexing）
series_obj = pd.Series(np.random.randn(12), index=[['a', 'a', 'a', 'b', 'b', 'b', 'c', 'c', 'c', 'd', 'd', 'd'],
                                                   [0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2]])
print(type(series_obj.index))  # <class 'pandas.core.indexes.multi.MultiIndex'>
print(series_obj.index)
print(series_obj)
index1 = pd.MultiIndex.from_arrays([['a', 'a', 'a', 'b', 'b', 'b', 'c', 'c', 'c', 'd', 'd', 'd'],
                                    [0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2]], names=['cloth', 'size'])
series_obj = ser_obj = pd.Series(np.random.randn(12), index=index1)
# 外层索引取数据
print(series_obj['c'])
# 内层索引取数据
print(series_obj[:, 2])
# 具体内外层索引取数据
print(series_obj['c', 2])
# 交换内层索引和外层索引
print(series_obj.swaplevel)
# 层级索引按那个索引级别排序
print(series_obj.sort_index(level=0))
# unstack()可以将Series对象重构为DataFrame对象，参数可以指定处理的层索引
# 把索引0(最外层)变为列索引
df_obj = ser_obj.unstack(0)
print(df_obj)
# 把索引1变为列索引
df_obj = ser_obj.unstack(1)
print(df_obj)
# stack()可以将DataFrame对象重构为Series对象，参数可以指定处理的列索引
# 把列索引放入内层,只能放到内层
print(df_obj)
series_obj = df_obj.stack(0)
print(series_obj)
series_obj = df_obj.stack()
print(series_obj)
