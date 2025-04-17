#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <unordered_set>

class Solution {
public:
    void dfs(std::vector<std::vector<char>>& grid, int i, int j, int n, int m)
    {
        if (i < 0 || i >= n || j < 0 || j >= m || grid[i][j] == '0') return;
        grid[i][j] = '0';
        dfs(grid, i+1, j, n, m);
        dfs(grid, i-1, j, n, m);
        dfs(grid, i, j+1, n, m);
        dfs(grid, i, j-1, n, m);
    } 

    // 1. 岛屿数量
    int numIslands(std::vector<std::vector<char>>& grid) {
       if (grid.empty() || grid[0].empty()) return 0;
       int n = grid.size();
       int m = grid[0].size();
       int cnt = 0;
       for (int i = 0; i < n; i++)
       {
            for (int j = 0; j < m; j++)
            {
                if (grid[i][j] == '1') {
                    dfs(grid, i, j, n, m);
                    cnt++;
                }
            }
       }
       return cnt;
    }
    
private:
    
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
        // 基本测试
        runTest("基本测试 - 单个岛屿", [this]() {
            std::vector<std::vector<char>> grid = {
                {'1', '1', '1'},
                {'1', '1', '1'},
                {'1', '1', '1'}
            };
            return solution.numIslands(grid) == 1;
        });

        runTest("基本测试 - 多个岛屿", [this]() {
            std::vector<std::vector<char>> grid = {
                {'1', '1', '0', '0', '0'},
                {'1', '1', '0', '0', '0'},
                {'0', '0', '1', '0', '0'},
                {'0', '0', '0', '1', '1'}
            };
            return solution.numIslands(grid) == 3;
        });

        // 边界测试
        runTest("边界测试 - 空网格", [this]() {
            std::vector<std::vector<char>> grid;
            return solution.numIslands(grid) == 0;
        });

        runTest("边界测试 - 全水", [this]() {
            std::vector<std::vector<char>> grid = {
                {'0', '0', '0'},
                {'0', '0', '0'},
                {'0', '0', '0'}
            };
            return solution.numIslands(grid) == 0;
        });

        // 新增测试用例
        runTest("边界测试 - 全陆地", [this]() {
            std::vector<std::vector<char>> grid = {
                {'1', '1', '1'},
                {'1', '1', '1'},
                {'1', '1', '1'}
            };
            return solution.numIslands(grid) == 1;
        });

        runTest("特殊形状 - 对角线岛屿", [this]() {
            std::vector<std::vector<char>> grid = {
                {'1', '0', '0'},
                {'0', '1', '0'},
                {'0', '0', '1'}
            };
            return solution.numIslands(grid) == 3;
        });

        runTest("特殊形状 - 十字形岛屿", [this]() {
            std::vector<std::vector<char>> grid = {
                {'0', '1', '0'},
                {'1', '1', '1'},
                {'0', '1', '0'}
            };
            return solution.numIslands(grid) == 1;
        });

        runTest("特殊形状 - 环形岛屿", [this]() {
            std::vector<std::vector<char>> grid = {
                {'1', '1', '1'},
                {'1', '0', '1'},
                {'1', '1', '1'}
            };
            return solution.numIslands(grid) == 1;
        });

        runTest("边界测试 - 单行网格", [this]() {
            std::vector<std::vector<char>> grid = {
                {'1', '0', '1', '0', '1'}
            };
            return solution.numIslands(grid) == 3;
        });

        runTest("边界测试 - 单列网格", [this]() {
            std::vector<std::vector<char>> grid = {
                {'1'},
                {'0'},
                {'1'},
                {'0'},
                {'1'}
            };
            return solution.numIslands(grid) == 3;
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
