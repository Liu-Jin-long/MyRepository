
import matplotlib.pyplot as plt
from matplotlib import font_manager
import random


# 分别传入x轴数据和y轴数据, 通过plot画图
plt.plot([1, 0, 9], [4, 5, 6])
plt.show()
###############################################
x = range(1, 8)
y = [17, 17, 18, 15, 11, 11, 13]
# 折线颜色，透明度(0-1)，样式，宽度(粗细)  折点样式
plt.plot(x, y, color='red', alpha=0.5, linestyle='--', linewidth=3, marker = 'o')
plt.show()
##################################################
# 设置的图片的大小和保存
x = range(2, 26, 2)
y = [random.randint(15, 30) for i in x]
# 设置画布对象，figsize中对应的图片的宽和高，单位是英寸；dpi是每英寸有多少像素点 缺省值为 80 ， 1 英寸等于 2.5cm
plt.figure(figsize=(20, 8), dpi=80)
plt.plot(x, y)
# plt.show()
# 注意：plt.show()会释放figure资源，如果在显示图像之后保存图片将只能保存空图片
plt.savefig('./t1.png')
#####################################################
# 绘制x轴和y轴的刻度和刻度标签
x = range(2, 26, 2)
y = [random.randint(15, 30) for i in x]
plt.figure(figsize=(20, 8), dpi=80)
x_ticks_label = [f'{i}:00' for i in x]
# 刻度 刻度标签 旋转角度
plt.xticks(x, x_ticks_label, rotation=45)
y_ticks_label = [f'{i}℃' for i in range(min(y), max(y) + 1)]
plt.yticks(range(min(y), max(y) + 1), y_ticks_label)
plt.plot(x, y)
plt.show()
###############################################
# 设置显示中文
# matplotlib只显示英文，无法显示中文，需要修改matplotlib的默认字体（现在版本直接就可以显示）
# 通过matplotlib下的font_manager可以解决
x = range(0, 120)
y = [random.randint(10, 30) for i in range(120)]
plt.figure(figsize=(20, 8), dpi=80)
plt.plot(x, y)
# 在有中文输出的地方，增加一个属性：fontproperties
# plt.xlabel('分钟', rotation=45, fontproperties='simHei', fontsize=20)
# plt.ylabel('温度',fontproperties='simHei', fontsize=40)
# 另一种方式
my_font = font_manager.FontProperties(fname='C:\Windows\Fonts\simsun.ttc', size=24)
plt.xlabel('分钟', fontproperties=my_font, rotation=45)
plt.ylabel('温度', fontproperties='simHei', fontsize=30)
# 设置标题
plt.title('我是标题', color='red', fontproperties=my_font, fontsize=40)
plt.show()
