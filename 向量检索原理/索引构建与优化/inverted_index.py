class InvertedIndex:
    def __init__(self):
        """初始化倒排索引结构"""
        self.index = {}  # 存储特征值到文档ID的映射

    def add_document(self, doc_id, features):
        """
        将文档添加到倒排索引中
        :param doc_id: 文档的唯一标识符
        :param features: 文档的特征值列表
        """
        for feature in features:
            if feature not in self.index:
                self.index[feature] = []  # 如果特征值不存在，初始化为空列表
            if doc_id not in self.index[feature]:
                self.index[feature].append(doc_id)  # 避免重复添加同一文档ID

    def query(self, feature):
        """
        查询特征值对应的文档列表
        :param feature: 要查询的特征值
        :return: 包含该特征值的文档列表
        """
        return self.index.get(feature, [])  # 返回特征值对应的文档列表，若不存在则返回空列表

    def remove_document(self, doc_id):
        """
        从倒排索引中移除文档
        :param doc_id: 要移除的文档的唯一标识符
        """
        for feature in list(self.index.keys()):  # 遍历所有特征值
            if doc_id in self.index[feature]:
                self.index[feature].remove(doc_id)  # 移除文档ID

    def get_all_features(self):
        """
        获取所有特征值
        :return: 特征值列表
        """
        return list(self.index.keys())  # 返回所有特征值的列表

# 示例用法
if __name__ == "__main__":
    # 创建倒排索引实例
    inverted_index = InvertedIndex()

    # 添加文档
    inverted_index.add_document(1, ['apple', 'banana', 'orange'])
    inverted_index.add_document(2, ['banana', 'kiwi'])
    inverted_index.add_document(3, ['apple', 'kiwi', 'grape'])

    # 查询特征值
    print("查询 'banana':", inverted_index.query('banana'))  # 输出: [1, 2]
    print("查询 'apple':", inverted_index.query('apple'))    # 输出: [1, 3]
    print("查询 'kiwi':", inverted_index.query('kiwi'))      # 输出: [2, 3]
    print("查询 'grape':", inverted_index.query('grape'))    # 输出: [3]
    print("查询 'mango':", inverted_index.query('mango'))    # 输出: []

    # 移除文档
    inverted_index.remove_document(2)
    print("移除文档 2 后查询 'banana':", inverted_index.query('banana'))  # 输出: [1]
    
    # 获取所有特征值
    print("所有特征值:", inverted_index.get_all_features())  # 输出: ['apple', 'banana', 'orange', 'kiwi', 'grape']