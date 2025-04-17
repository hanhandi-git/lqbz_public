// euclidean_distance.cpp
#include <cmath>
#include <vector>
#include <iostream>
#include <stdexcept>

double euclidean_distance(const std::vector<double>& vec1, const std::vector<double>& vec2) {
    if (vec1.size() != vec2.size()) throw std::invalid_argument("Vectors must have the same length.");
    double sum = 0.0;
    for (size_t i = 0; i < vec1.size(); ++i) {
        sum += std::pow(vec1[i] - vec2[i], 2);
    }
    return std::sqrt(sum);
}

int main() {
    std::vector<double> vec1 = {1.0, 2.0, 3.0};
    std::vector<double> vec2 = {4.0, 5.0, 6.0};
    std::cout << "Euclidean Distance: " << euclidean_distance(vec1, vec2) << std::endl;
    return 0;
}
