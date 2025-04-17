// hamming_distance.cpp
#include <vector>
#include <iostream>
#include <stdexcept>

int hamming_distance(const std::vector<int>& vec1, const std::vector<int>& vec2) {
    if (vec1.size() != vec2.size()) throw std::invalid_argument("Vectors must have the same length.");
    int distance = 0;
    for (size_t i = 0; i < vec1.size(); ++i) {
        if (vec1[i] != vec2[i]) ++distance;
    }
    return distance;
}

int main() {
    std::vector<int> vec1 = {1, 0, 1};
    std::vector<int> vec2 = {0, 1, 1};
    std::cout << "Hamming Distance: " << hamming_distance(vec1, vec2) << std::endl;
    return 0;
}
