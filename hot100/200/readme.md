# 岛屿数量

## 问题链接
[LeetCode 200. 岛屿数量](https://leetcode.com/problems/number-of-islands/)

## 问题描述
给你一个由 '1'（陆地）和 '0'（水）组成的二维网格 grid，请你计算网格中岛屿的数量。

岛屿总是被水包围，并且每座岛屿只能由水平方向和/或竖直方向上相邻的陆地连接形成。

此外，你可以假设该网格的四条边均被水包围。

## 解题思路
1. 使用深度优先搜索(DFS)来解决这个问题
2. 遍历整个网格，当遇到一个'1'时：
   - 计数器加1
   - 使用DFS将与之相连的所有'1'标记为已访问（通过将其改为'0'）
3. DFS会访问当前位置上、下、左、右四个相邻位置
4. 最终计数器的值就是岛屿的数量

## 代码实现
```cpp
class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;
        
        int islands = 0;
        int m = grid.size();
        int n = grid[0].size();
        
        // 遍历整个网格
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == '1') {
                    islands++;  // 发现新岛屿
                    dfs(grid, i, j);  // 标记与当前陆地相连的所有陆地
                }
            }
        }
        
        return islands;
    }
    
private:
    void dfs(vector<vector<char>>& grid, int i, int j) {
        int m = grid.size();
        int n = grid[0].size();
        
        // 边界检查和水域检查
        if (i < 0 || i >= m || j < 0 || j >= n || grid[i][j] == '0') {
            return;
        }
        
        // 标记当前陆地为已访问
        grid[i][j] = '0';
        
        // 访问四个方向的相邻位置
        dfs(grid, i-1, j);  // 上
        dfs(grid, i+1, j);  // 下
        dfs(grid, i, j-1);  // 左
        dfs(grid, i, j+1);  // 右
    }
};
```

## 复杂度分析
- 时间复杂度：O(M × N)，其中 M 和 N 分别是网格的行数和列数
- 空间复杂度：O(M × N)，在最坏情况下，整个网格都是陆地，递归深度为 M × N
