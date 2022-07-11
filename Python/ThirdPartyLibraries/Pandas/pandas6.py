# 加油！！！
import numpy as np
import pandas as pd

# 数据的分组运算
# 分组后给名称加前缀
dict_obj = {'key1': ['a', 'b', 'a', 'b',
                     'a', 'b', 'a', 'a'],
            'key2': ['one', 'one', 'two', 'three',
                     'two', 'two', 'one', 'three'],
            'data1': np.random.randint(1, 10, 8),
            'data2': np.random.randint(1, 10, 8)}
df_obj = pd.DataFrame(dict_obj)
print(df_obj)
# 按key1分组后，计算data1，data2的统计信息并附加到原始表格中，并添加表头前缀
key1_sum = df_obj.groupby('key1').sum().add_prefix('sum_')
print(key1_sum)
# 另一种：使用transform，分组后计算结果和原本的df_obj保持一致
key2_sum = df_obj.groupby('key2').transform(np.sum).add_prefix('sum_')
print(key2_sum)
print(df_obj.groupby('key1').mean())
print(df_obj.groupby('key1').transform(np.mean))
print('-' * 50)
print(df_obj)


def diff_mean(s):
    '''返回数据与均值的差值，s传入的是某一个分组的数据'''
    print(s)
    print('-' * 20)
    return s - s.mean()


print(df_obj.groupby('key1').transform(diff_mean))
print('-' * 50)

# 数据连接
df_obj1 = pd.DataFrame({'key': ['b', 'b', 'a', 'c', 'a', 'a', 'b'],
                        'data1': np.random.randint(0, 10, 7)})
df_obj2 = pd.DataFrame({'key': ['a', 'b', 'd'],
                        'data2': np.random.randint(0, 10, 3)})
print(df_obj1)
print(df_obj2)
# 默认连接使用相同的列名，默认连接方式是内连接
df_obj = pd.merge(df_obj1, df_obj2)
print(df_obj)
# 左表和右表都拿索引连接
df_obj = pd.merge(df_obj1, df_obj2, left_index=True, right_index=True)
print(df_obj)
# 左表和右表都拿key列来连接 on显式指定"外键"
df_obj = pd.merge(df_obj1, df_obj2, on='key')
print(df_obj)
# 更改列名
df_obj1 = df_obj1.rename(columns={'key': 'key1'})
df_obj2 = df_obj2.rename(columns={'key': 'key2'})
print(df_obj1)
print(df_obj2)
##左表指定key1来连接，右表指定key2来连接
df_obj = pd.merge(df_obj1, df_obj2, left_on='key1', right_on='key2')
print(df_obj)
# 全外连接
df_obj = pd.merge(df_obj1, df_obj2, left_on='key1', right_on='key2', how='outer')
print(df_obj)
# 左连接left join等价于数据库的left outer join
df_obj = pd.merge(df_obj1, df_obj2, left_on='key1', right_on='key2', how='left')
print(df_obj)
# 右连接right join等价于left outer join
df_obj = pd.merge(df_obj1, df_obj2, left_on='key1', right_on='key2', how='right')
print(df_obj)
print('-' * 50)

# 处理重复列名
# suffixes默认追加是_x, _y给数据添加后缀实现别名
df_obj1 = pd.DataFrame({'key': ['b', 'b', 'a', 'c', 'a', 'a', 'b'],
                        'data': np.random.randint(0, 10, 7)})
df_obj2 = pd.DataFrame({'key': ['a', 'b', 'd'],
                        'data': np.random.randint(0, 10, 3)})
print(pd.merge(df_obj1, df_obj2, on='key'))
df_obj = pd.merge(df_obj1, df_obj2, on='key', suffixes=('_left', '_right'))
print(df_obj)
print('-' * 50)
# 按索引连接 left_index=True或right_index=True
df_obj1 = pd.DataFrame({'key': ['b', 'b', 'a', 'c', 'a', 'a', 'b'],
                        'data1': np.random.randint(0, 10, 7)})
df_obj2 = pd.DataFrame({'data2': np.random.randint(0, 10, 3)}, index=['a', 'b', 'd'])
print(df_obj1)
print(df_obj2)
print(pd.merge(df_obj1, df_obj2, left_on='key', right_index=True))  # 默认内连接
print(pd.merge(df_obj2, df_obj1, left_index=True, right_on='key'))
print('-' * 50)

