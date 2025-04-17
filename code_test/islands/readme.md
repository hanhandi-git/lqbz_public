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

## 扩展：其他相关问题
1. 岛屿的最大面积
2. 岛屿的周长
3. 不同岛屿的数量

解题思路变体：
* 对于最大面积问题：在DFS过程中统计连通区域的大小
* 对于周长问题：统计每个陆地格子贡献的边长
* 对于不同岛屿问题：使用序列化方法记录每个岛屿的形状

这些变体都是基于基本的岛屿问题，通过DFS或BFS来解决，但在遍历过程中需要收集不同的信息。理解了基本的岛屿问题后，这些变体就是在原有思路上增加额外的处理逻辑。

## 扩展问题详解

### 1. 岛屿的最大面积
#### 问题链接
[LeetCode 695. 岛屿的最大面积](https://leetcode.com/problems/max-area-of-island/)

#### 解题思路
- 类似于岛屿数量的解法，但在DFS过程中需要统计每个岛屿的面积
- 维护一个全局最大值，在每次DFS结束后更新

```cpp
class Solution {
public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        int maxArea = 0;
        int m = grid.size(), n = grid[0].size();
        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    maxArea = max(maxArea, dfs(grid, i, j));
                }
            }
        }
        return maxArea;
    }
    
private:
    int dfs(vector<vector<int>>& grid, int i, int j) {
        if (i < 0 || i >= grid.size() || j < 0 || j >= grid[0].size() || grid[i][j] == 0) {
            return 0;
        }
        
        grid[i][j] = 0;  // 标记已访问
        return 1 + dfs(grid, i+1, j) + dfs(grid, i-1, j) + 
                   dfs(grid, i, j+1) + dfs(grid, i, j-1);
    }
};
```

#### 复杂度分析
- 时间复杂度：O(M × N)
- 空间复杂度：O(M × N)，最坏情况下的递归栈深度

### 2. 岛屿的周长
#### 问题链接
[LeetCode 463. 岛屿的周长](https://leetcode.com/problems/island-perimeter/)

#### 解题思路
- 对于每个陆地格子，计算其贡献的边长
- 每个陆地格子最多贡献4条边
- 如果相邻位置也是陆地，则减去相邻的边

```cpp
class Solution {
public:
    int islandPerimeter(vector<vector<int>>& grid) {
        int perimeter = 0;
        int m = grid.size(), n = grid[0].size();
        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    perimeter += 4;  // 每个陆地格子贡献4条边
                    
                    // 检查上方和左方是否有陆地，如果有则减去2条重复的边
                    if (i > 0 && grid[i-1][j] == 1) perimeter -= 2;
                    if (j > 0 && grid[i][j-1] == 1) perimeter -= 2;
                }
            }
        }
        return perimeter;
    }
};
```

#### 复杂度分析
- 时间复杂度：O(M × N)
- 空间复杂度：O(1)

### 3. 不同岛屿的数量
#### 问题链接
[LeetCode 694. 不同岛屿的数量](https://leetcode.com/problems/number-of-distinct-islands/)

#### 解题思路
- 使用相对坐标序列化每个岛屿的形状
- 使用哈希集合存储不同的岛屿形状
- DFS过程中记录移动方向，生成唯一的路径字符串

```cpp
class Solution {
public:
    int numDistinctIslands(vector<vector<int>>& grid) {
        unordered_set<string> islands;
        int m = grid.size(), n = grid[0].size();
        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    string path;
                    dfs(grid, i, j, i, j, path);
                    islands.insert(path);
                }
            }
        }
        return islands.size();
    }
    
private:
    void dfs(vector<vector<int>>& grid, int i, int j, int baseI, int baseJ, string& path) {
        if (i < 0 || i >= grid.size() || j < 0 || j >= grid[0].size() || grid[i][j] == 0) {
            return;
        }
        
        grid[i][j] = 0;  // 标记已访问
        // 记录相对坐标
        path += to_string(i - baseI) + "," + to_string(j - baseJ) + "|";
        
        dfs(grid, i+1, j, baseI, baseJ, path);
        dfs(grid, i-1, j, baseI, baseJ, path);
        dfs(grid, i, j+1, baseI, baseJ, path);
        dfs(grid, i, j-1, baseI, baseJ, path);
    }
};
```

#### 复杂度分析
- 时间复杂度：O(M × N)
- 空间复杂度：O(M × N)，用于存储不同岛屿的形状字符串

### 总结
这些岛屿问题的变体展示了如何通过修改DFS/BFS的过程来解决不同的需求：
1. 最大面积问题通过在DFS中累加面积
2. 周长问题通过计算每个格子的边界贡献
3. 不同岛屿问题通过序列化岛屿形状来判断唯一性

理解这些变体有助于掌握图形问题的通用解决思路，同时也展示了如何灵活运用DFS来收集不同类型的信息。
