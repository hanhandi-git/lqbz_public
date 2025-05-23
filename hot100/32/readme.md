# 最长有效括号

## 问题链接
[LeetCode 32. 最长有效括号](https://leetcode.com/problems/longest-valid-parentheses/)

## 问题描述
给你一个只包含 '(' 和 ')' 的字符串，找出最长有效（格式正确且连续）括号子串的长度。

示例:
输入: `s = "(()"` 
输出: `2`
解释: 最长有效括号子串是 "()"

输入: `s = ")()())"` 
输出: `4`
解释: 最长有效括号子串是 "()()"

输入: `s = ""` 
输出: `0`

## 解题思路
这道题可以通过多种方法解决，包括动态规划、栈和两次遍历。

### 方法一：动态规划
1. 定义状态：`dp[i]` 表示以第 i 个字符结尾的最长有效括号的长度。
2. 状态转移方程：
   - 如果 `s[i] = '('`，则 `dp[i] = 0`，因为有效括号子串不可能以 '(' 结尾。
   - 如果 `s[i] = ')'`：
     - 如果 `s[i-1] = '('`，则 `dp[i] = dp[i-2] + 2`（匹配前一个字符，加上之前的有效长度）。
     - 如果 `s[i-1] = ')'` 且 `s[i-dp[i-1]-1] = '('`，则 `dp[i] = dp[i-1] + 2 + dp[i-dp[i-1]-2]`（匹配更前面的字符，并加上之前的有效长度）。

### 方法二：栈
1. 使用栈来跟踪可能匹配的括号位置。
2. 初始化栈，并放入 -1 作为哨兵（便于计算长度）。
3. 遍历字符串：
   - 如果是 '('，将当前索引入栈。
   - 如果是 ')'：
     - 弹出栈顶元素。
     - 如果栈为空，将当前索引入栈作为新的哨兵。
     - 如果栈不为空，计算当前有效括号长度：`i - 栈顶元素`。

### 方法三：两次遍历
1. 从左到右遍历，用两个计数器记录左右括号数量：
   - 遇到 '(' 时，左括号计数器加一。
   - 遇到 ')' 时，右括号计数器加一。
   - 如果左右括号数量相等，更新最大长度。
   - 如果右括号数量大于左括号数量，重置两个计数器。
2. 从右到左再次遍历，处理左括号数量大于右括号数量的情况。

## 示例演示

### 方法一：动态规划
以 `s = ")()())"` 为例：

1. 初始化 `dp = [0, 0, 0, 0, 0, 0]`
2. `s[0] = ')'`，不满足条件，`dp[0] = 0`
3. `s[1] = '('`，不可能以 '(' 结尾，`dp[1] = 0`
4. `s[2] = ')'`，前一个字符是 '('，`dp[2] = dp[0] + 2 = 2`
5. `s[3] = '('`，不可能以 '(' 结尾，`dp[3] = 0`
6. `s[4] = ')'`，前一个字符是 '('，`dp[4] = dp[2] + 2 = 4`
7. `s[5] = ')'`，前一个字符是 ')'，检查 `s[5-dp[4]-1] = s[0] = ')'`，不匹配，`dp[5] = 0`
8. 最长有效括号长度为 4

### 方法二：栈
以 `s = ")()())"` 为例：

1. 初始化栈 `stack = [-1]`
2. `s[0] = ')'`，弹出 -1，栈为空，将 0 入栈，`stack = [0]`
3. `s[1] = '('`，将 1 入栈，`stack = [0, 1]`
4. `s[2] = ')'`，弹出 1，计算长度 `2 - 0 = 2`，`stack = [0]`
5. `s[3] = '('`，将 3 入栈，`stack = [0, 3]`
6. `s[4] = ')'`，弹出 3，计算长度 `4 - 0 = 4`，`stack = [0]`
7. `s[5] = ')'`，弹出 0，栈为空，将 5 入栈，`stack = [5]`
8. 最长有效括号长度为 4

## 代码实现

### 方法一：动态规划
```cpp
class Solution {
public:
    int longestValidParentheses(string s) {
        int n = s.length();
        if (n == 0) return 0;
        
        vector<int> dp(n, 0);
        int maxLen = 0;
        
        for (int i = 1; i < n; i++) {
            if (s[i] == ')') {
                if (s[i-1] == '(') {
                    dp[i] = (i >= 2 ? dp[i-2] : 0) + 2;
                } else if (i - dp[i-1] > 0 && s[i - dp[i-1] - 1] == '(') {
                    dp[i] = dp[i-1] + 2 + (i - dp[i-1] >= 2 ? dp[i - dp[i-1] - 2] : 0);
                }
                maxLen = max(maxLen, dp[i]);
            }
        }
        
        return maxLen;
    }
};
```

### 方法二：栈
```cpp
class Solution {
public:
    int longestValidParentheses(string s) {
        int maxLen = 0;
        stack<int> st;
        st.push(-1);
        
        for (int i = 0; i < s.length(); i++) {
            if (s[i] == '(') {
                st.push(i);
            } else {
                st.pop();
                if (st.empty()) {
                    st.push(i);
                } else {
                    maxLen = max(maxLen, i - st.top());
                }
            }
        }
        
        return maxLen;
    }
};
```

### 方法三：两次遍历
```cpp
class Solution {
public:
    int longestValidParentheses(string s) {
        int left = 0, right = 0, maxLen = 0;
        
        // 从左到右遍历
        for (int i = 0; i < s.length(); i++) {
            if (s[i] == '(') {
                left++;
            } else {
                right++;
            }
            
            if (left == right) {
                maxLen = max(maxLen, 2 * right);
            } else if (right > left) {
                left = right = 0;
            }
        }
        
        left = right = 0;
        
        // 从右到左遍历
        for (int i = s.length() - 1; i >= 0; i--) {
            if (s[i] == '(') {
                left++;
            } else {
                right++;
            }
            
            if (left == right) {
                maxLen = max(maxLen, 2 * left);
            } else if (left > right) {
                left = right = 0;
            }
        }
        
        return maxLen;
    }
};
```

## 复杂度分析
### 方法一：动态规划
- 时间复杂度：O(n)，其中 n 是字符串的长度，只需要遍历一次字符串。
- 空间复杂度：O(n)，需要一个长度为 n 的数组来存储动态规划的状态。

### 方法二：栈
- 时间复杂度：O(n)，遍历字符串一次。
- 空间复杂度：O(n)，栈的大小最多为 n。

### 方法三：两次遍历
- 时间复杂度：O(n)，需要遍历字符串两次。
- 空间复杂度：O(1)，只需要常数个变量。

## 关键点
1. 动态规划方法中，状态转移方程的设计是关键，特别是处理 ')' 时的两种情况。
2. 栈方法中，使用 -1 作为初始哨兵值很巧妙，便于计算有效括号的长度。
3. 两次遍历方法简单直观，分别处理了右括号多和左括号多的情况。
4. 理解有效括号的定义：必须是连续的，且格式正确（左右括号匹配）。
