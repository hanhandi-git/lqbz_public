import numpy as np
import matplotlib.pyplot as plt

# 参数设置
C = 0.4
K = 20
w_max = 100

# 时间范围
T = np.linspace(0, 50, 1000)

# 计算cwnd
cwnd = C * (T - K)**3 + w_max

# 绘制曲线
plt.plot(T, cwnd, label="Cubic")
plt.xlabel("Time (T)")
plt.ylabel("Congestion Window (cwnd)")
plt.title("Cubic Congestion Control Algorithm")
plt.axhline(y=w_max, color='r', linestyle='--', label="w_max")
plt.legend()
plt.grid(True)
# plt.show()
# 保存图像
plt.savefig("cubic_curve.png")
