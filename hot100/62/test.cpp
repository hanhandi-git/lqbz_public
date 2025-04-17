#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <functional>

class Solution {
public:
    int uniquePaths(int m, int n) {
        // 创建 DP 表格
        std::vector<std::vector<int>> dp(m, std::vector<int>(n, 0));
        
        // 初始化边界条件
        for (int i = 0; i < m; i++) {
            dp[i][0] = 1;  // 第一列
        }
        for (int j = 0; j < n; j++) {
            dp[0][j] = 1;  // 第一行
        }
        
        // 填充 DP 表格
        for (int i = 1; i < m; i++) {
            for (int j = 1; j < n; j++) {
                dp[i][j] = dp[i-1][j] + dp[i][j-1];
            }
        }
        
        return dp[m-1][n-1];
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
            std::cout << "通过: " << testName << std::endl;
            passed++;
        } else {
            std::cout << "失败: " << testName << std::endl;
        }
    }

public:
    void runAllTests() {
        // 示例测试
        runTest("示例1测试", [this]() {
            return solution.uniquePaths(3, 7) == 28;
        });

        runTest("示例2测试", [this]() {
            return solution.uniquePaths(3, 2) == 3;
        });

        // 边界情况测试
        runTest("最小网格测试 1x1", [this]() {
            return solution.uniquePaths(1, 1) == 1;
        });

        runTest("单行网格测试 1x10", [this]() {
            return solution.uniquePaths(1, 10) == 1;
        });

        runTest("单列网格测试 10x1", [this]() {
            return solution.uniquePaths(10, 1) == 1;
        });

        // 其他测试用例
        runTest("2x2网格测试", [this]() {
            return solution.uniquePaths(2, 2) == 2;
        });

        runTest("2x3网格测试", [this]() {
            return solution.uniquePaths(2, 3) == 3;
        });

        runTest("3x3网格测试", [this]() {
            return solution.uniquePaths(3, 3) == 6;
        });

        runTest("4x4网格测试", [this]() {
            return solution.uniquePaths(4, 4) == 20;
        });

        runTest("5x5网格测试", [this]() {
            return solution.uniquePaths(5, 5) == 70;
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