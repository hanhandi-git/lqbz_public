# manhattan_distance.py
def manhattan_distance(vec1, vec2):
    if len(vec1) != len(vec2):
        raise ValueError("Vectors must have the same length.")
    return sum(abs(a - b) for a, b in zip(vec1, vec2))

if __name__ == "__main__":
    vec1 = [1.0, 2.0, 3.0]
    vec2 = [4.0, 5.0, 6.0]
    print("Manhattan Distance:", manhattan_distance(vec1, vec2))
