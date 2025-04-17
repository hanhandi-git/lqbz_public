````thinking
让我思考一下如何修改这个readme来解释最大子数组和的问题...

首先需要理解题目的关键点：
1. 这是一个经典的动态规划问题
2. 也可以用分治法解决
3. 还有一个非常优雅的Kadane算法
4. 需要考虑负数的情况

解题思路重点：
- DP的状态定义和转移方程
- 如何处理全是负数的情况
- 如何优化空间复杂度
- 不同解法的优缺点比较

需要包含的内容：
- 清晰的问题描述
- 多种解法的详细说明
- 复杂度分析
- 代码实现
- 优化思路

让我开始修改内容...
````

# 最大子数组和

## 问题链接
https://leetcode.cn/problems/maximum-subarray/

## 问题描述
给你一个整数数组 `nums` ，请你找出一个具有最大和的连续子数组（子数组最少包含一个元素），返回其最大和。

子数组是数组中的一个连续部分。

### 示例 1：
```
输入：nums = [-2,1,-3,4,-1,2,1,-5,4]
输出：6
解释：连续子数组 [4,-1,2,1] 的和最大，为 6 。
```

### 示例 2：
```
输入：nums = [1]
输出：1
```

### 示例 3：
```
输入：nums = [5,4,-1,7,8]
输出：23
```

### 提示：
- 1 <= nums.length <= 105
- -104 <= nums[i] <= 104

## 解题思路

这个问题有多种解法，我们主要介绍三种方法：

### 1. 动态规划
- 定义dp[i]为以nums[i]结尾的最大子数组和
- 状态转移方程：dp[i] = max(nums[i], dp[i-1] + nums[i])
- 最终结果是dp数组中的最大值

### 2. Kadane算法
- 是动态规划的空间优化版本
- 用一个变量记录当前最大和
- 用另一个变量记录全局最大和

### 3. 分治法
- 将数组分成左右两部分
- 最大子数组和可能在：
  1. 左半部分
  2. 右半部分
  3. 跨越中点的子数组

## 代码实现

### 1. 动态规划解法
```cpp
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int n = nums.size();
        vector<int> dp(n);
        dp[0] = nums[0];
        int maxSum = dp[0];
        
        for (int i = 1; i < n; i++) {
            dp[i] = max(nums[i], dp[i-1] + nums[i]);
            maxSum = max(maxSum, dp[i]);
        }
        
        return maxSum;
    }
};
```

### 2. Kadane算法
```cpp
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int currentSum = nums[0];
        int maxSum = nums[0];
        
        for (int i = 1; i < nums.size(); i++) {
            currentSum = max(nums[i], currentSum + nums[i]);
            maxSum = max(maxSum, currentSum);
        }
        
        return maxSum;
    }
};
```

### 3. 分治法
```cpp
class Solution {
private:
    int maxCrossingSum(vector<int>& nums, int left, int mid, int right) {
        int leftSum = INT_MIN;
        int sum = 0;
        for (int i = mid; i >= left; i--) {
            sum += nums[i];
            leftSum = max(leftSum, sum);
        }
        
        int rightSum = INT_MIN;
        sum = 0;
        for (int i = mid + 1; i <= right; i++) {
            sum += nums[i];
            rightSum = max(rightSum, sum);
        }
        
        return leftSum + rightSum;
    }
    
    int maxSubArrayHelper(vector<int>& nums, int left, int right) {
        if (left == right) return nums[left];
        
        int mid = left + (right - left) / 2;
        return max({maxSubArrayHelper(nums, left, mid),
                   maxSubArrayHelper(nums, mid + 1, right),
                   maxCrossingSum(nums, left, mid, right)});
    }
    
public:
    int maxSubArray(vector<int>& nums) {
        return maxSubArrayHelper(nums, 0, nums.size() - 1);
    }
};
```

## 复杂度分析

### 动态规划
- 时间复杂度：O(n)
- 空间复杂度：O(n)

### Kadane算法
- 时间复杂度：O(n)
- 空间复杂度：O(1)

### 分治法
- 时间复杂度：O(n log n)
- 空间复杂度：O(log n)，递归调用栈的深度

## 优化思路

1. 空间优化：
   - 动态规划解法可以优化到O(1)空间，就是Kadane算法
   
2. 特殊情况处理：
   - 可以提前处理数组长度为1的情况
   - 可以提前处理全是负数的情况

3. 扩展功能：
   - 如果需要记录子数组的起始和结束位置，可以在遍历时额外记录

## 相关题目
- [152. 乘积最大子数组](https://leetcode.cn/problems/maximum-product-subarray/)
- [918. 环形子数组的最大和](https://leetcode.cn/problems/maximum-sum-circular-subarray/)
- [1186. 删除一次得到子数组最大和](https://leetcode.cn/problems/maximum-subarray-sum-with-one-deletion/)
