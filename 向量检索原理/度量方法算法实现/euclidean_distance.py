# euclidean_distance.py
import math

def euclidean_distance(vec1, vec2):
    if len(vec1) != len(vec2):
        raise ValueError("Vectors must have the same length.")
    return math.sqrt(sum((a - b) ** 2 for a, b in zip(vec1, vec2)))

if __name__ == "__main__":
    vec1 = [1.0, 2.0, 3.0]
    vec2 = [4.0, 5.0, 6.0]
    print("Euclidean Distance:", euclidean_distance(vec1, vec2))
