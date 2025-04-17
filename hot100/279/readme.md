# 完全平方数

## 问题链接
[LeetCode 279. 完全平方数](https://leetcode.com/problems/perfect-squares/)

## 问题描述
给你一个整数 n ，返回和为 n 的完全平方数的最少数量。

完全平方数是一个整数，其值等于另一个整数的平方；换句话说，其值等于一个整数自乘的积。例如，1、4、9 和 16 都是完全平方数，而 3 和 11 不是。

## 解题思路
1. 使用动态规划来解决这个问题。
2. 定义一个数组 `dp`，其中 `dp[i]` 表示组成数字 `i` 所需的最少完全平方数的数量。
3. 对于每个数字 i，遍历所有小于等于 i 的完全平方数 j*j：
   - 如果 i 可以由某个完全平方数 j*j 和一个更小的数 i-j*j 组成
   - 则 dp[i] 可以更新为 dp[i-j*j] + 1
4. 状态转移方程为：`dp[i] = min(dp[i], dp[i - j*j] + 1)`，其中 j*j <= i
5. 初始条件：`dp[0] = 0`
6. 最终结果为 `dp[n]`

## 代码实现
```cpp
class Solution {
public:
    int numSquares(int n) {
        vector<int> dp(n + 1, INT_MAX);
        dp[0] = 0;
        
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j * j <= i; j++) {
                dp[i] = min(dp[i], dp[i - j*j] + 1);
            }
        }
        
        return dp[n];
    }
};
```

## 复杂度分析
- 时间复杂度：O(n * sqrt(n))，其中外层循环为 n，内层循环为 sqrt(n)
- 空间复杂度：O(n)，用于存储动态规划数组
