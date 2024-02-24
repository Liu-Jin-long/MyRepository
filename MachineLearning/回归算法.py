# 加油！！！
from sklearn.datasets import load_boston
from sklearn.linear_model import LinearRegression, SGDRegressor, LogisticRegression, Ridge, Lasso
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import mean_squared_error, classification_report, roc_auc_score
import joblib
import pandas as pd
import numpy as np


def normal_equation():
    """正规方程"""
    data = load_boston()
    # print(data.data)
    # print(data.target)
    # print(data.DESCR)
    # print(data.feature_names)
    x_train, x_test, y_train, y_test = train_test_split(data.data, data.target)
    print(x_train.shape)  # (379, 13)
    print(x_test.shape)  # (127, 13)
    print(y_train.shape)
    print(type(y_test))  # <class 'numpy.ndarray'>
    # 特征值和目标值是都必须进行标准化处理, 实例化两个标准化API
    std_x = StandardScaler()
    x_train = std_x.fit_transform(x_train)
    x_test = std_x.transform(x_test)
    std_y = StandardScaler()
    # 压到一列数据上
    # print(y_train.reshape(-1, 1).shape)
    # print(type(y_train.reshape(-1, 1)))
    y_train = std_y.fit_transform(y_train.reshape(-1, 1))  # 目标值一维需转二维传入
    y_test = std_y.transform(y_test.reshape(-1, 1))

    # 正规方程求解方式预测结果，正规方程进行线性回归
    lr = LinearRegression()
    lr.fit(x_train, y_train)
    # 回归系数k可以看特征与目标之间的相关性
    print('回归系数')
    print(lr.coef_)
    y_predict = lr.predict(x_test)
    # 预测测试集的房子价格，通过inverse得到真正的房子价格
    y_lr_predict = std_y.inverse_transform(y_predict)
    # 保存训练好的模型，保存了w的值和模型结构
    joblib.dump(lr, './test.pkl')
    print("正规方程测试集里面每个房子的预测价格：")
    print(y_lr_predict)
    # 求测试集的损失
    print("正规方程的均方误差：")
    print(mean_squared_error(y_test, y_predict))

    # 加载保存的模型
    model = joblib.load('./test.pkl')
    # 目标值进行了标准化，一定要把预测后的值逆向转换回来
    y_predict = model.predict(x_test)
    print("存的模型预测的结果：")
    print(y_predict)
    print("正规方程的均方误差：")
    print(mean_squared_error(y_test, y_predict))
    print("正规方程inverse后的均方误差：")
    print(mean_squared_error(std_y.inverse_transform(y_test),
                             std_y.inverse_transform(y_predict)))


def stochastic_gradient_descent():
    """随机梯度下降法"""
    data = load_boston()
    x_train, x_test, y_train, y_test = train_test_split(data.data, data.target)
    # 特征值和目标值是都必须进行标准化处理, 实例化两个标准化API
    std_x = StandardScaler()
    x_train = std_x.fit_transform(x_train)
    x_test = std_x.transform(x_test)
    std_y = StandardScaler()
    # 压到一列数据上
    # print(y_train.reshape(-1, 1).shape)
    # print(type(y_train.reshape(-1, 1)))
    y_train = std_y.fit_transform(y_train.reshape(-1, 1))
    y_test = std_y.transform(y_test.reshape(-1, 1))

    # 梯度下降 适合数据量大
    sgd = SGDRegressor(eta0=0.008, penalty='l1', alpha=0.005)
    sgd.fit(x_train, y_train)
    print('梯度下降的回归系数')
    print(sgd.coef_)
    y_sgd_predict = std_y.inverse_transform(sgd.predict(x_test).reshape(-1, 1))
    y_predict = sgd.predict(x_test)
    print("梯度下降测试集里面每个房子的预测价格：")
    print(y_sgd_predict)
    print("梯度下降的均方误差：")
    print(mean_squared_error(y_test, y_predict))
    print("梯度下降的原始房价量纲均方误差：")
    print(mean_squared_error(std_y.inverse_transform(y_test), y_sgd_predict))


