// cosine_similarity.cpp
#include <vector>
#include <cmath>
#include <iostream>

double dot_product(const std::vector<double>& vec1, const std::vector<double>& vec2) {
    double dot = 0.0;
    for (size_t i = 0; i < vec1.size(); ++i) {
        dot += vec1[i] * vec2[i];
    }
    return dot;
}

double magnitude(const std::vector<double>& vec) {
    double sum = 0.0;
    for (double val : vec) {
        sum += val * val;
    }
    return std::sqrt(sum);
}

double cosine_similarity(const std::vector<double>& vec1, const std::vector<double>& vec2) {
    return dot_product(vec1, vec2) / (magnitude(vec1) * magnitude(vec2));
}

int main() {
    std::vector<double> vec1 = {1.0, 2.0, 3.0};
    std::vector<double> vec2 = {4.0, 5.0, 6.0};
    std::cout << "Cosine Similarity: " << cosine_similarity(vec1, vec2) << std::endl;
    return 0;
}
