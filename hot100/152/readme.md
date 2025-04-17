# 乘积最大子数组

## 问题链接
[LeetCode 152. Maximum Product Subarray](https://leetcode.com/problems/maximum-product-subarray/)

## 问题描述
给你一个整数数组 nums，请你找出数组中乘积最大的非空连续子数组（该子数组中至少包含一个数字），并返回该子数组所对应的乘积。

测试用例的答案是一个 32-位 整数。

子数组 是数组的连续子序列。

## 解题思路
1. 这道题看似和"最大子数组和"类似,但由于乘法可能涉及负数,所以需要特殊处理:
   - 当遇到负数时,最大值可能变成最小值,最小值可能变成最大值
   - 因此需要同时维护当前位置的最大值和最小值

2. 使用动态规划求解:
   - 定义 maxDP[i] 表示以 nums[i] 结尾的子数组的最大乘积
   - 定义 minDP[i] 表示以 nums[i] 结尾的子数组的最小乘积
   - 对于每个位置 i,有三种可能:
     * nums[i] 本身
     * nums[i] * maxDP[i-1]
     * nums[i] * minDP[i-1]

3. 状态转移方程:
   - maxDP[i] = max(nums[i], nums[i] * maxDP[i-1], nums[i] * minDP[i-1])
   - minDP[i] = min(nums[i], nums[i] * maxDP[i-1], nums[i] * minDP[i-1])

4. 由于每个位置只依赖前一个位置的状态,可以使用两个变量代替数组,优化空间复杂度。

## 代码实现
```cpp
class Solution {
public:
    int maxProduct(vector<int>& nums) {
        int maxF = nums[0], minF = nums[0], result = nums[0];
        
        for (int i = 1; i < nums.size(); i++) {
            int mx = maxF, mn = minF;
            maxF = max(nums[i], max(mx * nums[i], mn * nums[i]));
            minF = min(nums[i], min(mx * nums[i], mn * nums[i]));
            result = max(result, maxF);
        }
        
        return result;
    }
};
```

## 复杂度分析
- 时间复杂度：O(N)，其中 N 是数组的长度。我们只需要遍历一次数组。
- 空间复杂度：O(1)，只使用了常数个变量。

## 示例
```
输入: nums = [2,3,-2,4]
输出: 6
解释: 子数组 [2,3] 有最大乘积 6。

输入: nums = [-2,0,-1]
输出: 0
解释: 结果不能为 2, 因为 [-2,-1] 不是子数组。
```