# 加油！！！
import matplotlib.pyplot as plt
from matplotlib import font_manager

# 一图多线
y1 = [1, 0, 1, 1, 2, 4, 3, 4, 4, 5, 6, 5, 4, 3, 3, 1, 1, 1, 1, 1]
y2 = [1, 0, 3, 1, 2, 2, 3, 4, 3, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1]
x = range(11, 31)
plt.figure(figsize=(20, 8), dpi=80)
plt.plot(x, y1, color='red', label='图例:A')
plt.plot(x, y2, color='blue', label='图例:B')
x_ticks_labels = [f'{i}岁' for i in x]
my_font = font_manager.FontProperties(fname='C:\Windows\Fonts\simsun.ttc', size=24)
plt.xticks(x, x_ticks_labels, fontproperties=my_font, rotation=45)
## 绘制网格及其线样式
plt.grid(alpha=0.4, linestyle='-.')
# 添加图例 添加prop参数是显示中文。其它都用fontproperties
# loc参数：upper left、lower left、center left、upper center
plt.legend(prop=my_font, loc='upper right')
plt.show()
