# 加油！！！
import pandas as pd
import numpy as np

# pandas设置索引
df_obj = pd.DataFrame({'a': range(7), 'b': range(7, 0, -1),
                       'c': ['one', 'one', 'one', 'two', 'two', 'two', 'two'],
                       'd': list("hjklmno")})
print(df_obj)
temp = df_obj.copy()
df_obj.index = list('abcdefg')
print(df_obj)
print('-' * 50)
new_df = temp.reindex()  # 去除原DataFrame行索引，使用默认位置索引，原数据不变
print(new_df)
new_df = temp.reindex(list('abcdefg'))  # 去除原DataFrame行索引，将参数作为索引插入，数据清空为NaN
print(new_df)
# 将DataFrame某些列变为行索引
print(df_obj)
print(df_obj.set_index(['c', 'd']))

# 时间序列
# end和periods只需写一个
print(pd.date_range(start="20220101", end="20220708"))
print(pd.date_range(start="20220101", periods=10, freq='B'))  # 工作日频率
print(pd.date_range(start="20220101", periods=10, freq='M'))  # 月末
print(pd.date_range(start="20220101", periods=10, freq='MS'))  # 月初
print(pd.date_range(start="20220101", periods=10, freq='W'))  # 周末
series_obj = pd.Series(['3/11/2000', '3/12/2000', '3/13/2000'] * 10)
print(series_obj)
# to_datetime把字符串转为时间格式
print(pd.to_datetime(series_obj, infer_datetime_format=True))

file_path = "./starbucks_store_worldwide.csv"
df_obj = pd.read_csv(file_path)
print(df_obj.head())
print(df_obj.info())
group1 = df_obj.groupby(by='Country')
group1.size()
country_count = group1['Brand'].count()
# 各国店铺数量对比
print(country_count['US'])
print(country_count['CN'])
import matplotlib.pyplot as plt

country_cn = df_obj[df_obj['Country'] == 'CN']
cn_province = country_cn.groupby(by="State/Province").count()['Brand'].sort_values(ascending=False)
print(cn_province)
plt.figure(figsize=(20, 10), dpi=80)
plt.bar(cn_province.index, cn_province.values)
plt.show()
group1 = df_obj[["Brand"]].groupby(by=[df_obj["Country"], df_obj["State/Province"]]).count()
print(group1)
group2 = df_obj.groupby(by=[df_obj["Country"], df_obj["State/Province"]]).count()
print(group2)

df_obj = pd.read_csv("./911.csv")
df_obj["timeStamp"] = pd.to_datetime(df_obj["timeStamp"])
temp_list = df_obj['title'].str.split(':').tolist()
category_list = [i[0] for i in temp_list]
# print(temp_list)
# print(np.array(category_list).reshape((df_obj.shape[0], 1)))
df_obj['category'] = pd.DataFrame(np.array(category_list).reshape((df_obj.shape[0], 1)))
# 把时间字符串转为时间类型设置为索引
df_obj.set_index('timeStamp', inplace=True)
print(df_obj.head())
# 绘图
for group_name, group_data in df_obj.groupby(by='category'):
    # 重采样：指的是将时间序列从一个频率转化为另一个频率进行处理的过程，将高频率数据转化为低频率数据为降采样，低频率转化为高频率为升采样
    count_by_month = group_data.resample('M').count()['title']  # 降采样
    x_ = count_by_month.index
    print(x_)
    _y = count_by_month.values
    _x = [i.strftime('%Y%m%d') for i in x_]
    plt.plot(_x, _y, label=group_name)
plt.xticks(range(len(_x)), _x, rotation=45, fontsize=6)
plt.legend(loc='best')
plt.show()
