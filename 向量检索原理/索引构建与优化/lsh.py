import numpy as np
import random

class LSH:
    def __init__(self, num_hashes, num_buckets):
        """
        LSH 索引初始化
        :param num_hashes: 哈希函数数量
        :param num_buckets: 桶的数量
        """
        self.num_hashes = num_hashes
        self.num_buckets = num_buckets
        self.hash_tables = [[] for _ in range(num_buckets)]
        self.random_vectors = [np.random.randn(5) for _ in range(num_hashes)]  # 生成随机投影向量

    def _hash_function(self, vector):
        """计算哈希值"""
        # 使用随机投影计算哈希值
        hash_value = sum((np.dot(vector, rv) > 0) for rv in self.random_vectors)  # 计算投影
        return hash_value % self.num_buckets

    def add(self, vector):
        """添加向量到 LSH 索引"""
        hash_value = self._hash_function(vector)
        self.hash_tables[hash_value].append(vector)

    def search(self, query, k):
        """搜索查询向量的最近邻"""
        hash_value = self._hash_function(query)
        candidates = self.hash_tables[hash_value]

        # 在候选向量中进行精确搜索
        distances = [(np.linalg.norm(query - candidate), candidate) for candidate in candidates]
        distances.sort(key=lambda x: x[0])
        
        return [distances[i][1] for i in range(min(k, len(distances)))]

if __name__ == "__main__":
    lsh = LSH(num_hashes=10, num_buckets=100)
    np.random.seed(42)
    for i in range(10):
        lsh.add(np.random.rand(5))

    query = np.random.rand(5)
    print("Query:", query)
    result = lsh.search(query, k=3)
    print("Nearest Neighbors:", result)
# Query: [0.96958463 0.77513282 0.93949894 0.89482735 0.59789998]
# Nearest Neighbors: [array([0.37454012, 0.95071431, 0.73199394, 0.59865848, 0.15601864]), array([0.18340451, 0.30424224, 0.52475643, 0.43194502, 0.29122914])]