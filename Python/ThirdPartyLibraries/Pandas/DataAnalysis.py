# 加油！！！
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import csv
import os
import six
# 导入可视化库
import plotly as py
import plotly.graph_objs as go

# py.offline.init_notebook_mode()  # 初始化步骤
# pyplot = py.offline.iplot  # 并画图函数

online_data = pd.read_csv('data.csv', encoding='ISO-8859-1', dtype={'CustomerID': str})
# print(online_data.head())
print(type(online_data))  # <class 'pandas.core.frame.DataFrame'>
print(online_data.shape)
# print(online_data.info())
# print(online_data.describe())
# print(online_data.isnull())
missing_data_rate = online_data.apply(lambda x: sum(x.isnull()) / len(x), axis=0)
print(missing_data_rate)
print(type(missing_data_rate))  # <class 'pandas.core.series.Series'>
# 剔除缺失值  如果存在任何nan值,删除该行或列(默认行)
df_obj = online_data.dropna(how='any').copy()
df_obj['InvoiceDate'] = pd.to_datetime(df_obj['InvoiceDate'], errors='coerce')  # coerce代表无效的日期会被解析成nan
# print(df_obj.info())
print(type(df_obj['InvoiceDate']))  # <class 'pandas.core.series.Series'>
print(type(df_obj['InvoiceDate'].dt))  # <class 'pandas.core.indexes.accessors.DatetimeProperties'>
df_obj['InvoiceDate'] = df_obj['InvoiceDate'].apply(lambda x: x.strftime('%Y-%m-%d'))
print(df_obj['InvoiceDate'].dtype)  # <class 'pandas.core.indexes.accessors.DatetimeProperties'>
df_obj['Price'] = df_obj.apply(lambda x: x[3] * x[5], axis=1)
print(df_obj.head())
print('-' * 50)
# 购买商品数前十的国家
quantity_first_10 = df_obj[df_obj['Quantity'] > 0].groupby('Country')['Quantity'].sum().sort_values(
    ascending=False).head(10)
print(quantity_first_10)

# trace_basic = [go.Bar(x=quantity_first_10.index, y=quantity_first_10.values, marker=dict(color='orange'), opacity=0.5)]
# layout = go.Layout(title='购买商品数前十的国家', xaxis='国家')
# figure_basic = go.Figure(data=trace_basic, layout=layout)  # data与layout组成一一个图象对象
# pyplot(figure_basic)


# 交易额前十的国家
transaction_amount_first_10 = df_obj[df_obj['Quantity'] > 0].groupby('Country')['Price'].sum().sort_values(
    ascending=False).head(10)
print(transaction_amount_first_10)

# 各个月份销量销售情况
df_obj['month'] = pd.to_datetime(df_obj['InvoiceDate']).dt.month
print(pd.to_datetime(df_obj['InvoiceDate']).dt.year)
monthly_sales = df_obj[df_obj['Quantity'] > 0].groupby('month')['Quantity'].sum().sort_values(ascending=False).head(12)
print(monthly_sales)

# 平均客单价
sumPrice = df_obj[df_obj['Quantity'] > 0]['Price'].sum()
print(sumPrice)
total_orders = df_obj[df_obj['Quantity'] > 0].groupby('InvoiceNo').count().shape
print(total_orders)
average_unit_price = sumPrice / total_orders[0]
print(average_unit_price)

# 用户消费行为分析
customer = df_obj[df_obj['Quantity'] > 0].groupby('CustomerID').agg({'InvoiceNo': 'nunique',  # 统计用户不同的消费数量,一次订单就是消费一次
                                                                     'Quantity': np.sum,  # 产品数量
                                                                     'Price': np.sum})  # 消费总金额
print(customer.describe())

