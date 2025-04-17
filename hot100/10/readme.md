# 正则表达式匹配

## 问题链接
[LeetCode 10. 正则表达式匹配](https://leetcode.com/problems/regular-expression-matching/)

## 问题描述
给你一个字符串 s 和一个字符规律 p，请你来实现一个支持 '.' 和 '*' 的正则表达式匹配。

- '.' 匹配任意单个字符
- '*' 匹配零个或多个前面的那一个元素

所谓匹配，是要涵盖整个字符串 s，而不是部分字符串。

## 解题思路
1. 使用动态规划方法解决此问题。
   - 定义dp[i][j]表示字符串s的前i个字符和模式p的前j个字符是否能匹配
   - 当p[j]不是'*'时:
     - 如果当前字符匹配(s[i] == p[j] 或 p[j] == '.')，则dp[i][j] = dp[i-1][j-1]
     - 否则dp[i][j] = false
   - 当p[j]是'*'时:
     - 匹配0次: dp[i][j] = dp[i][j-2]
     - 匹配多次: 如果当前字符匹配，dp[i][j] = dp[i-1][j]

2. 初始化条件:
   - dp[0][0] = true 表示空字符串能够匹配
   - 处理p以'*'开头的特殊情况

## 代码实现
```cpp
class Solution {
public:
bool isMatch(string s, string p) {
int m = s.length(), n = p.length();
// dp[i][j]表示s的前i个字符和p的前j个字符是否匹配
vector<vector<bool>> dp(m + 1, vector<bool>(n + 1, false));
// 空字符串匹配
dp[0][0] = true;
// 处理p以''开头的情况
for (int j = 1; j <= n; j++) {
if (p[j-1] == '') {
dp[0][j] = dp[0][j-2];
}
}
// 动态规划填表
for (int i = 1; i <= m; i++) {
for (int j = 1; j <= n; j++) {
if (p[j-1] == '') {
// 不使用当前的''
dp[i][j] = dp[i][j-2];
// 使用当前的''匹配一次或多次
if (s[i-1] == p[j-2] || p[j-2] == '.') {
dp[i][j] = dp[i][j] || dp[i-1][j];
}
}
else if (s[i-1] == p[j-1] || p[j-1] == '.') {
dp[i][j] = dp[i-1][j-1];
}
}
}
return dp[m][n];
}
};
```


## 复杂度分析
- 时间复杂度：O(mn)，其中m和n分别是字符串s和p的长度
- 空间复杂度：O(mn)，需要m×n的dp数组