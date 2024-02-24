# 加油！！！
from sklearn.datasets import load_iris, fetch_20newsgroups, load_boston
from sklearn.model_selection import train_test_split, GridSearchCV
from sklearn.neighbors import KNeighborsClassifier
from sklearn.preprocessing import StandardScaler
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.naive_bayes import MultinomialNB
from sklearn.metrics import classification_report
from sklearn.feature_extraction import DictVectorizer
from sklearn.tree import DecisionTreeClassifier, export_graphviz
from sklearn.ensemble import RandomForestClassifier
import pandas as pd
import numpy as np
from sklearn.metrics import roc_auc_score


# # 鸢尾花数据集，查看特征，目标，样本量
# li = load_iris()
# # print(li)
# print("获取特征值")
# print(type(li))  # <class 'sklearn.utils._bunch.Bunch'>
# # print(li.data)
# print('-' * 50)
# print(type(li.data))  # <class 'numpy.ndarray'>
# print('-' * 50)
# print(li.data.shape)
# print('-' * 50)
# print("目标值")
# print(li.target)
# print('-' * 50)
# print(li.DESCR)
# print('-' * 50)
# print(li.feature_names)  # 特征名
# print('-' * 50)
# print(li.target_names)
#
# # 测试集25%  随机种子默认是乱序,random_state=1确保重复运行的随机策略一致
# x_train, x_test, y_train, y_test = train_test_split(li.data, li.target, test_size=0.25, random_state=1)
# print('训练集特征值')
# print(x_train)
# print(x_train.shape)
# print('训练集目标值')
# print(y_train)
# print(y_train.shape)
# print('-' * 50)
# print('测试集特征值')
# print(x_test)
# print(x_test.shape)
# print('测试集目标值')
# print(y_test)
# print(y_test.shape)

# # subset代表下载的数据集类型，默认是train，只有训练集
# news = fetch_20newsgroups(subset='all', data_home='data')
# print(news.DESCR)
# print('-' * 50)
# print(news.data[0])  # 第一个样本特征值
# print('-' * 50)
# print(len(news.data))
# print('-' * 50)
# print(news.target[0])
# print('-' * 50)
# print(news.target)
# print('-' * 50)
# print(min(news.target), max(news.target))

# # 回归数据 波士顿房价
# lb = load_boston()
# print(lb.data[0])  # 第一个样本特征值
# print(lb.data.shape)
# print('-' * 50)
# print('-' * 50)
# print('-' * 50)
# print(lb.target)
# print('-' * 50)
# print(lb.DESCR)
# print('-' * 50)
# print(lb.feature_names)
# print('-' * 50)
# # 回归问题没这个,打印这个会报错
# # print(lb.target_names)

##########################################################################
# 分类估计器
# K近邻算法
def KNeighbors():
    """
    K-近邻预测用户签到位置
    """
    # 读取数据
    data = pd.read_csv("./data/FBlocation/train.csv")
    print(data.head(10))
    # 处理数据
    # 缩小数据范围,查询数据
    data = data.query('x > 1.0 &  x < 1.25 & y > 2.5 & y < 2.75')
    # 处理时间数据
    time_value = pd.to_datetime(data['time'], unit='s')
    print(time_value)
    # 把日期格式转换成字典格式，把年，月，日，时，分，秒转换为字典格式
    time_value = pd.DatetimeIndex(time_value)
    print('-' * 50)
    print(time_value)
    print('-' * 50)
    # 构造一些有影响的特征
    # data['day'] = time_value.day
    # data['hour'] = time_value.hour
    # data['weekday'] = time_value.weekday
    data.insert(data.shape[1], 'day', time_value.day)
    data.insert(data.shape[1], 'hour', time_value.hour)
    data.insert(data.shape[1], 'weekday', time_value.weekday)
    # 删除时间戳特征
    data = data.drop(['time'], axis=1)
    print(data)
    # 把签到数量少于n个目标位置删除
    place_count = data.groupby('place_id').count()
    print(place_count)
    # 重置索引为默认位置索引  选择去的人数大于3的地点，人数较少的地点认为是噪声，不需要推荐该地点
    temp = place_count[place_count.row_id > 3].reset_index()
    data = data[data.place_id.isin(temp.place_id)]
    print(data)
    # 取出数据当中的特征值和目标值
    y = data.place_id
    # 删除目标值，保留特征值
    x = data.drop(['place_id'], axis=1)
    # 删除无用的特征值
    x = x.drop(['row_id'], axis=1)
    x_train, x_test, y_train, y_test = train_test_split(x, y, test_size=0.25, random_state=1)
    # 特征工程（标准化）
    std = StandardScaler()
    # 对测试集和训练集的特征值进行标准化
    # transform返回的是复制的数据
    x_train = std.fit_transform(x_train)
    print(id(x_test))
    print(std.mean_)
    print(std.var_)
    # fit拟合数据集训练出一个模型，transform变换是用这个数据集做预测；训练模型用训练集，所以fit_transform；而测试集只是用来做预测，不用来训练模型。
    x_test = std.transform(x_test)  # transfrom不再进行均值和方差的计算，是在原有的基础上去标准化
    print('-' * 50)
    print(id(x_test))
    print(std.mean_)
    print(std.var_)
    # K近邻算法流程
    # n_neighbors超参数，可自行控制调整
    kn = KNeighborsClassifier(n_neighbors=8)
    kn.fit(x_train, y_train)
    y_predict = kn.predict(x_test)
    print("预测的目标签到位置为：")
    print(y_predict)
    print('预测的准确率：')
    print(kn.score(x_test, y_test))
    # 网格搜索
    param = {'n_neighbors': [3, 5, 10, 12, 15]}
    # 进行网格搜索交叉验证(Cross-Validation)，cv=3是3折交叉验证，每次选择cv-1折训练，1折验证，进行cv次
    gc = GridSearchCV(kn, param_grid=param, cv=3)
    gc.fit(x_train, y_train)  # 会将x_train又划分为训练集和验证集
    print("在测试集上准确率：")
    print(gc.score(x_test, y_test))
    print("在交叉验证当中最好的结果：")
    print(gc.best_score_)
    print("选择最好的模型是：")
    print(gc.best_estimator_)
    print("每个超参数每次交叉验证的结果：")
    print(gc.cv_results_)


