# 最长公共子序列

## 问题链接
[LeetCode 1143. 最长公共子序列](https://leetcode.com/problems/longest-common-subsequence/)

## 问题描述
给定两个字符串 `text1` 和 `text2`，返回这两个字符串的最长公共子序列的长度。

一个字符串的子序列是指这样一个新的字符串：它是由原字符串在不改变字符的相对顺序的情况下删除某些字符（也可以不删除任何字符）后组成的新字符串。例如，"ace" 是 "abcde" 的子序列，但 "aec" 不是。两个字符串的「公共子序列」是这两个字符串所共同拥有的子序列。

若这两个字符串没有公共子序列，则返回 0。

## 解题思路
1. 使用动态规划来解决这个问题。
2. 定义 `dp[i][j]` 表示 `text1` 的前 `i` 个字符与 `text2` 的前 `j` 个字符的最长公共子序列的长度。
3. 状态转移方程：
   - 如果 `text1[i-1] == text2[j-1]`，则 `dp[i][j] = dp[i-1][j-1] + 1`
   - 否则，`dp[i][j] = max(dp[i-1][j], dp[i][j-1])`
4. 最终返回 `dp[m][n]`，其中 `m` 和 `n` 分别是 `text1` 和 `text2` 的长度。

## 代码实现
```cpp
class Solution {
public:
    int longestCommonSubsequence(string text1, string text2) {
        int m = text1.length();
        int n = text2.length();
        
        // 创建DP表格，初始化为0
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        
        // 填充DP表格
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (text1[i - 1] == text2[j - 1]) {
                    // 当前字符匹配，LCS长度加1
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    // 当前字符不匹配，取两种可能的最大值
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }
        
        // 返回最终结果
        return dp[m][n];
    }
};
```

## 复杂度分析
- 时间复杂度：O(M × N)，其中 M 和 N 分别是两个字符串的长度。
- 空间复杂度：O(M × N)，需要一个二维数组来存储动态规划的状态。

## 优化方案
可以将空间复杂度优化到 O(min(M, N))，因为每次计算 dp[i][j] 时只需要用到 dp[i-1][j-1]、dp[i-1][j] 和 dp[i][j-1]，所以可以只保留两行。进一步，可以只使用一行加一个变量来实现。
