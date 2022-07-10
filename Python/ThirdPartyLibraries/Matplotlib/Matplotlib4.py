# 加油！！！

import matplotlib.pyplot as plt
from matplotlib import font_manager
# 绘制散点图
y = [11, 17, 16, 11, 12, 11, 12, 6, 6, 7, 8, 9, 12, 15, 14, 17, 18, 21, 16, 17, 20, 14, 15, 15, 15, 19, 21, 22, 22, 22,
     23]
x = range(1, 32)
plt.figure(figsize=(20, 8), dpi=80)
# 使用scatter绘制散点图
plt.scatter(x, y, label='三月份')
my_font = font_manager.FontProperties(fname='C:\Windows\Fonts\simsun.ttc', size=24)
x_ticks_labels = [f'3月{i}日' for i in x]
plt.xticks(x[::3], x_ticks_labels[::3], fontproperties=my_font)
plt.xlabel('日期', fontproperties=my_font)
plt.ylabel('温度', fontproperties=my_font)
plt.legend(prop=my_font)
plt.show()
################################################################
# 绘制条形图
my_font = font_manager.FontProperties(fname='C:\Windows\Fonts\simsun.ttc', size=16)
a = ['流浪地球', '疯狂的外星人', '飞驰人生', '大黄蜂', '熊出没·原始时代', '新喜剧之王']
b = [38.13, 19.85, 14.89, 11.36, 6.47, 5.93]
plt.figure(figsize=(20, 8), dpi=80)
# 绘制条形图的方法
rects = plt.bar(range(len(a)), b, width=0.3, color='r')
plt.xticks(range(len(a)), a, fontproperties=my_font, rotation=45)
for rect in rects:
    height = rect.get_height()
    # 位置x坐标 y坐标 显示文本 对齐方式
    plt.text(rect.get_x() + rect.get_width() / 2, height + 0.3, str(height), ha='center')
plt.show()