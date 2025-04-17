# tfidf_vectorization.py
from sklearn.feature_extraction.text import TfidfVectorizer

def tfidf_vectorize(corpus):
    """
    将文档集合转换为 TF-IDF 特征矩阵
    :param corpus: List of strings, 文档集合
    :return: TF-IDF 特征矩阵
    """
    vectorizer = TfidfVectorizer()
    tfidf_matrix = vectorizer.fit_transform(corpus)
    return tfidf_matrix, vectorizer.get_feature_names_out()

if __name__ == "__main__":
    corpus = ["This is a sample document.", "This document is another example."]
    matrix, features = tfidf_vectorize(corpus)
    print("TF-IDF Matrix:\n", matrix.toarray())
    print("Feature Names:", features)
