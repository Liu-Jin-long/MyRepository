# 数据来源：https://www.kaggle.com/datasets/datasnaek/youtube  只保留views	likes dislikes comment_total四列
import numpy as np
import matplotlib.pyplot as plt

USvideos_file_path = './USvideos.csv'
UKvideos_file_path = './GBvideos.csv'
# unpack=True会将原文件中的一列转换成数组的一行
us = np.loadtxt(USvideos_file_path, delimiter=',', dtype='int')
# uk = np.loadtxt(UKvideos_file_path, delimiter=',', dtype='int', unpack=True)
us_comments = us[:, -1]
print(us_comments.max(), us_comments.min())
distance = 10000
bin_nums = (us_comments.max() - us_comments.min()) // distance
plt.figure(figsize=(20, 8), dpi=80)
plt.hist(us_comments, bin_nums)
plt.show()

print('-' * 50)
# 评论数目集中在0~5000，在5000到600000的数目很少，对数据进行过滤
us_comments_filter = us_comments[us_comments <= 5000]
print(us_comments_filter.max(), us_comments_filter.min())
distance = 50
bin_nums = (us_comments_filter.max() - us_comments_filter.min()) // distance
plt.figure(figsize=(20, 8), dpi=80)
plt.hist(us_comments_filter, bin_nums)
plt.show()
#######################################################################
# 散点图：英国youtube视频的评论数和喜欢数的关系
uk = np.loadtxt(UKvideos_file_path, delimiter=',', dtype='int')
uk_comments = uk[:, -1]
uk_likes = uk[:, 1]
plt.figure(figsize=(20,8),dpi=80)
plt.scatter(uk_likes,uk_comments)
plt.show()