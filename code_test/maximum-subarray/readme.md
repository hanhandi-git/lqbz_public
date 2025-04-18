# 最大子数组之和

## 问题链接
[LeetCode 53. 最大子数组和](https://leetcode.com/problems/maximum-subarray/)

## 问题描述
给你一个整数数组 nums ，请你找出一个具有最大和的连续子数组（子数组最少包含一个元素），返回其最大和。

子数组 是数组中的一个连续部分。

## 解题思路
1. 使用动态规划（Kadane's算法）来解决这个问题。
2. 遍历数组，对于每个元素，我们有两个选择：
   - 将当前元素加入前面的子数组
   - 以当前元素开始一个新的子数组
3. 我们选择这两者中的较大值，并更新最大和。

## 代码实现
```cpp
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int maxSum = nums[0];  // 初始化最大和为第一个元素
        int currentSum = nums[0];  // 当前子数组的和
        
        for (int i = 1; i < nums.size(); i++) {
            // 选择将当前元素加入前面的子数组，或者开始一个新的子数组
            currentSum = max(nums[i], currentSum + nums[i]);
            // 更新最大和
            maxSum = max(maxSum, currentSum);
        }
        
        return maxSum;
    }
};
```

## 复杂度分析
- 时间复杂度：O(n)，其中 n 是数组的长度。我们只需要遍历一次数组。
- 空间复杂度：O(1)，只使用了常数额外空间。

## 扩展：其他相关问题

### 1. 最大子矩阵和

问题描述：给定一个二维整数矩阵，找出其中和最大的子矩阵，返回该子矩阵的和。

例子：
```
输入：
[
  [1,  4, -5],
  [-7, 3,  6],
  [3,  2,  1]
]
输出：11
解释：和最大的子矩阵为 [3, 6] 和 [2, 1]，它们的和为 11。
```

解题思路：
* 可以将二维问题转化为一维问题，对每一列进行压缩，然后使用最大子数组和的算法。
* 具体步骤：
  1. 枚举上下边界
  2. 将上下边界之间的元素按列求和，得到一个一维数组
  3. 对这个一维数组应用最大子数组和算法
  4. 更新全局最大和

### 2. 买卖股票的最佳时机

问题描述：给定一个数组 prices ，它的第 i 个元素 prices[i] 表示一支给定股票第 i 天的价格。你只能选择 某一天 买入这只股票，并选择在 未来的某一个不同的日子 卖出该股票。设计一个算法来计算你所能获取的最大利润。

例子：
```
输入：[7,1,5,3,6,4]
输出：5
解释：在第 2 天（股票价格 = 1）的时候买入，在第 5 天（股票价格 = 6）的时候卖出，最大利润 = 6-1 = 5 。
```

解题思路：
* 可以转化为寻找数组中差值最大的两个数，其中较小的数在较大数之前。
* 遍历数组，记录当前遇到的最小价格，并计算当前价格与最小价格的差值，更新最大利润。

### 3. 环形子数组的最大和

问题描述：给定一个由整数数组 A 表示的环形数组 C，求 C 的非空子数组的最大可能和。在此问题中，环形数组意味着数组的末端将会与开头相连呈环状。

例子：
```
输入：[1,-2,3,-2]
输出：3
解释：从子数组 [3] 得到最大和 3
```

解题思路：

环形子数组的最大和可能有两种情况：
a) 最大和子数组不跨越数组的首尾（这种情况与普通的最大子数组和问题相同）
b) 最大和子数组跨越了数组的首尾
对于情况a，我们可以直接使用Kadane算法求得最大子数组和（maxSum）。
对于情况b，我们可以通过以下方式求解：
计算整个数组的总和（totalSum）
找出数组中和最小的子数组（minSum）
那么跨越首尾的最大和就是 totalSum - minSum
最终的结果是这两种情况的较大值：max(maxSum, totalSum - minSum)
5. 需要注意的特殊情况：如果数组中所有元素都是负数，那么 totalSum - minSum 会等于0（因为minSum会等于totalSum）。在这种情况下，我们应该直接返回maxSum。

这些变体问题都是基于"最大子数组和"的基本思想，但可能需要使用不同的技巧或算法来解决。它们展示了如何将一个基本问题扩展到更复杂的场景，以及如何灵活运用动态规划的思想来解决各种子数组或子序列的问题。
