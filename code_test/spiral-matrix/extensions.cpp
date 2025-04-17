#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>  
#include <climits>

class Solution {
public:
    std::vector<int> spiralOrder(std::vector<std::vector<int>>& matrix) {
        std::vector<int> result;
        if (matrix.empty()) return result;

        int top = 0, bottom = matrix.size() - 1;
        int left = 0, right = matrix[0].size() - 1;

        while (top <= bottom && left <= right) {
            // 从左到右遍历上界
            for (int i = left; i <= right; i++) {
                result.push_back(matrix[top][i]);
            }
            top++;

            // 从上到下遍历右边界
            for (int i = top; i <= bottom; i++) {
                result.push_back(matrix[i][right]);
            }
            right--;

            // 从右到左遍历下边界
            if (top <= bottom) {
                for (int i = right; i >= left; i--) {
                    result.push_back(matrix[bottom][i]);
                }
                bottom--;
            }

            // 从下到上遍历左边界
            if (left <= right) {
                for (int i = bottom; i >= top; i--) {
                    result.push_back(matrix[i][left]);
                }
                left++;
            }
        }

        return result;
    }

    std::vector<int> spiralOrderCounterClockwise(std::vector<std::vector<int>>& matrix) {
        std::vector<int> result;
        if (matrix.empty()) return result;

        int top = 0, bottom = matrix.size() - 1;
        int left = 0, right = matrix[0].size() - 1;

        while (top <= bottom && left <= right) {
            
            for (int i = top; i <= bottom; i++) {
                result.push_back(matrix[i][left]);
            }
            left++;


            for (int i = left; i <= right; i++) {
                result.push_back(matrix[bottom][i]);
            }
            bottom--;


            if (left <= right) {
                for (int i = bottom; i >= top; i--) {
                    result.push_back(matrix[i][right]);
                }
                right--;
            }


            if (top <= bottom) {
                for (int i = right; i >= left; i--) {
                    result.push_back(matrix[top][i]);
                }
                top++;
            }
        }
        // 打印result
        // for (int num : result) {
        //     std::cout << num << " ";
        // }
        // std::cout << std::endl;
        return result;
    }
};

class TestFramework {
private:
    Solution solution;
    int passed = 0;
    int total = 0;

    void runTest(const std::string& testName, std::function<bool()> test) {
        total++;
        if (test()) {
            std::cout << "PASS: " << testName << std::endl;
            passed++;
        } else {
            std::cout << "FAIL: " << testName << std::endl;
        }
    }

    bool expectEqual(const std::vector<int>& expected, const std::vector<int>& actual) {
        if (expected.size() != actual.size()) return false;
        for (size_t i = 0; i < expected.size(); i++) {
            if (expected[i] != actual[i]) {
                return false;
            }
        }
        return true;
    }

public:
    void runAllTests() {
        runTest("SpiralOrderExample1", [this]() {
            std::vector<std::vector<int>> matrix = {
                {1, 2, 3},
                {4, 5, 6},
                {7, 8, 9}
            };
            std::vector<int> expected = {1, 2, 3, 6, 9, 8, 7, 4, 5};
            return expectEqual(expected, solution.spiralOrder(matrix));
        });

        runTest("SpiralOrderCounterClockwiseExample", [this]() {
            std::vector<std::vector<int>> matrix = {
                {1, 2, 3},
                {4, 5, 6},
                {7, 8, 9}
            };
            std::vector<int> expected = {1, 4, 7, 8, 9, 6, 3, 2, 5};
            return expectEqual(expected, solution.spiralOrderCounterClockwise(matrix));
        });

        std::cout << "\n测试结果: " << passed << " 通过, " 
                  << (total - passed) << " 失败, " 
                  << total << " 总计" << std::endl;
    }
};

int main() {
    TestFramework testFramework;
    testFramework.runAllTests();
    return 0;
}
