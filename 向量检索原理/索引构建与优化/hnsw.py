import numpy as np
import heapq

class HNSW:
    def __init__(self, max_elements, m=16, ef_construction=200, ef=10):
        """
        HNSW 索引初始化
        :param max_elements: 最大数据量
        :param m: 每层的最大近邻数
        :param ef_construction: 构建阶段的扩展因子
        :param ef: 搜索阶段的扩展因子
        """
        self.max_elements = max_elements
        self.m = m
        self.ef_construction = ef_construction
        self.ef = ef
        self.data = []
        self.graph = []
        self.max_layer = -1
        self.entry_point = None

    def _distance(self, vec1, vec2):
        """计算欧几里得距离"""
        return np.linalg.norm(vec1 - vec2)

    def _select_neighbors(self, candidates, num_neighbors):
        """选择最近的邻居"""
        return heapq.nsmallest(num_neighbors, candidates, key=lambda x: x[1])

    def _search_layer(self, query, entry_point, ef, layer):
        """在指定层中进行搜索"""
        visited = set()
        candidates = []
        heapq.heappush(candidates, (self._distance(query, self.data[entry_point]), entry_point))
        visited.add(entry_point)

        result = []

        while candidates:
            current_distance, current_node = heapq.heappop(candidates)
            result.append((current_distance, current_node))
            for neighbor in self.graph[layer][current_node]:
                if neighbor not in visited:
                    visited.add(neighbor)
                    heapq.heappush(candidates, (self._distance(query, self.data[neighbor]), neighbor))
            if len(result) >= ef:
                break
        return result

    def add(self, vector):
        """添加向量到 HNSW 索引"""
        self.data.append(vector)
        current_index = len(self.data) - 1
        num_layers = np.random.geometric(0.5) - 1
        self.max_layer = max(self.max_layer, num_layers)

        # 确保每层的邻居列表已经初始化
        while len(self.graph) <= num_layers:
            self.graph.append([[] for _ in range(self.max_elements)])  # 使用 max_elements 以确保可以容纳所有元素

        if self.entry_point is None:
            self.entry_point = current_index
            return

        # 从顶层开始搜索插入点
        entry_point = self.entry_point
        for layer in range(self.max_layer, num_layers, -1):
            neighbors = self._search_layer(self.data[current_index], entry_point, 1, layer)
            entry_point = neighbors[0][1]

        # 在目标层建立连接
        for layer in range(min(num_layers, self.max_layer) + 1):
            neighbors = self._search_layer(self.data[current_index], entry_point, self.ef_construction, layer)
            selected_neighbors = [n[1] for n in self._select_neighbors(neighbors, self.m)]
            self.graph[layer][current_index] = selected_neighbors
            for neighbor in selected_neighbors:
                self.graph[layer][neighbor].append(current_index)

    def search(self, query, k):
        """搜索查询向量的最近邻"""
        entry_point = self.entry_point
        for layer in range(self.max_layer, 0, -1):
            neighbors = self._search_layer(query, entry_point, 1, layer)
            entry_point = neighbors[0][1]
        
        # 在底层进行精确搜索
        neighbors = self._search_layer(query, entry_point, self.ef, 0)
        selected_neighbors = self._select_neighbors(neighbors, k)

        # 打印每个最近邻的向量和距离
        for n in selected_neighbors:
            neighbor_index = n[1]
            neighbor_vector = self.data[neighbor_index]
            distance = n[0]
            print(f"Neighbor Index: {neighbor_index}, Vector: {neighbor_vector}, Distance: {distance}")

        return [n[1] for n in selected_neighbors]

if __name__ == "__main__":
    hnsw = HNSW(max_elements=100)
    np.random.seed(42)
    for i in range(10):
        hnsw.add(np.random.rand(5))

    query = np.random.rand(5)
    print("Query:", query)
    result = hnsw.search(query, k=3)
    print("Nearest Neighbors:", result)
# Query: [0.38867729 0.27134903 0.82873751 0.35675333 0.28093451]
# Neighbor Index: 0, Vector: [0.37454012 0.95071431 0.73199394 0.59865848 0.15601864], Distance: 0.738389079969668
# Neighbor Index: 1, Vector: [0.05808361 0.86617615 0.60111501 0.70807258 0.02058449], Distance: 0.8403159067898334
# Neighbor Index: 2, Vector: [0.83244264 0.21233911 0.18182497 0.18340451 0.30424224], Distance: 0.8059148573453739
# Nearest Neighbors: [0, 1, 2]