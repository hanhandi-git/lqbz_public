

# 不同路径

## 问题链接
https://leetcode.cn/problems/unique-paths/

## 问题描述
一个机器人位于一个 m x n 网格的左上角（起始点在下图中标记为 "Start" ）。

机器人每次只能向下或者向右移动一步。机器人试图达到网格的右下角（在下图中标记为 "Finish" ）。

问总共有多少条不同的路径？

### 示例 1：
```
输入：m = 3, n = 7
输出：28
```

### 示例 2：
```
输入：m = 3, n = 2
输出：3
解释：
从左上角开始，总共有 3 条路径可以到达右下角。
1. 向右 -> 向下 -> 向下
2. 向下 -> 向下 -> 向右
3. 向下 -> 向右 -> 向下
```

### 提示：
- 1 <= m, n <= 100
- 题目数据保证答案小于等于 2 * 10^9

## 解题思路

这是一个经典的动态规划问题。我们可以定义 dp[i][j] 表示从起点 (0,0) 到达位置 (i,j) 的不同路径数量。

由于机器人每次只能向下或向右移动，所以到达位置 (i,j) 的路径只能从 (i-1,j) 或 (i,j-1) 到达。因此状态转移方程为：

dp[i][j] = dp[i-1][j] + dp[i][j-1]

边界条件：
- 对于第一行，只能从左边到达，所以 dp[0][j] = 1
- 对于第一列，只能从上边到达，所以 dp[i][0] = 1

## 代码实现

```cpp
class Solution {
public:
    int uniquePaths(int m, int n) {
        // 创建 DP 表格
        vector<vector<int>> dp(m, vector<int>(n, 0));
        
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
```

## 空间优化版本

我们可以发现，计算 dp[i][j] 时只需要用到 dp[i-1][j] 和 dp[i][j-1]，因此可以使用一维数组优化空间复杂度：

```cpp
class Solution {
public:
    int uniquePaths(int m, int n) {
        vector<int> dp(n, 1);  // 初始化为 1，对应第一行
        
        for (int i = 1; i < m; i++) {
            for (int j = 1; j < n; j++) {
                dp[j] += dp[j-1];  // dp[j] 是上一行的值，dp[j-1] 是当前行左边的值
            }
        }
        
        return dp[n-1];
    }
};
```

## 数学解法

这个问题也可以用组合数学解决。从起点到终点需要移动 m-1 步向下和 n-1 步向右，总共 m+n-2 步。
问题转化为：从 m+n-2 步中选择 m-1 步向下（或选择 n-1 步向右），即组合数 C(m+n-2, m-1)。

```cpp
class Solution {
public:
    int uniquePaths(int m, int n) {
        long long ans = 1;
        for (int i = 1; i <= n - 1; i++) {
            ans = ans * (m - 1 + i) / i;
        }
        return (int)ans;
    }
};
```

## 复杂度分析

- 动态规划解法：
  - 时间复杂度：O(m*n)
  - 空间复杂度：O(m*n)，优化后为 O(n)
  
- 数学解法：
  - 时间复杂度：O(min(m,n))
  - 空间复杂度：O(1)

## 相关题目
- [63. 不同路径 II](https://leetcode.cn/problems/unique-paths-ii/)
- [64. 最小路径和](https://leetcode.cn/problems/minimum-path-sum/)
- [980. 不同路径 III](https://leetcode.cn/problems/unique-paths-iii/)