def ridge_regression():
    data = load_boston()
    x_train, x_test, y_train, y_test = train_test_split(data.data, data.target)
    # 特征值和目标值是都必须进行标准化处理, 实例化两个标准化API
    std_x = StandardScaler()
    x_train = std_x.fit_transform(x_train)
    x_test = std_x.transform(x_test)
    std_y = StandardScaler()
    y_train = std_y.fit_transform(y_train.reshape(-1, 1))
    y_test = std_y.transform(y_test.reshape(-1, 1))
    # 岭回归去进行房价预测
    r = Ridge(alpha=0.05)
    r.fit(x_train, y_train)
    print("模型中的系数为：")
    print(r.coef_)
    print("模型中的偏置为：", )
    print(r.intercept_)
    # 预测测试集的房价
    print(r.predict(x_test).shape)
    y_r_predict = std_y.inverse_transform(r.predict(x_test))
    y_predict = r.predict(x_test)
    print("岭回归的均方误差：")
    print(mean_squared_error(y_test, y_predict))
    print("岭回归的均方误差：")
    print(mean_squared_error(std_y.inverse_transform(y_test), y_r_predict))


def lasso_regression():
    data = load_boston()
    x_train, x_test, y_train, y_test = train_test_split(data.data, data.target)
    # 特征值和目标值是都必须进行标准化处理, 实例化两个标准化API
    std_x = StandardScaler()
    x_train = std_x.fit_transform(x_train)
    x_test = std_x.transform(x_test)
    std_y = StandardScaler()
    y_train = std_y.fit_transform(y_train.reshape(-1, 1))
    y_test = std_y.transform(y_test.reshape(-1, 1))
    # 岭回归去进行房价预测
    l = Lasso(alpha=0.05)
    l.fit(x_train, y_train)
    print("模型中的系数为：")
    print(l.coef_)
    print("模型中的偏置为：", )
    print(l.intercept_)
    # 预测测试集的房价
    print(l.predict(x_test).shape)
    y_l_predict = std_y.inverse_transform(l.predict(x_test).reshape(-1, 1))
    y_predict = l.predict(x_test)
    print("Lasso回归的均方误差：")
    print(mean_squared_error(y_test, y_predict))
    print("Lasso回归的均方误差：")
    print(mean_squared_error(std_y.inverse_transform(y_test), y_l_predict))


# 线性回归
def linear_regression():
    """线性回归直接预测房子价格"""
    # estimator预测
    # 正规方程
    # normal_equation()
    # 梯度下降
    # stochastic_gradient_descent()
    # 岭回归
    ridge_regression()
    # Lasso回归
    lasso_regression()


# linear_regression()


# 逻辑回归
def logistic_regression():
    """逻辑回归做二分类进行癌症预测"""
    columns = ['Sample code number', 'Clump Thickness', 'Uniformity of Cell Size', 'Uniformity of Cell Shape',
               'Marginal Adhesion', 'Single Epithelial Cell Size', 'Bare Nuclei', 'Bland Chromatin', 'Normal Nucleoli',
               'Mitoses', 'Class']
    data = pd.read_csv(
        "https://archive.ics.uci.edu/ml/machine-learning-databases/breast-cancer-wisconsin/breast-cancer-wisconsin.data",
        names=columns)
    print(data.head())
    print(data.describe())
    # 缺失值进行处理
    data = data.replace(to_replace='?', value=np.nan)
    data = data.dropna()
    print(data.shape)
    x_train, x_test, y_train, y_test = train_test_split(data[columns[1:10]], data[columns[10]], test_size=0.25,
                                                        random_state=1)
    std = StandardScaler()
    x_train = std.fit_transform(x_train)
    x_test = std.transform(x_test)
    # 逻辑回归预测  C是正则化力度  solver是学习率优化算法，学习率会随着epoch的变化而变化
    lr = LogisticRegression(C=0.8, solver='newton-cg')
    lr.fit(x_train, y_train)
    print("逻辑回归的权重参数：")
    print(lr.coef_)
    y_predict = lr.predict(x_test)
    print(y_predict)
    print("准确率：")
    print(lr.score(x_test, y_test))
    print("模型中每个类的样本概率：")
    print(lr.predict_proba(x_test))
    print("召回率：")
    print(classification_report(y_test, y_predict, labels=[2, 4], target_names=["良性", "恶性"]))
    print('AUC指标：')
    print(roc_auc_score(y_test,y_predict))

logistic_regression()
