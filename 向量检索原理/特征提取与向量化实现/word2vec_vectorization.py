# word2vec_vectorization.py
from gensim.models import Word2Vec

def train_word2vec(sentences):
    """
    使用 Word2Vec 训练语料，生成词向量
    :param sentences: List of lists, 每个子列表是一个句子的分词结果
    :return: 训练好的 Word2Vec 模型
    """
    model = Word2Vec(sentences, vector_size=100, window=5, min_count=1, workers=4)
    return model

if __name__ == "__main__":
    sentences = [["this", "is", "a", "sample"], ["this", "is", "another", "example"]]
    model = train_word2vec(sentences)
    print("Word Vector for 'sample':", model.wv['sample'])