# 数据合并concat 沿轴方向将多个对象合并到一起
# NumPy中的合并
arr1 = np.random.randint(0, 10, (3, 4))
arr2 = np.random.randint(0, 10, (3, 4))
print(arr1, arr2)
print(np.concatenate([arr1, arr2]))
print(np.concatenate([arr1, arr2], axis=1))
# Pandas中的合并
# join指定合并方式，默认为outer
# 注意指定轴方向，Series合并时注意查看行索引有无重复
# Series的index没有重复
series_obj1 = pd.Series(np.random.randint(0, 10, 5), index=range(0, 5))
series_obj2 = pd.Series(np.random.randint(0, 10, 4), index=range(5, 9))
series_obj3 = pd.Series(np.random.randint(0, 10, 3), index=range(9, 12))
print(pd.concat([series_obj1, series_obj2, series_obj3]))
print(pd.concat([series_obj1, series_obj2, series_obj3], axis=1))
# Series的index重复
series_obj1 = pd.Series(np.random.randint(0, 10, 5), index=range(5))
series_obj2 = pd.Series(np.random.randint(0, 10, 4), index=range(4))
series_obj3 = pd.Series(np.random.randint(0, 10, 3), index=range(3))
print(series_obj1)
print(series_obj2)
print(series_obj3)
print(pd.concat([series_obj1, series_obj2, series_obj3]))
print(pd.concat([series_obj1, series_obj2, series_obj3], axis=1))  # 默认外连接
print(pd.concat([series_obj1, series_obj2, series_obj3], axis=1, join='inner'))
print('-' * 50)

# DataFrame的concat合并
df_obj1 = pd.DataFrame(np.random.randint(0, 10, (3, 2)), index=['a', 'b', 'c'],
                       columns=['A', 'B'])
df_obj2 = pd.DataFrame(np.random.randint(0, 10, (2, 2)), index=['a', 'b'],
                       columns=['A', 'B'])
print(df_obj1)
print(df_obj2)
print(pd.concat([df_obj1, df_obj2]))  # 默认外连接
print(pd.concat([df_obj1, df_obj2], axis=1))
print('-' * 50)

# 数据重构
# stack和unstack将行索引和列索引进行相互转换，可构成和展开层级索引，还可将DataFrame和Series互相转换
df_obj = pd.DataFrame(np.random.randint(0, 10, (5, 2)), columns=['data1', 'data2'])
print(df_obj)
# stack内部带的参数level，选择哪一个column变为index
stack_df = df_obj.stack()
print(stack_df)
# 默认操作内层索引
print(stack_df.unstack())
# 通过level指定操作索引的级别
print(stack_df.unstack(level=0))
# stack和unstack对DataFrame操作
index1 = pd.MultiIndex.from_arrays([['a', 'a', 'a', 'b', 'b', 'b', 'c', 'c', 'c', 'd', 'd', 'd'],
                                    [0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2]], names=['cloth', 'size'])
df_obj = pd.DataFrame(np.arange((24)).reshape(12, 2), index=index1, columns=['data1', 'data2'])
print(df_obj)
stack_df = df_obj.unstack(level=0)
print(stack_df)
print(stack_df.columns)
# 把columns上的0层索引放到index的内层索引
print(stack_df.stack(0))
print('-' * 50)

# 处理重复数据
df_obj = pd.DataFrame({'data1': ['a'] * 4 + ['b'] * 4,
                       'data2': np.random.randint(0, 4, 8)})
print(df_obj)
# duplicated返回bool型Series表示每行是否为重复行
print(df_obj.duplicated())
print(df_obj[df_obj.duplicated()])  # 取出不重复行
# 按照某一列去重
print(df_obj.duplicated('data2'))
print('-' * 50)
# 在Pandas中 duplicated认为空值和空值相等的
df_obj = pd.DataFrame({'data1': [np.nan] * 4,
                       'data2': list('1225'),
                       'data3': list('abbc')})
print(df_obj)
# nunique是去重后计算个数
print(df_obj.groupby('data3')['data2'].nunique())
print('-' * 20)
print(df_obj.duplicated('data1'))
print(df_obj.drop_duplicates('data1'))
# 删除重复行
print(df_obj.drop_duplicates())
print(df_obj.drop_duplicates('data2'))
# 根据map传入的函数对每行或每列进行转换
# Series根据map传入的函数对每行或每列进行转换
series_obj = pd.Series(np.random.randint(0, 10, 10))
print(series_obj)
print(series_obj.map(lambda x: x ** 2))
print('-' * 50)

# 数据替换
# replace根据值的内容进行替换
series_obj = pd.Series(np.arange(10), index=range(3, 13))
print(series_obj)
# 单个值替换单个值
print(series_obj.replace(1, -1))
# 多个值替换一个值
print(series_obj.replace(range(5, 8), -1))
print(series_obj.replace([5, 8], -1))
# 多个值替换多个值
print(series_obj.replace([5, 8], [-1, -2]))
print('-' * 50)

df_obj = pd.DataFrame({'A': [0, 1, 2, 3, 4],
                       'B': [5, 6, 7, 8, 9],
                       'C': ['a', 'b', 'c', 'd', 'e']})
print(df_obj)
print(df_obj.replace(to_replace=r'^c', value=np.nan, regex=True))
