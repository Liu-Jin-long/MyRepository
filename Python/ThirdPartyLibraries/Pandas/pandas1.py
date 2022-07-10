# 加油！！！
import packaging.version
import pandas as pd
import numpy as np

# Series 是一种类似于一维数组的对象，由一组数据元素(各种NumPy数据类型)以及一组与之对应的索引(数据标签)组成
# 索引(自动创建)(index)在左，数据(values)在右
# 通过list构建Series
series_obj = pd.Series(range(10, 20))
print(series_obj)
print(type(series_obj))  ## <class 'pandas.core.series.Series'>
print(series_obj.head(3))
print(series_obj.dtype == 'int64')
print('-' * 50)
# 获取数据和索引
print(series_obj.values)
print(type(series_obj.values))  # <class 'numpy.ndarray'>
print(series_obj.index)
print(type(series_obj.index))  # <class 'pandas.core.indexes.range.RangeIndex'>
# 通过索引获取数据
print(series_obj[8])
print(series_obj[1:5])
# 通过dict构建Series
year_data = {2001: 17.8, 2002: 20.1, 2003: 16.5}
series_obj = pd.Series(year_data)
print(series_obj.head())  # head默认显示前5行
print(series_obj.index)
print(series_obj[2002])
# name属性
print(series_obj.name)
print(series_obj.index.name)
series_obj.name = 'temp'
series_obj.index.name = 'year'
print(series_obj)
print('-' * 50)

# DataFrame是一个表格型的数据结构，它含有一组有序的列，每列可以是不同类型的值
# DataFrame既有行索引也有列索引，它可以被看做是由 Series 组成的字典（共用同一个索引），数据是以二维结构存放的。
# DataFrame对象既有行索引,又有列索引;行索引,表明不同行,横向索引叫index,0 轴,axis=0;列索引,表名不同列,纵向索引,叫columns,1轴,axis=1
#
#
# 通过ndarray构建DataFrame
df_obj = pd.DataFrame(np.arange(12).reshape((3, 4)))
print(df_obj)
array = np.random.randn(5, 4)
print(array)
df_obj = pd.DataFrame(array)
print(df_obj.head())
# 通过dict构建DataFrame
dict_data = {'A': 1,
             'B': pd.Timestamp('20220708'),
             'C': pd.Series(1, index=list(range(4)), dtype='float32'),
             'D': np.array([1, 2, 3, 4], dtype='int32'),
             'E': ["Python", "Java", "C++", "C"],
             'F': 'liu'}
df_obj = pd.DataFrame(dict_data)
print(df_obj)
print(df_obj.index)
print(df_obj.columns)
print(df_obj.values)
print(df_obj.dtypes)
print(type(df_obj))  # <class 'pandas.core.frame.DataFrame'>
# df_obj.index[2] = 2  # 不可以单独修改某个索引值 保证数据安全
# df_obj.columns[1] = 'B'
dates = pd.date_range('20220708', periods=6)
temp = pd.DataFrame(np.random.randn(6, 4), index=dates, columns=list('cdef'))
print(temp)
print(temp.index)
print(type(temp.index))  # <class 'pandas.core.indexes.datetimes.DatetimeIndex'>
print('-' * 50)

# 使用索引名来取某一行，或者列
print(df_obj)
print(df_obj.loc[1])
print(type(df_obj.loc[1]))  # <class 'pandas.core.series.Series'>
# print(df_obj[1])  不可以
# print(df_obj.index[1])  取索引值
print(df_obj['B'])
print(type(df_obj['B']))  # <class 'pandas.core.series.Series'>
# 自定义增加列
df_obj['H'] = df_obj['D'] + 1
print(df_obj.head())
# 删除列
del (df_obj['H'])
print(df_obj)
print('-' * 50)

# pandas的索引操作
# 常见的Index种类 Index索引；Int64Index整数索引；MultiIndex层级索引；DatetimeIndex时间戳类型
# Series索引
series_obj = pd.Series(range(10, 15), index=list('abcde'))
print(type(series_obj.index))  # <class 'pandas.core.indexes.base.Index'>
print(series_obj)
# 可以用索引名，可以用索引顺序或来取
print(series_obj[2])
print(series_obj['b'])
# 切片索引
print(series_obj[1:3])  # 索引顺序是左闭右开区间
print(series_obj['b':'d'])  # 索引名是左闭右闭区间
# 不连续索引
print(series_obj[[1, 3, 4]])
print(series_obj[['b', 'd']])
# 布尔索引
bool_index = series_obj > 12
print(bool_index)
print(series_obj[bool_index])
print(series_obj[series_obj > 13])
print('-' * 50)

# DataFrame索引
df_obj = pd.DataFrame(np.random.randn(5, 4), columns=list('abcd'))
print(df_obj)
# 列索引
print(df_obj['a'])
print(type(df_obj['a']))  # <class 'pandas.core.series.Series'>
print(df_obj[['a']])  # 可取多列组成一个DataFrame
print(df_obj[['b', 'd']])
print(type(df_obj[['a']]))  # <class 'pandas.core.frame.DataFrame'>
# loc 标签索引 建议使用loc，效率更高 使用索引名进行索引取值
# Series
print(series_obj)
print(series_obj['b': 'd'])
print(series_obj.loc['b': 'd'])
# DataFrame
df_obj = pd.DataFrame(np.random.randn(5, 4),
                      columns=list('dcba'),
                      index=list('abcde'))
print(df_obj)
print(df_obj['b'])  # 取的是行 不建议使用
# print(df_obj['b','b'])  # 不可以
print(df_obj.loc['b'])  # 取的是列
# 第一个参数索引行，第二个参数是列，loc或者iloc效率高于直接用取下标的方式
print(df_obj.loc['a':'c', 'd':'b'])  # 左闭右闭
print(df_obj.loc[['a', 'c'], 'd':'b'])

# 位置索引 iloc 使用0,1,2...(非索引名)索引来取值  推荐,效率更高
# Series
print(series_obj)
print(series_obj[1:3])
print(series_obj.iloc[1:3])
# DataFrame
print(df_obj)
print(df_obj.iloc[0:2, 0:2])



