# 加油！！！
import pandas as pd
import numpy as np
from matplotlib import pyplot as plt

# pandas统计计算和描述
df_obj = pd.DataFrame(np.random.randn(5, 4), columns=['a', 'b', 'c', 'd'])
df_obj.loc[0, 'b'] = np.nan
print(df_obj)
print(df_obj.sum())
print(df_obj.max())
print(df_obj.min(axis=1, skipna=True))
# describe  产生多个统计数据
print(df_obj.describe())
# DataFrame信息
print(df_obj.info())
# 计算Series最小值或者最大值的索引的位置
print(df_obj.loc[:, 'c'].argmin())
print(df_obj.loc[2, :].argmax())
# 计算DataFrame最小值或者最大值的索引值 返回一个Series
print(df_obj)
print(df_obj.idxmax())
print(df_obj.idxmin(axis=1))

# 数据来源：https://www.kaggle.com/code/damianpanek/sunday-eda/data
# Title, Genre, Description, Director, Actors, Year, Runtime (Minutes), Rating, Votes, Revenue (Millions),Metascore
df_obj = pd.read_csv('./IMDB-Movie-Data.csv')
# # print(df_obj)
print(type(df_obj))  # <class 'pandas.core.frame.DataFrame'>
print(df_obj.describe())
print(df_obj.info())
print(df_obj.head(1))
# 获取平均评分
print(df_obj['Rating'].mean())
# 导演的人数
print(len(set(df_obj['Director'].tolist())))
print(len(df_obj['Director'].unique()))
# 获取演员的人数
# print(df_obj['Actors'])
temp_actors_list = df_obj['Actors'].str.split(',').tolist()  # 二维列表
# print(temp_actors_list)
actor_list = [j for i in temp_actors_list for j in i]
print(actor_list)
print(len(set(actor_list)))
# 直方图表示rating,runtime分布情况
runtime_data = df_obj['Runtime (Minutes)'].values
max_runtime = runtime_data.max()
min_runtime = runtime_data.min()
print(max_runtime - min_runtime)
num_bin = (max_runtime - min_runtime) // 5
print(num_bin)
plt.figure(figsize=(20, 8), dpi=80)
# 第二个bins参数必须为int或sequence或str，bins代表划分为多少个单元
plt.hist(runtime_data, int(num_bin))
# plt.xticks(range(min_runtime, max_runtime + 5, 5))
# plt.grid(alpha=0.5)
# plt.show()
# range的步长不支持0.5,自己做一个列表
x_ = [min_runtime]
i = min_runtime
while i <= max_runtime + 0.5:
    i += 0.5
    x_.append(i)
plt.xticks(x_)
plt.show()

# 统计电影题材信息
temp_genre_list = df_obj['Genre'].str.split(',').tolist()  # 二维列表
genre_list = list(set(j for i in temp_genre_list for j in i))  # 题材种类
print(genre_list)
zeros_df = pd.DataFrame(np.zeros((df_obj.shape[0], len(genre_list)), dtype=np.int8), columns=genre_list)
print(zeros_df)
# 构造全为0的数组，行是原来的样本数，列是题材类型数目，one-hot编码，
for i in range(df_obj.shape[0]):
    zeros_df.loc[i, temp_genre_list[i]] = 1
print(zeros_df.head())
# 每种题材电影的数目
genre_count = zeros_df.sum(axis=0)
print(genre_count)
genre_count_sort = genre_count.sort_values()
print(genre_count_sort)
plt.figure(figsize=(20, 8), dpi=80)
plt.bar(range(len(genre_count.index)), genre_count.values, width=0.5, color='orange')
plt.xticks(range(len(genre_count.index)), genre_count.index)
plt.show()
