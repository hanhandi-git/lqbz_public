# 打家劫舍

## 问题链接
[LeetCode 198. 打家劫舍](https://leetcode.com/problems/house-robber/)

## 问题描述
你是一个专业的小偷，计划偷窃沿街的房屋。每间房内都藏有一定的现金，影响你偷窃的唯一制约因素就是相邻的房屋装有相互连通的防盗系统。如果两间相邻的房屋在同一晚上被小偷闯入，系统会自动报警。

给定一个代表每个房屋存放金额的非负整数数组，计算你不触动警报装置的情况下，能够偷窃到的最高金额。

## 解题思路
1. 使用动态规划来解决这个问题。
2. 定义一个数组 `dp`，其中 `dp[i]` 表示偷窃到第 `i` 个房屋时能够获得的最高金额。
3. 对于每个房屋，有两个选择：
   - 不偷窃当前房屋，则最高金额为 `dp[i-1]`。
   - 偷窃当前房屋，则最高金额为 `dp[i-2] + nums[i]`。
4. 状态转移方程为：`dp[i] = max(dp[i-1], dp[i-2] + nums[i])`。
5. 初始条件：`dp[0] = nums[0]`，`dp[1] = max(nums[0], nums[1])`。
6. 最终结果为 `dp[n-1]`，其中 `n` 是房屋的数量。

## 代码实现
```cpp
class Solution {
public:
    int rob(vector<int>& nums) {
        if (nums.empty()) return 0;
        int n = nums.size();
        if (n == 1) return nums[0];
        
        vector<int> dp(n, 0);
        dp[0] = nums[0];
        dp[1] = max(nums[0], nums[1]);
        
        for (int i = 2; i < n; i++) {
            dp[i] = max(dp[i-1], dp[i-2] + nums[i]);
        }
        
        return dp[n-1];
    }
};
```

## 复杂度分析
- 时间复杂度：O(N)，其中 N 是房屋的数量。
- 空间复杂度：O(N)，用于存储动态规划数组。