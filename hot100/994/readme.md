# 腐烂的橘子

## 问题链接
[LeetCode 994. 腐烂的橘子](https://leetcode.com/problems/rotting-oranges/)

## 问题描述
在一个 `m x n` 的网格中，每个单元格可以是以下三种状态之一：
- `0`：空单元格
- `1`：新鲜橘子
- `2`：腐烂的橘子

每分钟，腐烂的橘子会使其四个方向上相邻的新鲜橘子腐烂。请你计算所有橘子腐烂所需的最小分钟数。如果无法使所有新鲜橘子腐烂，则返回 `-1`。

## 解题思路
1. 使用广度优先搜索（BFS）来解决这个问题。
2. 首先遍历整个网格，记录所有腐烂橘子的初始位置，并将新鲜橘子的数量计数。
3. 使用队列来进行 BFS，逐层处理腐烂橘子，更新相邻的新鲜橘子的状态。
4. 每处理一层，时间计数加1，直到队列为空。
5. 最后检查是否还有新鲜橘子，如果有则返回 `-1`，否则返回时间计数。

## 代码实现
```cpp
class Solution {
public:
    int orangesRotting(vector<vector<int>>& grid) {
        if (grid.empty() || grid[0].empty()) return -1;

        int freshCount = 0;
        queue<pair<int, int>> q;
        int minutes = 0;
        int m = grid.size();
        int n = grid[0].size();

        // 遍历整个网格，初始化队列和新鲜橘子计数
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    freshCount++;  // 统计新鲜橘子数量
                } else if (grid[i][j] == 2) {
                    q.push({i, j});  // 将腐烂橘子加入队列
                }
            }
        }

        // BFS 处理腐烂橘子
        while (!q.empty()) {
            int size = q.size();
            for (int i = 0; i < size; i++) {
                auto [x, y] = q.front();
                q.pop();

                // 访问四个方向的相邻位置
                for (auto& dir : directions) {
                    int newX = x + dir[0];
                    int newY = y + dir[1];

                    // 边界检查和新鲜橘子检查
                    if (newX >= 0 && newX < m && newY >= 0 && newY < n && grid[newX][newY] == 1) {
                        grid[newX][newY] = 2;  // 腐烂新鲜橘子
                        freshCount--;  // 新鲜橘子数量减1
                        q.push({newX, newY});  // 将新腐烂的橘子加入队列
                    }
                }
            }
            if (!q.empty()) minutes++;  // 只有在队列不为空时才增加时间
        }

        return freshCount == 0 ? minutes : -1;  // 检查是否还有新鲜橘子
    }

private:
    vector<vector<int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};  // 上、下、左、右
};
```

## 复杂度分析
- 时间复杂度：O(M × N)，其中 M 和 N 分别是网格的行数和列数。
- 空间复杂度：O(M × N)，在最坏情况下，队列可能会存储整个网格的橘子。
