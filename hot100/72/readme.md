# 编辑距离

## 问题链接
[LeetCode 72. 编辑距离](https://leetcode.com/problems/edit-distance/)

## 问题描述
给你两个单词 word1 和 word2，请你计算出将 word1 转换成 word2 所使用的最少操作数。

你可以对一个单词进行如下三种操作：
1. 插入一个字符
2. 删除一个字符
3. 替换一个字符

示例:
输入: `word1 = "horse", word2 = "ros"`
输出: `3`
解释: 
- horse -> rorse (将 'h' 替换为 'r')
- rorse -> rose (删除 'r')
- rose -> ros (删除 'e')

输入: `word1 = "intention", word2 = "execution"`
输出: `5`
解释: 
- intention -> inention (删除 't')
- inention -> enention (将 'i' 替换为 'e')
- enention -> exention (将 'n' 替换为 'x')
- exention -> exection (将 'n' 替换为 'c')
- exection -> execution (插入 'u')

## 解题思路
这道题是经典的编辑距离问题，可以使用动态规划来解决。

### 动态规划
1. 定义状态：`dp[i][j]` 表示将 word1 的前 i 个字符转换为 word2 的前 j 个字符所需的最少操作次数。
2. 初始化：
   - `dp[i][0] = i`：将 word1 的前 i 个字符转换为空字符串，需要删除 i 次
   - `dp[0][j] = j`：将空字符串转换为 word2 的前 j 个字符，需要插入 j 次
3. 状态转移方程：
   - 如果 `word1[i-1] == word2[j-1]`，则 `dp[i][j] = dp[i-1][j-1]`（不需要操作）
   - 否则，`dp[i][j] = min(dp[i-1][j-1], dp[i][j-1], dp[i-1][j]) + 1`
     - `dp[i-1][j-1] + 1`：替换操作
     - `dp[i][j-1] + 1`：插入操作
     - `dp[i-1][j] + 1`：删除操作

4. 最终返回 `dp[m][n]`，其中 m 和 n 分别是 word1 和 word2 的长度。

## 代码实现
```cpp
class Solution {
public:
    int minDistance(string word1, string word2) {
        int m = word1.length();
        int n = word2.length();
        // 创建 DP 表格
        vector<vector<int>> dp(m + 1, vector<int>(n + 1));
        // 初始化边界条件
        for (int i = 0; i <= m; i++) {
            dp[i][0] = i;  // 将 word1 的前 i 个字符转换为空字符串
        }
        for (int j = 0; j <= n; j++) {
            dp[0][j] = j;  // 将空字符串转换为 word2 的前 j 个字符
        }
        // 填充 DP 表格
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (word1[i-1] == word2[j-1]) {
                    // 如果当前字符相同，不需要操作
                    dp[i][j] = dp[i-1][j-1];
                } else {
                    // 取三种操作中的最小值 + 1
                    dp[i][j] = min(min(dp[i-1][j], dp[i][j-1]), dp[i-1][j-1]) + 1;
                }
            }
        }
        return dp[m][n];
    }
};
```

## 优化空间复杂度
我们可以将空间复杂度从 O(m*n) 优化到 O(min(m,n))，因为每次计算 dp[i][j] 时只需要用到 dp[i-1][j-1]、dp[i-1][j] 和 dp[i][j-1]。

```cpp
class Solution {
public:
    int minDistance(string word1, string word2) {
        int m = word1.length();
        int n = word2.length();
        // 确保 word1 是较短的字符串，优化空间
        if (m > n) {
            return minDistance(word2, word1);
        }
        vector<int> dp(m + 1);
        // 初始化第一行
        for (int i = 0; i <= m; i++) {
            dp[i] = i;
        }
        // 填充 DP 表格
        for (int j = 1; j <= n; j++) {
            int prev = dp[0];
            dp[0] = j;
            for (int i = 1; i <= m; i++) {
                int temp = dp[i];
                if (word1[i-1] == word2[j-1]) {
                    dp[i] = prev;
                } else {
                    dp[i] = min(min(dp[i-1], dp[i]), prev) + 1;
                }
                prev = temp;
            }
        }
        return dp[m];
    }
};
```

## 复杂度分析
- 时间复杂度：O(m*n)
  - 需要填充大小为 m*n 的 DP 表格。
- 空间复杂度：
  - 基础实现：O(m*n)
  - 优化实现：O(min(m,n))

## 关键点
1. 理解编辑距离的三种基本操作：插入、删除和替换。
2. 正确定义状态和状态转移方程。
3. 处理好边界情况，特别是空字符串的情况。
4. 理解如何优化空间复杂度，只保留必要的状态信息。
5. 注意字符索引与 DP 表格索引的对应关系（字符索引从0开始，DP表格索引从1开始）。
