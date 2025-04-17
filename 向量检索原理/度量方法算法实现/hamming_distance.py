# hamming_distance.py
def hamming_distance(vec1, vec2):
    if len(vec1) != len(vec2):
        raise ValueError("Vectors must have the same length.")
    return sum(a != b for a, b in zip(vec1, vec2))

if __name__ == "__main__":
    vec1 = [1, 0, 1]
    vec2 = [0, 1, 1]
    print("Hamming Distance:", hamming_distance(vec1, vec2))
