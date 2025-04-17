# 目标和

## 问题链接
[LeetCode 494. 目标和](https://leetcode.com/problems/target-sum/)

## 问题描述
给你一个整数数组 nums 和一个整数 target 。

向数组中的每个整数前添加 '+' 或 '-' ，然后串联起所有整数，可以构造一个表达式：

- 例如，nums = [2, 1]，可以在 2 之前添加 '+'，在 1 之前添加 '-'，然后串联起来得到表达式 "+2-1" = 1。

返回可以通过上述方法构造的、运算结果等于 target 的不同表达式的数目。

## 解题思路
这个问题可以通过两种主要方法来解决：动态规划和回溯（DFS）。

### 方法一：动态规划（转化为子集和问题）
1. 将问题转化为子集和问题：
   - 设数组所有元素之和为 sum
   - 设添加 '+' 号的元素和为 P，添加 '-' 号的元素和为 N
   - 有 P + N = sum 且 P - N = target
   - 解得 P = (target + sum) / 2
   - 因此，问题转化为：找到一个子集，使其和为 (target + sum) / 2

2. 动态规划步骤：
   - 定义 dp[i][j] 表示考虑前 i 个数字，组成和为 j 的方案数
   - 状态转移方程：dp[i][j] = dp[i-1][j] + dp[i-1][j-nums[i-1]]
   - 初始化：dp[0][0] = 1，表示不选任何数字和为0的方案数为1

### 方法二：回溯法（DFS）
1. 使用深度优先搜索遍历所有可能的表达式组合。
2. 对于每个数字，尝试添加 '+' 或 '-'，然后递归到下一个数字。
3. 当处理完所有数字后，检查最终结果是否等于目标和。

## 代码实现
```cpp
class Solution {
public:
    int findTargetSumWays(vector<int>& nums, int target) {
        int sum = 0;
        for (int num : nums) {
            sum += num;
        }
        
        // 如果target绝对值大于sum，或者(target + sum)为奇数，则无解
        if (abs(target) > sum || (target + sum) % 2 != 0) {
            return 0;
        }
        
        int positiveSum = (target + sum) / 2;
        
        // 动态规划求解子集和问题
        vector<int> dp(positiveSum + 1, 0);
        dp[0] = 1;  // 空集和为0的方案数为1
        
        for (int num : nums) {
            for (int j = positiveSum; j >= num; j--) {
                dp[j] += dp[j - num];
            }
        }
        
        return dp[positiveSum];
    }
};
```

## 复杂度分析
- 时间复杂度：O(n * target)，其中 n 是数组长度，target 是目标和。
- 空间复杂度：O(target)，使用了一维dp数组。

## 示例说明
以下是一个具体的例子，说明动态规划方法的工作过程：

输入：
- nums = [1, 1, 1, 1, 1]
- target = 3

分析：
1. 数组总和 sum = 5
2. 计算 (target + sum) / 2 = (3 + 5) / 2 = 4
3. 问题转化为：在nums中找到一个子集，使其和为4
4. 初始化dp[0] = 1
5. 遍历数组 nums:
   - 对于 num = 1:
     - dp[4] += dp[3] = 0
     - dp[3] += dp[2] = 0
     - dp[2] += dp[1] = 0
     - dp[1] += dp[0] = 1
   - 对于下一个 num = 1:
     - dp[4] += dp[3] = 1
     - dp[3] += dp[2] = 1
     - dp[2] += dp[1] = 1
     - dp[1] += dp[0] = 1
   - 继续处理剩余元素...
6. 最终 dp[4] = 5，表示有5种不同的方式可以组成和为4的子集

输出：5

这5种方式分别为：
- +1+1+1+1+1 = 5，组成的子集和为5
- +1+1+1-1+1 = 3，组成的子集和为4
- +1+1-1+1+1 = 3，组成的子集和为4
- +1-1+1+1+1 = 3，组成的子集和为4
- -1+1+1+1+1 = 3，组成的子集和为4