# 加油！！！
from sklearn.feature_extraction import DictVectorizer
from sklearn.feature_extraction.text import CountVectorizer, TfidfVectorizer
from sklearn.preprocessing import MinMaxScaler, StandardScaler
from sklearn.feature_selection import VarianceThreshold
from sklearn.decomposition import PCA
import jieba
import numpy as np
from sklearn.impute import SimpleImputer


# fit()求得训练集X的均值，方差，最大值，最小值等，这些训练集固有的属性  获取特征和目标值有价值的信息 并没有对特征和目标值进行任何处理
# transform(): transform的作用是在fit的基础上，进行标准化，降维，归一化等操作（看具体用的是哪个工具，如PCA，StandardScaler等）。作用是通过找中心和缩放等实现标准化
# fit_transform(): 是fit和transform的结合，既包括了训练又包含了转换

# 特征抽取
# 数据中含有字符串
def dictvec():
    """字典数据抽取"""
    # 实例化
    # sparse(稀疏的)改为True(默认),输出的是每个不为零位置的坐标，稀疏矩阵可以节省存储空间；False输出整个矩阵
    # 矩阵中存在大量的0，sparse存储只记录非零位置，节省空间的作用
    dict = DictVectorizer()

    # 调用fit_transform
    data = dict.fit_transform([{'city': '北京', 'temperature': 100},
                               {'city': '上海', 'temperature': 60},
                               {'city': '深圳', 'temperature': 30}])
    print(data)
    print(type(data))  # <class 'scipy.sparse._csr.csr_matrix'>  Compressed Sparse Row matrix
    print('-' * 50)
    print(dict.get_feature_names_out())  # 字典中的一些类别数据，分别进行转换成特征
    print('-' * 50)
    print(dict.inverse_transform(data))  # 将数组或稀疏矩阵转换回特征映射


# dictvec()

# 英文文本转换为数值类型
def countvec1():
    # 实例化CountVectorizer
    # max_df, min_df整数：指每个词的所有文档词频数不小于最小值，出现该词的文档数目小于等于max_df
    # max_df, min_df小数：某个词的出现的次数／所有文档数量
    # 默认会去除单个字母的单词，默认认为这个词对整个样本没有影响
    cv = CountVectorizer(min_df=2)
    # 以空格划分统计词频 忽略单字符单词
    res = cv.fit_transform(["life is  short,i like python life",
                            "life is too long,i dislike python",
                            "life is short"])
    print(cv.get_feature_names_out())
    print(res)
    print(type(res))  # <class 'scipy.sparse._csr.csr_matrix'>  Compressed Sparse Row matrix
    print(res.toarray())
    print(cv.inverse_transform(res))


# countvec1()

# 中文文本数值化，对于汉字不能用空格来分割
def countvec2():
    """
    对文本进行特征值化,单个汉字单个字母不统计，因为单个汉字字母没有意义
    :return:
    """
    cv = CountVectorizer()
    data = cv.fit_transform(["人生苦短，我 喜欢 python python", "人生漫长，不用 python"])
    print(cv.get_feature_names_out())
    print(data)
    print(type(data))
    print(data.toarray())
    print(cv.inverse_transform(data))


# countvec2()


# 通过jieba对中文分词
def word_segmentation():
    con1 = jieba.cut("今天很残酷，明天更残酷，后天很美好，但绝对大部分是死在明天晚上，所以每个人不要放弃今天。")
    con2 = jieba.cut("我们看到的从很远星系来的光是在几百万年之前发出的，这样当我们看到宇宙时，我们是在看它的过去。")
    con3 = jieba.cut("如果只用一种方式了解某样事物，你就不会真正了解它。了解事物真正含义的秘密取决于如何将其与我们所了解的事物相联系。")
    # 转换成列表
    print(type(con1))  # <class 'generator'>
    # print(con1)
    content1 = list(con1)
    content2 = list(con2)
    content3 = list(con3)
    # print(content1)
    # print(content2)
    # print(content3)
    c1 = ' '.join(content1)
    c2 = ' '.join(content2)
    c3 = ' '.join(content3)
    return c1, c2, c3


# word_segmentation()

# 中文特征值化
def ChineseVec():
    c1, c2, c3 = word_segmentation()
    print(c1)
    print(c2)
    print(c3)
    print('-' * 50)
    cv = CountVectorizer()
    data = cv.fit_transform([c1, c2, c3])
    # print(data)
    print(cv.get_feature_names_out())
    print(data.toarray())


# ChineseVec()

