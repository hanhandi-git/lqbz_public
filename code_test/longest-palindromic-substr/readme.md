# 最长回文子串

## 问题链接
[LeetCode 5. 最长回文子串](https://leetcode.com/problems/longest-palindromic-substring/)

## 问题描述
给你一个字符串 s，找到 s 中最长的回文子串。

如果字符串的反序与原始字符串相同，则该字符串称为回文字符串。

## 解题思路
1. 使用动态规划方法解决此问题。
    定义dp[i][j]表示字符串s从索引i到j的子串是否为回文串。
    状态转移方程：
    - 当s[i] == s[j]时，如果j-i <= 2，则dp[i][j] = true
    - 当s[i] == s[j]时，如果j-i > 2，则dp[i][j] = dp[i+1][j-1]
    - 当s[i] != s[j]时，dp[i][j] = false
2. 使用中心扩散法解决此问题

## 代码实现
```cpp
class Solution {
public:
    string longestPalindrome(string s) {
        int n = s.length();
        // dp[i][j] 表示s[i..j]是否是回文串
        vector<vector<bool>> dp(n, vector<bool>(n, false));
        int start = 0;  // 最长回文子串的起始位置
        int maxLen = 1; // 最长回文子串的长度
        
        // 所有单个字符都是回文串
        for (int i = 0; i < n; i++) {
            dp[i][i] = true;
        }
        
        // 检查长度大于1的子串
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i < n - len + 1; i++) {
                int j = i + len - 1;
                
                if (len == 2) {
                    dp[i][j] = (s[i] == s[j]);
                } else {
                    dp[i][j] = (s[i] == s[j] && dp[i+1][j-1]);
                }
                
                if (dp[i][j] && len > maxLen) {
                    start = i;
                    maxLen = len;
                }
            }
        }
        
        return s.substr(start, maxLen);
    }
};
```
```cpp
class Solution {
public:
    pair<int, int> judge(const string& str, int left, int right) {
        while (left >= 0 && right < str.size()) {
            if (str[left] != str[right]) break;
            else {
                left--;
                right++;
            }
        }
        return {left + 1, right - 1};
    }
    string longestPalindrome(string s) {
        pair<int, int> res(0, 0);
        for (int i = 0; i < s.size(); i++) {
            auto l1 = judge(s, i, i);
            auto l2 = judge(s, i, i+1);
            int len1 = l1.second - l1.first + 1;
            int len2 = l2.second - l2.first + 1;
            int res_len = res.second - res.first + 1;
            if (len1 > res_len) res = l1;
            if (len2 > res_len) res = l2;
        }
        return s.substr(res.first, res.second - res.first + 1);
    }
};
```

## 复杂度分析
- 时间复杂度：O(n²)，其中n是字符串的长度
- 空间复杂度：O(n²)，需要n×n的dp数组

## 扩展：其他相关问题
1. 最长回文子序列
2. 回文子串的个数
3. 回文排列
解题思路：
* 中心扩展法
    * 遍历每个位置作为回文中心
    * 分别处理奇数长度和偶数长度的情况
    * 向两边扩展，直到不满足回文条件
* Manacher算法
    * 通过在字符间插入特殊字符，将奇偶统一处理
    * 利用已有回文信息，避免重复计算
    * 时间复杂度可以优化到O(n)

这些变体问题都是基于回文串的基本概念，但可能需要使用不同的算法策略来解决。
理解并掌握这些方法对于解决字符串处理问题特别有帮助，尤其是在处理回文相关的问题时。
