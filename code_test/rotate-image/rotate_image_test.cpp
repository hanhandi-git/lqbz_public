#include <vector>
#include <iostream>
#include <functional>
#include <algorithm>

class Solution {
public:
    void rotate(std::vector<std::vector<int>>& matrix) {
        int n = matrix.size();
        // 沿着主对角线反转 主对角线就是x=y
        for (int i = 0; i < n; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                std::swap(matrix[i][j], matrix[j][i]);
            }
        }
        // 验证中轴线反转
        for (int i = 0; i < n; i++)
        {
            std::reverse(matrix[i].begin(), matrix[i].end());
        }
    }

    // 顺时针旋转180度
    void rotate180(std::vector<std::vector<int>>& matrix) {
        int n = matrix.size();
        // 上下翻转
        std::reverse(matrix.begin(), matrix.end());
        // 每行左右翻转
        for (int i = 0; i < n; i++) {
            std::reverse(matrix[i].begin(), matrix[i].end());
        }
    }

    // 顺时针旋转270度（逆时针旋转90度）
    void rotate270(std::vector<std::vector<int>>& matrix) {
        int n = matrix.size();
        // 沿副对角线翻转
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n - i; j++) {
                std::swap(matrix[i][j], matrix[n-1-j][n-1-i]);
            }
        }
        // 每行左右翻转
        for (int i = 0; i < n; i++) {
            std::reverse(matrix[i].begin(), matrix[i].end());
        }
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

    bool expectEqual(const std::vector<std::vector<int>>& expected, const std::vector<std::vector<int>>& actual) {
        return expected == actual;
    }

public:
    void runAllTests() {
        runTest("Example1", [this]() {
            std::vector<std::vector<int>> matrix = {
                {1,2,3},
                {4,5,6},
                {7,8,9}
            };
            std::vector<std::vector<int>> expected = {
                {7,4,1},
                {8,5,2},
                {9,6,3}
            };
            solution.rotate(matrix);
            return expectEqual(expected, matrix);
        });

        runTest("Example2", [this]() {
            std::vector<std::vector<int>> matrix = {
                {5,1,9,11},
                {2,4,8,10},
                {13,3,6,7},
                {15,14,12,16}
            };
            std::vector<std::vector<int>> expected = {
                {15,13,2,5},
                {14,3,4,1},
                {12,6,8,9},
                {16,7,10,11}
            };
            solution.rotate(matrix);
            return expectEqual(expected, matrix);
        });

        runTest("SingleElement", [this]() {
            std::vector<std::vector<int>> matrix = {{1}};
            std::vector<std::vector<int>> expected = {{1}};
            solution.rotate(matrix);
            return expectEqual(expected, matrix);
        });

        runTest("FourByFour", [this]() {
            std::vector<std::vector<int>> matrix = {
                {1,2,3,4},
                {5,6,7,8},
                {9,10,11,12},
                {13,14,15,16}
            };
            std::vector<std::vector<int>> expected = {
                {13,9,5,1},
                {14,10,6,2},
                {15,11,7,3},
                {16,12,8,4}
            };
            solution.rotate(matrix);
            return expectEqual(expected, matrix);
        });

        runTest("Rotate180", [this]() {
            std::vector<std::vector<int>> matrix = {
                {1,2,3},
                {4,5,6},
                {7,8,9}
            };
            std::vector<std::vector<int>> expected = {
                {9,8,7},
                {6,5,4},
                {3,2,1}
            };
            solution.rotate180(matrix);
            return expectEqual(expected, matrix);
        });

        runTest("Rotate270", [this]() {
            std::vector<std::vector<int>> matrix = {
                {1,2,3},
                {4,5,6},
                {7,8,9}
            };
            std::vector<std::vector<int>> expected = {
                {3,6,9},
                {2,5,8},
                {1,4,7}
            };
            solution.rotate270(matrix);
            return expectEqual(expected, matrix);
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