# 规范{'l1'，'l2'}，默认='l2'
# 每个输出行都有单位范数，或者：
# 'l2'：向量元素的平方和为 1。当应用 l2 范数时，两个向量之间的余弦相似度是它们的点积。
# 'l1'：向量元素的绝对值之和为 1。参见preprocessing.normalize。

# smooth_idf布尔值，默认 = True
# 通过在文档频率上加一来平滑 idf 权重，就好像看到一个额外的文档包含集合中的每个术语恰好一次。防止零分裂0/0。
def tfidf():
    """中文特征值化,计算tfidf值     IDF=log10(总文档数量/该词出现的文档数量)  常见的IDF平滑公式之一IDF=log10[(总文档数量+1)/(该词出现的文档数量+1)]  +1"""
    # TermFrequency-InverseDocumentFrequency词频-逆文档频率  计算=TF*IDF
    c1, c2, c3 = word_segmentation()
    print(c1)
    print(c2)
    print(c3)
    print('-' * 50)
    tf = TfidfVectorizer()
    data = tf.fit_transform([c1, c2, c3])
    print(tf.get_feature_names_out())
    print(type(data))  # <class 'scipy.sparse._csr.csr_matrix'>
    print(data.toarray())


# tfidf()


#####################################################################
# 特征处理 不同的特征拉到到同一个量纲

# 归一化
def normalized():
    """归一化处理"""
    # 归一化缺点 极值容易受异常点影响 特征值容易受极值的影响
    # feature_range代表特征值范围，一般设置为[0,1],或者[-1,1],默认是[0,1]
    # x1=(x-min)/(max-min)  x2=x1*(mx-mi)+mi  feature_range=(mx, mi)
    mms = MinMaxScaler(feature_range=(-1, 1))
    data = mms.fit_transform([[90, 2, 10, 40], [60, 4, 15, 45], [75, 3, 13, 46]])
    print(data)


# normalized()

# 标准化
# 在已有样本足够多的情况下比较稳定，适合现代嘈杂大数据场景
def standardization():
    """标准化缩放，不是标准正太分布，只均值为0，方差为1的分布"""
    std = StandardScaler()
    data = std.fit_transform([[1., -1., 3.], [2., 4., 2.], [4., 6., -1.]])
    print(data)
    print(std.mean_)  # 均值
    print(std.var_)  # 方差
    print(std.n_samples_seen_)  # 样本数
    # 对上面data数据进行标准化
    std_data = std.fit_transform([[-1.06904497, -1.35873244, 0.98058068],
                                  [-0.26726124, 0.33968311, 0.39223227],
                                  [1.33630621, 1.01904933, -1.37281295]])
    # print(std_data) 无意义
    print(std.mean_)
    print(std.var_)
    print(std.n_samples_seen_)


# standardization()

#############################################################################
# 缺失值处理
def missing_value_handling():
    # numpy的数组中可以使用np.nan/np.NaN来代替缺失值，是其他符号如？需要用replace转换成nan
    # 策略可以是均值 中位数 众数 常数
    impute = SimpleImputer(missing_values=np.nan, strategy='median')
    data = impute.fit_transform([[1, 2], [np.nan, 3], [7, 6], [3, np.nan]])
    print(data)


# missing_value_handling()

##################################################################################
# 降维
# 减少特征数 提高模型训练熟读

def vt():
    """特征选择-移除低方差特征"""
    # 参数threshold默认为0 删除小于等于该值的特征
    var = VarianceThreshold(threshold=0)
    data = var.fit_transform([[0, 2, 0, 3],
                              [0, 1, 4, 3],
                              [0, 1, 1, 3]])
    print(data)
    # 获取剩余的特征的原始列编号
    print(f'The support is {var.get_support(True)}')
    # 参数indices默认False 返回原始特征的布尔掩码
    print(f'The support is {var.get_support()}')


# vt()

# Principal Components Analysis主成分分析技术 主分量分析
def pca():
    """主成分分析进行特征降维"""
    # n_components：整数指定希望PCA降维后的特征维度数目 小数(0-1)指定主成分的方差和所占的最小比例阈值，让PCA类自己去根据样本特征方差来决定降维到的维度数
    # 参数设置为"mle", 此时PCA类会用MLE算法根据特征的方差分布情况自己去选择一定数量的主成分特征来降维  默认n_components=min(样本数，特征数)
    pca = PCA(n_components=0.9)
    data = pca.fit_transform([[2, 8, 4, 5], [6, 3, 0, 8], [5, 4, 9, 1]])
    print(data)


pca()
