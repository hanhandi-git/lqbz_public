# cosine_similarity.py
import math

def dot_product(vec1, vec2):
    return sum(a * b for a, b in zip(vec1, vec2))

def magnitude(vec):
    return math.sqrt(sum(x ** 2 for x in vec))

def cosine_similarity(vec1, vec2):
    return dot_product(vec1, vec2) / (magnitude(vec1) * magnitude(vec2))

if __name__ == "__main__":
    vec1 = [1.0, 2.0, 3.0]
    vec2 = [4.0, 5.0, 6.0]
    print("Cosine Similarity:", cosine_similarity(vec1, vec2))