# KNeighbors()

# 朴素贝叶斯算法
def NBC():
    news = fetch_20newsgroups(subset='all', data_home='data')
    print(news.target)
    print(news.target_names)
    x_train, x_test, y_train, y_test = train_test_split(news.data, news.target, test_size=0.25, random_state=1)
    # 数据集进行特征抽取
    tfidf = TfidfVectorizer()
    x_train = tfidf.fit_transform(x_train)
    print(len(tfidf.get_feature_names_out()))
    print(tfidf.get_feature_names_out()[70000])
    x_test = tfidf.transform(x_test)  # 特征数目不发生改变
    print(len(tfidf.get_feature_names_out()))
    # 进行朴素贝叶斯算法的预测,alpha是拉普拉斯平滑系数，分子和分母加上一个系数，分母加alpha*特征词数目
    mnb = MultinomialNB(alpha=1.0)
    print(x_train)
    print(x_train.toarray())
    mnb.fit(x_train, y_train)
    y_predict = mnb.predict(x_test)
    print("预测的文章类别为：")
    print(y_predict)
    print("准确率为：")
    print(mnb.score(x_test, y_test))
    print("每个类别的精确率和召回率：")
    # 构建显示主要分类指标的文本报告  support是该类别真实的样本
    print(classification_report(y_test, y_predict, target_names=news.target_names))

    # 针对多分类计算AUC策略
    # 把0-19总计20个分类，变为0和1
    y_test1 = np.where(y_test == 5, 1, 0)
    print(y_test1.sum())
    y_predict1 = np.where(y_predict == 5, 1, 0)
    print(y_test1.sum())
    # roc_auc_score的y_test只能是二分类
    print("AUC指标：")
    print(roc_auc_score(y_test1, y_predict1))


# NBC()

def data_preprocessing():
    data = pd.read_csv("./data/titanic.txt")
    print(data.info)
    # 处理数据 选择特征值和目标值
    x = data[['pclass', 'age', 'sex']]
    y = data['survived']
    print(x.info)
    # 缺失值处理
    x['age'].fillna(x['age'].mean(), inplace=True)
    x_train, x_test, y_train, y_test = train_test_split(x, y, test_size=0.25, random_state=4)
    print(x_train.head())
    print(x_train[x_train['sex'] == 'female'].count())
    print(y_train[y_train == 0].count())
    # 特征工程进行处理 特征->类别->one_hot编码
    dict = DictVectorizer(sparse=False)
    # 对字典进行特征抽取 records代表列名变为键
    # print(x_train.to_dict(orient='records'))
    x_train = dict.fit_transform(x_train.to_dict(orient='records'))
    print(type(x_train))  # <class 'numpy.ndarray'>
    print(dict.get_feature_names_out())
    x_test = dict.transform(x_test.to_dict(orient='records'))
    # print(x_train)
    # print(x_test)
    return x_train, x_test, y_train, y_test

# 决策树
def decision_tree():
    x_train, x_test, y_train, y_test=data_preprocessing()
    # 用决策树进行预测，尝试控制修改max_depth  max_depth=10,min_impurity_decrease=0.01
    dt = DecisionTreeClassifier()
    # 训练
    dt.fit(x_train, y_train)
    print("预测的准确率：")
    print(dt.score(x_test, y_test))
    # 导出决策树的结构  windows下命令行dot -Tpng tree.dot -o tree.png
    export_graphviz(dt, out_file='tree.dot',
                    feature_names=['age', 'pclass=1st', 'pclass=2nd', 'pclass=3rd', 'sex=female', 'sex=male'])


# decision_tree()


def random_forest():
    x_train, x_test, y_train, y_test = data_preprocessing()
    # 随机森林进行预测 超参数调优   n_jobs表示利用核心数
    rf = RandomForestClassifier(n_jobs=-1)
    # n_estimators森林中决策树的数目，也就是分类器的数目
    param = {'n_estimators': [1500, 2000, 5000], 'max_depth': [2, 3, 5, 8, 15, 25]}
    # 网格搜索交叉验证
    gc = GridSearchCV(rf, param_grid=param, cv=3)
    gc.fit(x_train,y_train)
    print("准确率：")
    print(gc.score(x_test,y_test))
    print("查看选择的参数模型：")
    print(gc.best_params_)
    print("选择最好的模型是：")
    print(gc.best_estimator_)
    print("每个超参数每次交叉验证的结果：")
    print(gc.cv_results_)
# random_forest()
