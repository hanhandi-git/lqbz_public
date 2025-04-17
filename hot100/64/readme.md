# 最小路径和

## 问题链接
https://leetcode.cn/problems/minimum-path-sum/

## 问题描述
给定一个包含非负整数的 m x n 网格 grid ，请找出一条从左上角到右下角的路径，使得路径上的数字总和为最小。

说明：每次只能向下或者向右移动一步。

### 示例 1：
```
输入：grid = [[1,3,1],[1,5,1],[4,2,1]]
输出：7
解释：因为路径 1→3→1→1→1 的总和最小。
```

### 示例 2：
```
输入：grid = [[1,2,3],[4,5,6]]
输出：12
```

### 提示：
- m == grid.length
- n == grid[i].length
- 1 <= m, n <= 200
- 0 <= grid[i][j] <= 100

## 解题思路

这是一个经典的动态规划问题。我们可以定义 dp[i][j] 表示从起点 (0,0) 到达位置 (i,j) 的最小路径和。

由于每次只能向下或向右移动，所以到达位置 (i,j) 的路径只能从 (i-1,j) 或 (i,j-1) 到达。因此状态转移方程为：

dp[i][j] = grid[i][j] + min(dp[i-1][j], dp[i][j-1])

边界条件：
- 对于第一行，只能从左边到达，所以 dp[0][j] = dp[0][j-1] + grid[0][j]
- 对于第一列，只能从上边到达，所以 dp[i][0] = dp[i-1][0] + grid[i][0]

## 代码实现

```cpp
class Solution {
public:
    int minPathSum(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid[0].size();
        
        // 创建 DP 表格
        vector<vector<int>> dp(m, vector<int>(n, 0));
        
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
                dp[i][j] = grid[i][j] + min(dp[i-1][j], dp[i][j-1]);
            }
        }
        
        return dp[m-1][n-1];
    }
};
```

## 空间优化版本

我们可以发现，计算 dp[i][j] 时只需要用到 dp[i-1][j] 和 dp[i][j-1]，因此可以使用一维数组优化空间复杂度：

```cpp
class Solution {
public:
    int minPathSum(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid[0].size();
        
        vector<int> dp(n, 0);
        
        // 初始化第一行
        dp[0] = grid[0][0];
        for (int j = 1; j < n; j++) {
            dp[j] = dp[j-1] + grid[0][j];
        }
        
        // 逐行更新 dp 数组
        for (int i = 1; i < m; i++) {
            dp[0] += grid[i][0];  // 更新第一列
            for (int j = 1; j < n; j++) {
                dp[j] = grid[i][j] + min(dp[j], dp[j-1]);
            }
        }
        
        return dp[n-1];
    }
};
```

## 原地修改版本

如果允许修改输入数组，我们可以直接在原数组上进行动态规划，不需要额外空间：

```cpp
class Solution {
public:
    int minPathSum(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid[0].size();
        
        // 初始化第一行
        for (int j = 1; j < n; j++) {
            grid[0][j] += grid[0][j-1];
        }
        
        // 初始化第一列
        for (int i = 1; i < m; i++) {
            grid[i][0] += grid[i-1][0];
        }
        
        // 填充其余位置
        for (int i = 1; i < m; i++) {
            for (int j = 1; j < n; j++) {
                grid[i][j] += min(grid[i-1][j], grid[i][j-1]);
            }
        }
        
        return grid[m-1][n-1];
    }
};
```

## 复杂度分析

- 时间复杂度：O(m*n)，其中 m 和 n 分别是网格的行数和列数
- 空间复杂度：
  - 标准动态规划：O(m*n)
  - 空间优化版本：O(n)
  - 原地修改版本：O(1)

## 相关题目
- [62. 不同路径](https://leetcode.cn/problems/unique-paths/)
- [63. 不同路径 II](https://leetcode.cn/problems/unique-paths-ii/)
- [174. 地下城游戏](https://leetcode.cn/problems/dungeon-game/)
- [931. 下降路径最小和](https://leetcode.cn/problems/minimum-falling-path-sum/)