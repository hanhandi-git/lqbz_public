#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    int maximalSquare(vector<vector<char>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) return 0;
        int m = matrix.size(), n = matrix[0].size();
        std::vector<std::vector<int>> dp(m, std::vector<int>(n, 0));
        int maxSide = 0;
        for (int i = 0; i < m; i++)
        {
            if (matrix[i][0] == '1') {
                dp[i][0] = 1;
                maxSide = 1;
            }
        }
        for (int i = 0; i < n; i++)
        {
            if (matrix[0][i] == '1') {
                dp[0][i] = 1;
                maxSide = 1;
            }
        }
        for (int i = 1; i < m; i++)
        {
            for (int j = 1; j < n; j++)
            {
                if (matrix[i][j] == '1') {
                    dp[i][j] = min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]}) + 1;
                    maxSide = max(maxSide, dp[i][j]);
                }
            }
        }
        return maxSide * maxSide;
    }
};

// 测试框架
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

public:
    void runAllTests() {
        // 基本测试 - readme中的示例
        runTest("基本测试 - readme示例", [this]() {
            vector<vector<char>> matrix = {
                {'1','0','1','0','0'},
                {'1','0','1','1','1'},
                {'1','1','1','1','1'},
                {'1','0','0','1','0'}
            };
            return solution.maximalSquare(matrix) == 4;
        });

        // 边界测试 - 空矩阵
        runTest("边界测试 - 空矩阵", [this]() {
            vector<vector<char>> matrix;
            return solution.maximalSquare(matrix) == 0;
        });

        // 边界测试 - 1x1矩阵(0)
        runTest("边界测试 - 1x1矩阵(0)", [this]() {
            vector<vector<char>> matrix = {{'0'}};
            return solution.maximalSquare(matrix) == 0;
        });

        // 边界测试 - 1x1矩阵(1)
        runTest("边界测试 - 1x1矩阵(1)", [this]() {
            vector<vector<char>> matrix = {{'1'}};
            return solution.maximalSquare(matrix) == 1;
        });

        // 边界测试 - 1xn矩阵
        runTest("边界测试 - 1xn矩阵", [this]() {
            vector<vector<char>> matrix = {{'1','1','1'}};
            return solution.maximalSquare(matrix) == 1;
        });

        // 边界测试 - nx1矩阵
        runTest("边界测试 - nx1矩阵", [this]() {
            vector<vector<char>> matrix = {
                {'1'},
                {'1'},
                {'1'}
            };
            return solution.maximalSquare(matrix) == 1;
        });

        // 特殊情况 - 全0矩阵
        runTest("特殊情况 - 全0矩阵", [this]() {
            vector<vector<char>> matrix = {
                {'0','0','0'},
                {'0','0','0'},
                {'0','0','0'}
            };
            return solution.maximalSquare(matrix) == 0;
        });

        // 特殊情况 - 全1矩阵
        runTest("特殊情况 - 全1矩阵", [this]() {
            vector<vector<char>> matrix = {
                {'1','1','1'},
                {'1','1','1'},
                {'1','1','1'}
            };
            return solution.maximalSquare(matrix) == 9;
        });

        // 特殊情况 - 只有一个1
        runTest("特殊情况 - 只有一个1", [this]() {
            vector<vector<char>> matrix = {
                {'0','0','0'},
                {'0','1','0'},
                {'0','0','0'}
            };
            return solution.maximalSquare(matrix) == 1;
        });

        // 特殊情况 - 相邻的1但不能形成正方形
        runTest("特殊情况 - 相邻的1但不能形成正方形", [this]() {
            vector<vector<char>> matrix = {
                {'1','1','1'},
                {'1','0','1'},
                {'1','1','1'}
            };
            return solution.maximalSquare(matrix) == 1;
        });

        // 特殊情况 - 多个可能的正方形
        runTest("特殊情况 - 多个可能的正方形", [this]() {
            vector<vector<char>> matrix = {
                {'1','1','1','1'},
                {'1','1','1','1'},
                {'1','1','1','1'}
            };
            return solution.maximalSquare(matrix) == 9;
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