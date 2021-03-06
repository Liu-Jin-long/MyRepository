# 加油！！！

import matplotlib.pyplot as plt
import matplotlib

# 绘制直方图
time_data = [131, 98, 125, 131, 124, 139, 131, 117, 128, 108, 135, 138, 131, 102, 107, 114,
             119, 128, 121, 142, 127, 130, 124, 101, 110, 116, 117, 110, 128, 128, 115, 99,
             136, 126, 134, 95, 138, 117, 111, 78, 132, 124, 113, 150, 110, 117, 86, 95, 144,
             105, 126, 130, 126, 130, 126, 116, 123, 106, 112, 138, 123, 86, 101, 99, 136, 123,
             117, 119, 105, 137, 123, 128, 125, 104, 109, 134, 125, 127, 105, 120, 107, 129, 116,
             108, 132, 103, 136, 118, 102, 120, 114, 105, 115, 132, 145, 119, 121, 112, 139, 125,
             138, 109, 132, 134, 156, 106, 117, 127, 144, 139, 139, 119, 140, 83, 110, 102, 123,
             107, 143, 115, 136, 118, 139, 123, 112, 118, 125, 109, 119, 133, 112, 114, 122, 109,
             106, 123, 116, 131, 127, 115, 118, 112, 135, 115, 146, 137, 116, 103, 144, 83, 123,
             111, 110, 111, 100, 154, 136, 100, 118, 119, 133, 134, 106, 129, 126, 110, 111, 109,
             141, 120, 117, 106, 149, 122, 122, 110, 118, 127, 121, 114, 125, 126, 114, 140, 103,
             130, 141, 117, 106, 114, 121, 114, 133, 137, 92, 121, 112, 146, 97, 137, 105, 98,
             117, 112, 81, 97, 139, 113, 134, 106, 144, 110, 137, 137, 111, 104, 117, 100, 111,
             101, 110, 105, 129, 137, 112, 120, 113, 133, 112, 83, 94, 146, 133, 101, 131, 116,
             111, 84, 137, 115, 122, 106, 144, 109, 123, 116, 111, 111, 133, 150]
my_font = matplotlib.font_manager.FontProperties(fname='C:\Windows\Fonts\simsun.ttc', size=20)
plt.figure(figsize=(20, 8), dpi=100)
# 设置组距
distance = 2
# 最大值减去最小值除以组距就得到组数
group_num = int((max(time_data) - min(time_data)) / distance)
##bins是组数，自动统计每一组的数量，来显示直方图的高度
plt.hist(time_data, bins=group_num)
# 修改 x 轴刻度显示
plt.xticks(range(min(time_data), max(time_data) + 1)[::2])
# 添加网格显示
plt.grid(linestyle='--', alpha=0.5)
# 添加x, y轴描述信息
plt.xlabel('电影时长大小', fontproperties=my_font)
plt.ylabel('电影的数据量', fontproperties=my_font)
plt.show()

################################################################################################
# 绘制饼状图
# 各部分标签
label_list = ["第一部分", "第二部分", "第三部分"]
# 各部分大小
size = [55, 35, 10]
# 各部分颜色
color = ['r', 'g', 'b']
# 各部分突出值
explode = [0, 0.05, 0]
"""
绘制饼图
explode：设置各部分突出
label:设置各部分标签
labeldistance:设置标签文本距圆心位置，1.1表示1.1倍半径
autopct：设置圆里面文本
shadow：设置是否有阴影
startangle：起始角度，默认从0开始逆时针转
pctdistance：设置圆内文本距圆心距离返回值
l_text：圆内部文本，
matplotlib.text.Text object
p_text：圆外部文本
"""
matplotlib.rcParams['font.sans-serif'] = ['SimHei']
patches, l_text, p_text = plt.pie(size, explode=explode, colors=color,
                                  labels=label_list, labeldistance=1.1, autopct='%1.1f%%',
                                  shadow=False, startangle=90, pctdistance=0.6)
# 设置横轴和纵轴大小相等，这样饼才是圆的
plt.axis('equal')
plt.legend()
plt.show()
