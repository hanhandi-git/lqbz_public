// manhattan_distance.cpp
#include <vector>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <cmath>
double manhattan_distance(const std::vector<double>& vec1, const std::vector<double>& vec2) {
    if (vec1.size() != vec2.size()) throw std::invalid_argument("Vectors must have the same length.");
    double sum = 0.0;
    for (size_t i = 0; i < vec1.size(); ++i) {
        sum += std::abs(vec1[i] - vec2[i]);
    }
    return sum;
}

int main() {
    std::vector<double> vec1 = {1.0, 2.0, 3.0};
    std::vector<double> vec2 = {4.0, 5.0, 6.0};
    std::cout << "Manhattan Distance: " << manhattan_distance(vec1, vec2) << std::endl;
    return 0;
}
