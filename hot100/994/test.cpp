#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <unordered_set>
#include <queue>

class Solution {
public:
    std::vector<std::vector<int>> direction = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    int orangesRotting(std::vector<std::vector<int>>& grid) {
        if (grid.empty() || grid[0].empty()) return -1;
        int n = grid.size();
        int m = grid[0].size();
        std::queue<std::pair<int, int>> que;
        int freshCnt = 0;
        int times = 0;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                if (grid[i][j] == 1)
                {
                    freshCnt++;
                } else if (grid[i][j] == 2)
                {
                    que.push({i,j});
                }
            }
        }

        while (!que.empty())
        {
            int size = que.size();
            for (int i = 0; i < size; i++)
            {
                auto node = que.front();
                que.pop();
                int x = node.first;
                int y = node.second;
                //  向四周进行扩展
                for (int j = 0; j < 4; j++)
                {
                    int tmpX = x + direction[j][0];
                    int tmpY = y + direction[j][1];
                    if (tmpX < 0 || tmpX >= n || tmpY < 0 || tmpY >= m || grid[tmpX][tmpY] != 1) continue;
                    que.push({tmpX, tmpY});
                    grid[tmpX][tmpY] = 2;
                    freshCnt--;
                }
            }
            if (!que.empty()) times++;
        }
        if (freshCnt == 0) return times;
        else return -1;
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
        // 基本测试
        runTest("基本测试 - 所有橘子腐烂", [this]() {
            std::vector<std::vector<int>> grid = {
                {2, 1, 1},
                {1, 1, 0},
                {0, 1, 2}
            };
            return solution.orangesRotting(grid) == 4;
        });

        runTest("基本测试 - 无法腐烂所有橘子", [this]() {
            std::vector<std::vector<int>> grid = {
                {2, 1, 1},
                {0, 1, 1},
                {1, 0, 2}
            };
            return solution.orangesRotting(grid) == -1;
        });

        runTest("基本测试 - 没有新鲜橘子", [this]() {
            std::vector<std::vector<int>> grid = {
                {0, 0, 0},
                {0, 0, 0},
                {0, 0, 0}
            };
            return solution.orangesRotting(grid) == 0;
        });

        runTest("基本测试 - 所有橘子都是腐烂的", [this]() {
            std::vector<std::vector<int>> grid = {
                {2, 2, 2},
                {2, 2, 2},
                {2, 2, 2}
            };
            return solution.orangesRotting(grid) == 0;
        });

        runTest("边界测试 - 空网格", [this]() {
            std::vector<std::vector<int>> grid;
            return solution.orangesRotting(grid) == -1;
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