# 零钱兑换

## 问题链接
[LeetCode 322. 零钱兑换](https://leetcode.com/problems/coin-change/)

## 问题描述
给定不同面额的硬币 coins 和一个总金额 amount，计算可以凑成总金额所需的最少的硬币个数。如果没有任何一种硬币组合能组成总金额，返回 -1。

## 解题思路
1. 使用动态规划来解决这个问题。
2. 定义 `dp[i]` 为凑成金额 `i` 所需的最少硬币个数。
3. 初始化 `dp[0] = 0`，表示凑成金额 0 不需要任何硬币。
4. 对于每个金额 `i`，遍历所有硬币面额 `coin`，更新 `dp[i]` 的值：
   - 如果 `i - coin >= 0`，则可以通过 `dp[i - coin] + 1` 来更新 `dp[i]`。
5. 最终返回 `dp[amount]`，如果 `dp[amount]` 仍然是初始值，则返回 -1。

### 公式推导解释

1. **状态定义**：
   - `dp[i]` 表示凑成金额 `i` 所需的最少硬币个数。

2. **状态转移**：
   - 对于每个硬币面额 `coin`，我们可以通过 `dp[i - coin] + 1` 来更新 `dp[i]`，表示使用一个硬币 `coin` 后，剩余金额 `i - coin` 的最优解。

3. **初始化**：
   - 初始化 `dp` 数组为一个较大的值（如 `amount + 1`），表示初始状态下无法凑成该金额。

### 总结

通过这个动态规划的过程，我们可以逐步计算出每个金额的最少硬币个数。最终，`dp[amount]` 就是凑成总金额所需的最少硬币个数。如果无法凑成该金额，则返回 -1。

## 代码实现
```cpp
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        vector<int> dp(amount + 1, amount + 1);
        dp[0] = 0; // 凑成金额 0 不需要任何硬币
        
        for (int i = 1; i <= amount; i++) {
            for (int coin : coins) {
                if (i - coin >= 0) {
                    dp[i] = min(dp[i], dp[i - coin] + 1);
                }
            }
        }
        
        return dp[amount] == amount + 1 ? -1 : dp[amount];
    }
};
```

## 复杂度分析
- 时间复杂度：O(n * m)，其中 n 是金额，m 是硬币的种类数。
- 空间复杂度：O(n)，用于存储动态规划表。

好的，这就是根据 LeetCode 322 题修改后的 `readme.md` 文件内容。请根据需要进行进一步的调整。