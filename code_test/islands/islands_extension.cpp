#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <string>
#include <unordered_set>

class Solution {
public:
    // 1. 岛屿的最大面积
    int maxAreaOfIsland(std::vector<std::vector<int>>& grid) {
        if (grid.empty()) return 0;
        int maxArea = 0;
        int n = grid.size();
        int m = grid[0].size();
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                if (grid[i][j] == 1)
                {
                    maxArea = std::max(maxArea, dfs(grid, i, j));
                }
            }
        }
        return maxArea;
    }
    
    // 2. 岛屿的周长
    int islandPerimeter(std::vector<std::vector<int>>& grid) {
        int perimeter = 0;
        for (int i = 0; i < grid.size(); i++)
        {
            for (int j = 0; j < grid[0].size(); j++)
            {
                if (grid[i][j] == 1)
                {
                    perimeter += 4;
                    if (i-1 >= 0 && grid[i-1][j] == 1) perimeter -= 2;
                    if (j-1 >= 0 && grid[i][j-1] == 1) perimeter -= 2;
                }
            }
        }
        return perimeter;
    }

    // 3. 不同岛屿的数量
    int numDistinctIslands(std::vector<std::vector<int>>& grid) {
        std::unordered_set<std::string> islands;
        int m = grid.size(), n = grid[0].size();
        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1)
                {
                    std::string path;
                    dfs(grid, i, j, i, j, path);
                    islands.insert(path);
                }
            }
        }
        return islands.size();
    }
    
private:
    int dfs(std::vector<std::vector<int>>& grid, int i, int j) {
        if (i < 0 || i >= grid.size() || j < 0 || j >= grid[0].size() || grid[i][j] == 0) return 0;

        grid[i][j] = 0;
        return 1 + dfs(grid, i+1, j) + dfs(grid, i-1, j) + dfs(grid, i, j+1) + dfs(grid, i, j-1);
    }
    
    void dfs(std::vector<std::vector<int>>& grid, int i, int j, int baseI, int baseJ, std::string& path) {
        if (i < 0 || i >= grid.size() || j < 0 || j >= grid[0].size() || grid[i][j] == 0) {
            return;
        }
        grid[i][j] = 0;
        path += std::to_string(i - baseI) + "," + std::to_string(j - baseJ) + "|";
        dfs(grid, i-1, j, baseI, baseJ, path);
        dfs(grid, i+1, j, baseI, baseJ, path);
        dfs(grid, i, j-1, baseI, baseJ, path);
        dfs(grid, i, j+1, baseI, baseJ, path);
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
        // 最大面积测试
        runTest("最大面积 - 基本测试", [this]() {
            std::vector<std::vector<int>> grid = {
                {1,1,0,0,0},
                {1,1,0,0,0},
                {0,0,0,1,1},
                {0,0,0,1,1}
            };
            return solution.maxAreaOfIsland(grid) == 4;
        });

        runTest("最大面积 - 空网格", [this]() {
            std::vector<std::vector<int>> grid;
            return solution.maxAreaOfIsland(grid) == 0;
        });

        runTest("最大面积 - 全水域", [this]() {
            std::vector<std::vector<int>> grid = {
                {0,0,0},
                {0,0,0},
                {0,0,0}
            };
            return solution.maxAreaOfIsland(grid) == 0;
        });

        runTest("最大面积 - 复杂形状", [this]() {
            std::vector<std::vector<int>> grid = {
                {1,1,1,0,0},
                {1,0,1,0,0},
                {1,1,1,0,0}
            };
            return solution.maxAreaOfIsland(grid) == 8;
        });

        // 周长测试
        runTest("周长 - 基本测试", [this]() {
            std::vector<std::vector<int>> grid = {
                {0,1,0,0},
                {1,1,1,0},
                {0,1,0,0},
                {1,1,0,0}
            };
            return solution.islandPerimeter(grid) == 16;
        });

        runTest("周长 - 单个方块", [this]() {
            std::vector<std::vector<int>> grid = {
                {0,0,0},
                {0,1,0},
                {0,0,0}
            };
            return solution.islandPerimeter(grid) == 4;
        });

        runTest("周长 - L形状", [this]() {
            std::vector<std::vector<int>> grid = {
                {1,0},
                {1,1}
            };
            return solution.islandPerimeter(grid) == 8;
        });

        runTest("周长 - 环形", [this]() {
            std::vector<std::vector<int>> grid = {
                {1,1,1},
                {1,0,1},
                {1,1,1}
            };
            return solution.islandPerimeter(grid) == 16;
        });

        // 不同岛屿测试
        runTest("不同岛屿 - 基本测试", [this]() {
            std::vector<std::vector<int>> grid = {
                {1,1,0,1,1},
                {1,0,0,0,0},
                {0,0,0,0,1},
                {1,1,0,1,1}
            };
            return solution.numDistinctIslands(grid) == 3;
        });

        runTest("不同岛屿 - 旋转相同形状", [this]() {
            std::vector<std::vector<int>> grid = {
                {1,1,0,0},
                {1,0,0,0},
                {0,0,0,1},
                {0,0,1,1}
            };
            return solution.numDistinctIslands(grid) == 2;
        });

        runTest("不同岛屿 - 对称形状", [this]() {
            std::vector<std::vector<int>> grid = {
                {1,1,0,1,1},
                {1,0,0,0,1},
                {0,0,0,0,0}
            };
            return solution.numDistinctIslands(grid) == 2;
        });

        runTest("不同岛屿 - 单点岛屿", [this]() {
            std::vector<std::vector<int>> grid = {
                {1,0,1,0},
                {0,1,0,1},
                {1,0,1,0}
            };
            return solution.numDistinctIslands(grid) == 1;
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
