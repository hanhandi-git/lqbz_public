#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <functional>

class Solution {
public:
    std::vector<int> spiralOrder(std::vector<std::vector<int>>& matrix) {
        if (matrix.empty()) {
            return {};
        }
        std::vector<int> res;
        int row = matrix.size();
        int col = matrix[0].size();
        int left = 0;
        int right = col - 1;
        int top = 0;
        int botton = row - 1;
        while (left <= right && top <= botton)
        {
            // 向右遍历
            for (int i = left; i <= right; i++)
            {
                res.push_back(matrix[top][i]);
            }
            top++;
            // 向下遍历
            for (int i = top; i <= botton; i++)
            {
                res.push_back(matrix[i][right]);
            }
            right--;
            // 向左遍历 
            if (top <= botton) {
                for (int i = right; i >= left; i--)
                {
                    res.push_back(matrix[botton][i]);
                }
                botton--;
            }
            // 向上遍历
            if (left <= right) {
                for (int i = botton; i >= top; i--) 
                {
                    res.push_back(matrix[i][left]);
                }
                left++;
            }
        }
        return res;
    }
};


// class Solution {
// public:
//     std::vector<int> spiralOrder(std::vector<std::vector<int>>& matrix) {
//         // 处理空矩阵情况
//         if (matrix.empty()) return {};
        
//         std::vector<int> result;
//         int rows = matrix.size(), cols = matrix[0].size();
        
//         // 定义四个边界
//         int left = 0, right = cols - 1;
//         int top = 0, bottom = rows - 1;
        
//         // 当边界合法时继续遍历
//         while (left <= right && top <= bottom) {
//             // 1. 向右遍历当前顶部行
//             for (int j = left; j <= right; j++) {
//                 result.push_back(matrix[top][j]);
//             }
//             top++; // 收缩上边界
            
//             // 2. 向下遍历当前最右列
//             for (int i = top; i <= bottom; i++) {
//                 result.push_back(matrix[i][right]);
//             }
//             right--; // 收缩右边界
            
//             // 3. 向左遍历当前底部行(如果还有行未遍历)
//             if (top <= bottom) {
//                 for (int j = right; j >= left; j--) {
//                     result.push_back(matrix[bottom][j]);
//                 }
//                 bottom--; // 收缩下边界
//             }
            
//             // 4. 向上遍历当前最左列(如果还有列未遍历)
//             if (left <= right) {
//                 for (int i = bottom; i >= top; i--) {
//                     result.push_back(matrix[i][left]);
//                 }
//                 left++; // 收缩左边界
//             }
//         }
        
//         return result;
//     }
// };

class TestFramework {
private:
    Solution solution;
    int passed = 0;
    int total = 0;

    void runTest(const std::string& testName, std::function<bool()> test) {
        total++;
        if (test()) {
            std::cout << "通过: " << testName << std::endl;
            passed++;
        } else {
            std::cout << "失败: " << testName << std::endl;
        }
    }

    // 辅助函数：比较两个向量是否相等
    bool vectorsEqual(const std::vector<int>& a, const std::vector<int>& b) {
        if (a.size() != b.size()) return false;
        for (size_t i = 0; i < a.size(); i++) {
            if (a[i] != b[i]) return false;
        }
        return true;
    }

    // 打印向量内容，用于调试
    void printVector(const std::vector<int>& vec) {
        std::cout << "[";
        for (size_t i = 0; i < vec.size(); i++) {
            std::cout << vec[i];
            if (i < vec.size() - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }

public:
    void runAllTests() {
        // 示例1测试
        runTest("示例1测试", [this]() {
            std::vector<std::vector<int>> matrix = {
                {1, 2, 3},
                {4, 5, 6},
                {7, 8, 9}
            };
            std::vector<int> expected = {1, 2, 3, 6, 9, 8, 7, 4, 5};
            auto result = solution.spiralOrder(matrix);
            if (!vectorsEqual(result, expected)) {
                std::cout << "期望: ";
                printVector(expected);
                std::cout << "实际: ";
                printVector(result);
                return false;
            }
            return true;
        });

        // 示例2测试
        runTest("示例2测试", [this]() {
            std::vector<std::vector<int>> matrix = {
                {1, 2, 3, 4},
                {5, 6, 7, 8},
                {9, 10, 11, 12}
            };
            std::vector<int> expected = {1, 2, 3, 4, 8, 12, 11, 10, 9, 5, 6, 7};
            auto result = solution.spiralOrder(matrix);
            if (!vectorsEqual(result, expected)) {
                std::cout << "期望: ";
                printVector(expected);
                std::cout << "实际: ";
                printVector(result);
                return false;
            }
            return true;
        });

        // 边界情况测试
        runTest("空矩阵测试", [this]() {
            std::vector<std::vector<int>> matrix;
            std::vector<int> expected;
            return vectorsEqual(solution.spiralOrder(matrix), expected);
        });

        // 单行矩阵
        runTest("单行矩阵测试", [this]() {
            std::vector<std::vector<int>> matrix = {{1, 2, 3, 4, 5}};
            std::vector<int> expected = {1, 2, 3, 4, 5};
            return vectorsEqual(solution.spiralOrder(matrix), expected);
        });

        // 单列矩阵
        runTest("单列矩阵测试", [this]() {
            std::vector<std::vector<int>> matrix = {{1}, {2}, {3}, {4}, {5}};
            std::vector<int> expected = {1, 2, 3, 4, 5};
            return vectorsEqual(solution.spiralOrder(matrix), expected);
        });

        // 1x1矩阵
        runTest("1x1矩阵测试", [this]() {
            std::vector<std::vector<int>> matrix = {{6}};
            std::vector<int> expected = {6};
            return vectorsEqual(solution.spiralOrder(matrix), expected);
        });

        // 2x2矩阵
        runTest("2x2矩阵测试", [this]() {
            std::vector<std::vector<int>> matrix = {
                {1, 2},
                {3, 4}
            };
            std::vector<int> expected = {1, 2, 4, 3};
            return vectorsEqual(solution.spiralOrder(matrix), expected);
        });

        // 包含负数的矩阵
        runTest("负数矩阵测试", [this]() {
            std::vector<std::vector<int>> matrix = {
                {-1, -2, -3},
                {-4, -5, -6},
                {-7, -8, -9}
            };
            std::vector<int> expected = {-1, -2, -3, -6, -9, -8, -7, -4, -5};
            return vectorsEqual(solution.spiralOrder(matrix), expected);
        });

        // 非方形矩阵 3x2
        runTest("3x2矩阵测试", [this]() {
            std::vector<std::vector<int>> matrix = {
                {1, 2},
                {3, 4},
                {5, 6}
            };
            std::vector<int> expected = {1, 2, 4, 6, 5, 3};
            return vectorsEqual(solution.spiralOrder(matrix), expected);
        });

        // 非方形矩阵 2x3
        runTest("2x3矩阵测试", [this]() {
            std::vector<std::vector<int>> matrix = {
                {1, 2, 3},
                {4, 5, 6}
            };
            std::vector<int> expected = {1, 2, 3, 6, 5, 4};
            return vectorsEqual(solution.spiralOrder(matrix), expected);
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