import matplotlib.pyplot as plt

class KDTreeNode:
    def __init__(self, point, left=None, right=None):
        self.point = point  # 存储数据点
        self.left = left    # 左子树
        self.right = right  # 右子树


class KDTree:
    def __init__(self, points):
        self.root = self.build_kd_tree(points, depth=0)

    def build_kd_tree(self, points, depth):
        if not points:
            return None

        # 选择当前维度
        k = len(points[0])  # 数据点的维度
        axis = depth % k

        # 按当前维度排序并选择中位数
        points.sort(key=lambda x: x[axis])
        median_index = len(points) // 2

        # 创建节点并递归构建子树
        return KDTreeNode(
            point=points[median_index],
            left=self.build_kd_tree(points[:median_index], depth + 1),
            right=self.build_kd_tree(points[median_index + 1:], depth + 1)
        )

    def nearest_neighbor(self, point):
        return self._nearest_neighbor(self.root, point)

    def _nearest_neighbor(self, node, point, depth=0, best=None):
        if node is None:
            return best

        # 计算当前节点与目标点的距离
        if best is None or self.distance(point, node.point) < self.distance(point, best):
            best = node.point

        # 选择当前维度
        k = len(point)
        axis = depth % k

        # 确定搜索方向
        next_branch = None
        opposite_branch = None

        if point[axis] < node.point[axis]:
            next_branch = node.left
            opposite_branch = node.right
        else:
            next_branch = node.right
            opposite_branch = node.left

        # 递归搜索下一个分支
        best = self._nearest_neighbor(next_branch, point, depth + 1, best)

        # 检查是否需要搜索对侧分支
        if opposite_branch is not None:
            if abs(point[axis] - node.point[axis]) < self.distance(point, best):
                best = self._nearest_neighbor(opposite_branch, point, depth + 1, best)

        return best

    @staticmethod
    def distance(point1, point2):
        return sum((p1 - p2) ** 2 for p1, p2 in zip(point1, point2)) ** 0.5

    def plot_kd_tree(self, node, depth=0, min_bounds=None, max_bounds=None):
        if node is None:
            return

        k = len(node.point)
        axis = depth % k

        # 计算当前节点的边界
        if min_bounds is None:
            min_bounds = [float('-inf')] * k
        if max_bounds is None:
            max_bounds = [float('inf')] * k

        # 绘制当前节点
        plt.scatter(*node.point, color='red')
        plt.text(node.point[0], node.point[1], str(node.point), fontsize=9, ha='right')

        # 绘制分割线
        if axis == 0:  # x轴分割
            plt.axvline(x=node.point[axis], color='blue', linestyle='--')
            self.plot_kd_tree(node.left, depth + 1, min_bounds, node.point)
            self.plot_kd_tree(node.right, depth + 1, node.point, max_bounds)
        else:  # y轴分割
            plt.axhline(y=node.point[axis], color='blue', linestyle='--')
            self.plot_kd_tree(node.left, depth + 1, min_bounds, node.point)
            self.plot_kd_tree(node.right, depth + 1, node.point, max_bounds)

# 示例用法
if __name__ == "__main__":
    points = [(2, 3, 1), (5, 4, 2), (9, 6, 3), (4, 7, 4), (8, 1, 5), (7, 2, 6)]
    kd_tree = KDTree(points)

    target_point = (9, 2, 0)
    nearest = kd_tree.nearest_neighbor(target_point)
    print(f"最近邻点: {nearest}")

    # 绘制 KDTree 并保存到本地
    plt.figure(figsize=(8, 6))
    kd_tree.plot_kd_tree(kd_tree.root)
    plt.title("KDTree Visualization")
    plt.xlabel("X轴")
    plt.ylabel("Y轴")
    plt.grid()
    plt.savefig("kd_tree_visualization.png")  # 保存图像到本地
    plt.close()  # 关闭图像以释放内存