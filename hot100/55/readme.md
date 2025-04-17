## 问题链接
https://leetcode.cn/problems/jump-game/

## 问题描述
给定一个非负整数数组 `nums`，其中 `nums[i]` 表示你在第 `i` 个位置可以跳跃的最大长度。判断你是否能够到达最后一个索引。

### 示例 1：
输入：
```
nums = [2, 3, 1, 1, 4]
```
输出：true
解释：从索引 0 跳到 1，然后跳到 4。

### 示例 2：
输入：
```
nums = [3, 2, 1, 0, 4]
```
输出：false
解释：无法到达最后一个索引。

### 提示：
- `1 <= nums.length <= 10^4`
- `0 <= nums[i] <= 10^4`

## 解题思路

### 方法一：贪心算法
为了判断是否能够到达最后一个索引，我们可以使用贪心算法。具体步骤如下：

1. 初始化一个变量 `max_reachable`，用于存储当前能够到达的最远位置。
2. 遍历数组，对于每个位置 `i`：
   - 如果 `i` 大于 `max_reachable`，说明无法到达该位置，返回 false。
   - 更新 `max_reachable` 为 `max(max_reachable, i + nums[i])`，即更新能够到达的最远位置。
   - 如果 `max_reachable` 大于或等于最后一个索引，返回 true。
3. 遍历结束后，如果没有返回 false，说明可以到达最后一个索引，返回 true。

### 代码实现

```cpp
class Solution {
public:
    bool canJump(vector<int>& nums) {
        int max_reachable = 0;  // 初始化为0

        for (int i = 0; i < nums.size(); i++) {
            if (i > max_reachable) {
                return false;  // 无法到达该位置
            }
            max_reachable = max(max_reachable, i + nums[i]);  // 更新最远可达位置
            if (max_reachable >= nums.size() - 1) {
                return true;  // 可以到达最后一个索引
            }
        }

        return true;  // 遍历结束，能够到达最后一个索引
    }
};
```

## 复杂度分析

### 时间复杂度
- O(n)：每个位置只被访问一次。

### 空间复杂度
- O(1)：只使用了常数级别的额外空间。

## 优化技巧

1. 提前处理特殊情况：
   - 如果数组长度为1，直接返回true。
   
2. 使用贪心算法，避免不必要的重复计算。

3. 只需一次遍历，提升效率。

## 相关题目
- 45. 跳跃游戏 II
- 134. 加油站
- 55. 跳跃游戏

这些题目都涉及到跳跃和路径选择的策略，值得一起学习和比较。