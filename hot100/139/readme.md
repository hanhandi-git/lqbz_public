# 单词拆分

## 问题链接
[LeetCode 139. 单词拆分](https://leetcode.com/problems/word-break/)

## 问题描述
给定一个字符串 `s` 和一个字符串字典 `wordDict`，判断 `s` 是否可以被空格拆分为一个或多个在字典中出现的单词。

## 逻辑推导流程
以下是获取单词拆分的逻辑推导流程：

1. **初始化**：
   - 使用动态规划数组 `dp`，其中 `dp[i]` 表示字符串 `s` 的前 `i` 个字符是否可以被拆分。

2. **动态规划状态转移**：
   - 遍历字符串 `s` 的每个位置 `i`，对于每个位置，检查从 `j` 到 `i` 的子字符串 `s[j:i]` 是否在字典中。
   - 如果 `dp[j]` 为 `true`，则 `dp[i]` 也应为 `true`。

3. **返回结果**：
   - 最终返回 `dp[s.length()]` 的值，表示整个字符串是否可以被拆分。

## 解题思路
1. 使用动态规划来判断字符串的可拆分性。
2. 维护一个布尔数组 `dp`，记录每个位置的拆分状态。
3. 通过遍历和字典查找，更新 `dp` 数组。

## 代码实现
```cpp
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> wordSet(wordDict.begin(), wordDict.end());
        vector<bool> dp(s.size() + 1, false);
        dp[0] = true; // 空字符串可以被拆分

        for (int i = 1; i <= s.size(); i++) {
            for (int j = 0; j < i; j++) {
                if (dp[j] && wordSet.count(s.substr(j, i - j))) {
                    dp[i] = true;
                    break;
                }
            }
        }
        return dp[s.size()];
    }
};
```

## 复杂度分析
- 时间复杂度：O(N^2)，其中 N 是字符串 `s` 的长度。
- 空间复杂度：O(N)，用于存储动态规划数组。

## 示例
假设我们有字符串 `s = "leetcode"` 和字典 `wordDict = ["leet", "code"]`，我们可以通过上述逻辑判断 `s` 可以被拆分为 `["leet", "code"]`。

### 逻辑推导示例
1. 字符串 `s = "leetcode"`，初始化 `dp` 数组为 `[true, false, false, false, false, false, false, false, false]`。
2. 通过动态规划更新 `dp` 数组，最终得到 `dp[8]` 为 `true`，表示字符串可以被拆分。