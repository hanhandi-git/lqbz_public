#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <functional>

class Solution {
public:
    int minPathSum(std::vector<std::vector<int>>& grid) {
        int m = grid.size();
        int n = grid[0].size();
        
        // 创建 DP 表格
        std::vector<std::vector<int>> dp(m, std::vector<int>(n, 0));
        
        // 初始化起点
        dp[0][0] = grid[0][0];
        
        // 初始化第一行
        for (int j = 1; j < n; j++) {
            dp[0][j] = dp[0][j-1] + grid[0][j];
        }
        
        // 初始化第一列
        for (int i = 1; i < m; i++) {
            dp[i][0] = dp[i-1][0] + grid[i][0];
        }
        
        // 填充 DP 表格
        for (int i = 1; i < m; i++) {
            for (int j = 1; j < n; j++) {
                dp[i][j] = grid[i][j] + std::min(dp[i-1][j], dp[i][j-1]);
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
            std::vector<std::vector<int>> grid = {{1,3,1},{1,5,1},{4,2,1}};
            return solution.minPathSum(grid) == 7;
        });

        runTest("示例2测试", [this]() {
            std::vector<std::vector<int>> grid = {{1,2,3},{4,5,6}};
            return solution.minPathSum(grid) == 12;
        });

        // 边界情况测试
        runTest("最小网格测试 1x1", [this]() {
            std::vector<std::vector<int>> grid = {{5}};
            return solution.minPathSum(grid) == 5;
        });

        runTest("单行网格测试", [this]() {
            std::vector<std::vector<int>> grid = {{1,2,3,4,5}};
            return solution.minPathSum(grid) == 15;
        });

        runTest("单列网格测试", [this]() {
            std::vector<std::vector<int>> grid = {{1},{2},{3},{4},{5}};
            return solution.minPathSum(grid) == 15;
        });

        // 其他测试用例
        runTest("2x2网格测试", [this]() {
            std::vector<std::vector<int>> grid = {{1,1},{1,1}};
            return solution.minPathSum(grid) == 3;
        });

        runTest("全零网格测试", [this]() {
            std::vector<std::vector<int>> grid = {{0,0,0},{0,0,0},{0,0,0}};
            return solution.minPathSum(grid) == 0;
        });

        runTest("较大值网格测试", [this]() {
            std::vector<std::vector<int>> grid = {{9,9,9},{9,1,9},{9,9,9}};
            return solution.minPathSum(grid) == 37;
        });

        runTest("不同路径选择测试", [this]() {
            std::vector<std::vector<int>> grid = {{1,3,1},{1,9,1},{4,2,1}};
            return solution.minPathSum(grid) == 7;
        });

        runTest("复杂路径测试", [this]() {
            std::vector<std::vector<int>> grid = {{1,4,8,6,2},{6,8,7,5,9},{4,2,3,1,6},{7,9,4,2,5}};
            return solution.minPathSum(grid) == 24;
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