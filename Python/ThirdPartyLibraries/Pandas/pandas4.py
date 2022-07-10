# 加油！！！
import pandas as pd
import numpy as np

# pandas分组与聚合
dict_obj = {'key1': ['a', 'b', 'a', 'b',
                     'a', 'b', 'a', 'a'],
            'key2': ['one', 'one', 'two', 'three',
                     'two', 'two', 'one', 'three'],
            'data1': np.random.randint(0, 3, 8),
            'data2': np.random.randint(0, 3, 8)}
df_obj = pd.DataFrame(dict_obj)
print(df_obj)
# 分组运算过程：split->apply->combine
# 拆分：进行分组的根据
# 应用：每个分组运行的计算规则
# 合并：把每个分组的计算结果合并起来
# 不进行聚合操作拿到的只是groupby的对象
print(type(df_obj.groupby('key1')))  # <class 'pandas.core.groupby.generic.DataFrameGroupBy'>
print(df_obj.groupby('key1'))
# DataFrame的data1列根据key1进行分组
print(type(df_obj['data1'].groupby(df_obj['key1'])))  # <class 'pandas.core.groupby.generic.SeriesGroupBy'>
print(df_obj['data1'].groupby(df_obj['key1']))
# 分组运算  聚合
# 不是数值类型的不会计算
group1 = df_obj.groupby('key1')
print(group1.mean())
print(group1.size())  # 返回每个分组的元素个数
group2 = df_obj['data1'].groupby(df_obj['key1'])
print(group2.mean())
print(group2.size())
print('-' * 50)

# 按自定义key分组
self_def_key = [0, 1, 2, 3, 3, 3, 5, 7]
# 分别对应0 1 2 3 4 5 6 7行，用自定义key值作为分组依据
print(df_obj)
print(df_obj.groupby(self_def_key).size())  # 相当于mysql的count
print(df_obj.groupby(self_def_key).sum())
# 自定义的列索引,用的不多
self_column_key = [1, 2, 3, 3]
print(df_obj.groupby(self_column_key, axis=1).sum())  # 通过自定义索引去让不同列相加
# 按自定义key分组，多层列表
print(df_obj.groupby([df_obj['key1'], df_obj['key2']]).size())
# 按多个列多层分组
group1 = df_obj.groupby(['key1', 'key2'])
print(group1.size())
# 多层分组按key的顺序进行
group2 = df_obj.groupby(['key2', 'key1'])
print(group2.size())
print(group2.mean())
# unstack可以将多层行索引的结果转换成单层行索引的DataFrame
print(group2.mean().index)
print(group2.mean().unstack())
print(group2.mean().unstack().columns)
# GroupBy对象支持迭代操作
# 单层分组
group1 = df_obj.groupby('key1')
for group_name, group_data in group1:
    print(group_name)
    print(group_data)
# 多层分组
group2 = df_obj.groupby(['key2', 'key1'])
for group_name, group_data in group2:
    print(group_name)
    print(group_data)
# GroupBy对象可以转换成列表或字典
print(list(group1))
print(dict(list(group2)))
# 按列分组
print(df_obj)
print(df_obj.dtypes)
# 按数据类型分组
print(df_obj.groupby(df_obj.dtypes, axis=1).size())
print(df_obj.groupby(df_obj.dtypes, axis=1).sum())
print('-' * 50)

# 通过字典分组
df_obj = pd.DataFrame(np.random.randint(1, 10, (5, 5)),
                      columns=['a', 'b', 'c', 'd', 'e'],
                      index=['A', 'B', 'C', 'D', 'E'])
df_obj.loc['B', 'b':'d'] = np.nan
print(df_obj)
mapping_dict = {'a': 'Python', 'b': 'Python', 'c': 'Java', 'd': 'C', 'e': 'Java'}
print(df_obj.groupby(mapping_dict, axis=1).size())
print(df_obj.groupby(mapping_dict, axis=1).count())  # df_obj内部值非NaN的个数
print(df_obj.groupby(mapping_dict, axis=1).sum())  ##求和不记录nan,nan当为0

# 通过函数分组
df_obj = pd.DataFrame(np.random.randint(1, 10, (5, 5)),
                      columns=['a', 'b', 'c', 'd', 'ee'],
                      index=['AA', 'BBBB', 'CC', 'D', 'EE'])
print(df_obj)


def group_key(idx):
    '''
    idx为行索引或列索引
    默认为行索引axis=0,传入行索引，轴为1，传入列索引
    传入所有原索引值，返回显示的新索引值
    '''
    # return idx
    return len(idx)


print(df_obj.groupby(group_key, axis=1).size())
print(df_obj.groupby(group_key).size())
# 上式等价于
print(df_obj.groupby(len).size())
print('-' * 50)

# 通过索引级别分组，可以指定某个级别进行分组
columns = pd.MultiIndex.from_arrays([['Python', 'Java', 'Python', 'Java', 'Python'],
                                     ['A', 'A', 'B', 'C', 'B']],
                                    names=['language', 'alphabetical_index'])
df_obj = pd.DataFrame(np.random.randint(1, 10, (5, 5)), columns=columns)
print(df_obj)
# 根据language进行分组
print(df_obj.groupby(level='language', axis=1).sum())
# alphabetical_index
print(df_obj.groupby(level='alphabetical_index', axis=1).sum())
print('-' * 50)

# 聚合aggregation
dict_obj = {'key1': ['a', 'b', 'a', 'b',
                     'a', 'b', 'a', 'a'],
            'key2': ['one', 'one', 'two', 'three',
                     'two', 'two', 'one', 'three'],
            'data1': np.random.randint(1, 10, 8),
            'data2': np.random.randint(1, 10, 8)}
df_obj = pd.DataFrame(dict_obj)
print(df_obj)
# 内置的聚合函数
print(df_obj.groupby('key1').sum())
print(df_obj.groupby('key1').max())
print(df_obj.groupby('key1').min())
print(df_obj.groupby('key1').mean())
print(df_obj.groupby('key1').describe())


# 自定义聚合函数 传入agg方法中
def peak_range(df_obj):
    '''
    返回数值范围,df参数是groupby后的groupby对象
    '''
    # print(type(df_obj))  # 参数为索引所对应的Series
    return df_obj.max() - df_obj.min()


print(df_obj.loc[:, ['key1', 'data1', 'data2']].groupby('key1').agg(peak_range))
print(df_obj.loc[:, ['key1', 'data1', 'data2']].groupby('key1').agg(lambda df: df.max() - df.min()))

# 同时应用多个聚合函数和内置函数  内置函数要用引号引起来  # 警告是未来版本中对字符串聚合运算操作可能会抛出异常
print(df_obj.groupby('data1').agg(['mean', 'std', 'count', peak_range]))
# 对不同列作用不同的聚合函数
dict_mapping = {'data1': 'mean',
                'data2': 'count'}
print(df_obj.groupby('key1').agg(dict_mapping))
