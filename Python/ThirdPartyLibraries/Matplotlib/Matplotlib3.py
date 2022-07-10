# 加油！！！

import matplotlib.pyplot as plt
import numpy as np

# 一画布多个坐标系子图
x = np.arange(1, 100)  # 划分子图
# 画布切分为4个区域
fig, axes = plt.subplots(2, 2)
ax1 = axes[0, 0]
ax2 = axes[0, 1]
ax3 = axes[1, 0]
ax4 = axes[1, 1]
fig = plt.figure(figsize=(20, 10), dpi=80)
# 图一
ax1.plot(x, x)
# 图二
ax2.plot(x, -x)
# 图三
ax3.plot(x, x ** 2)
ax3.grid(color='r', linestyle='--', linewidth=1, alpha=0.3)
# 图四
ax4.plot(x, np.log(x))
plt.show()

# add_subplot 方法----给 figure 新增子图
x = np.arange(1, 100)
fig = plt.figure(figsize=(20, 10), dpi=80)
ax1 = fig.add_subplot(2, 2, 1)
ax1.plot(x, x)
ax2 = fig.add_subplot(2, 2, 2)
ax2.plot(x, x ** 2)
ax4 = fig.add_subplot(2, 2, 4)
ax4.plot(x, np.log(x))
plt.show()
