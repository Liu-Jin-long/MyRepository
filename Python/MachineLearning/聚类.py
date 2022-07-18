#%%
import pandas as pd
from sklearn.decomposition import PCA
from sklearn.cluster import KMeans
import matplotlib.pyplot as plt
from sklearn.metrics import silhouette_score

#%%
prior = pd.read_csv('./data/instacart/order_products__prior.csv')
products = pd.read_csv('./data/instacart/products.csv')
orders = pd.read_csv("./data/instacart/orders.csv")
aisles = pd.read_csv('./data/instacart/aisles.csv')
mer = pd.merge(prior, products, on=['product_id', 'product_id'])
mer = pd.merge(mer, orders, on=['order_id', 'order_id'])
merge_table = pd.merge(mer, aisles, on=['aisle_id', 'aisle_id'])

#%%
print(merge_table.head())
print(merge_table.shape)
#%%
cross_table = pd.crosstab(merge_table['user_id'], merge_table['aisle'])
#%%
print(cross_table.head())
print(cross_table.shape)
#%%
pca = PCA(n_components=0.9)
data = pca.fit_transform(cross_table)
print(type(data))  # <class 'numpy.ndarray'>
print(data.shape)
#%%
# 减少样本数目
x = data[:500]
print(x.shape)
print(x.min())
print(x.max())
#%%
# 假设用户一共分为四个类别
km = KMeans(n_clusters=4)
km.fit(x)
predict = km.predict(x)
print(type(predict))  # <class 'numpy.ndarray'>
print(predict)
print(len(predict))
#%%
# 显示聚类的结果
plt.figure(figsize=(20, 20))
colored = ['orange', 'green', 'blue', 'purple']
col = [colored[i] for i in predict]
plt.scatter(x[:, 2], x[:, 20], color=col)
plt.xlabel('1')
plt.ylabel('20')
plt.show()
print(x)
#%%
# 通过轮廓系数来评判聚类效果，聚类效果很难超过0.7，可以通过这个得分去调整上面KMeans的n_clusters
print(silhouette_score(x, predict))
#%%
# 超参数改为2
km = KMeans(n_clusters=2)
km.fit(x)
predict = km.predict(x)
print(silhouette_score(x, predict))
#%%
# 超参数改为3
km = KMeans(n_clusters=3)
km.fit(x)
predict = km.predict(x)
print(silhouette_score(x, predict))
