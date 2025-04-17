## LeetCode 45 II: 跳跃游戏 II

### 问题描述
给定一个非负整数数组 `nums`，其中 `nums[i]` 表示你在第 `i` 个位置可以跳跃的最大长度。计算到达最后一个索引所需的最小跳跃次数。

### 示例 1：
输入：
```
nums = [2, 3, 1, 1, 4]
```
输出：2  
解释：从索引 0 跳到 1，然后跳到 4。

### 示例 2：
输入：
```
nums = [2, 1]
```
输出：1  
解释：从索引 0 跳到 1。

### 提示：
- `1 <= nums.length <= 10^4`
- `0 <= nums[i] <= 10^4`

## 解题思路

### 方法一：贪心算法
为了计算到达最后一个索引所需的最小跳跃次数，我们可以使用贪心算法。具体步骤如下：

1. 初始化变量 `jumps`（跳跃次数）、`current_end`（当前跳跃的结束位置）和 `farthest`（当前能到达的最远位置）。
2. 遍历数组，对于每个位置 `i`：
   - 更新 `farthest` 为 `max(farthest, i + nums[i])`，即更新能够到达的最远位置。
   - 如果到达了 `current_end`，则增加跳跃次数 `jumps`，并更新 `current_end` 为 `farthest`。
3. 遍历结束后，返回 `jumps`。

### 代码实现

```cpp
class Solution {
public:
    int jump(vector<int>& nums) {
        int steps = 0;
        int start = 0;
        int end = 0;
        while (end < nums.size() - 1) {
            int max_distance = 0;
            for (int i = start; i <= end; i++) {
                max_distance = max(max_distance, nums[i] + i);
            }
            start = end + 1;
            end = max_distance;
            steps++;
        }
        return steps;
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
   - 如果数组长度为1，直接返回0。
   
2. 使用贪心算法，避免不必要的重复计算。

3. 只需一次遍历，提升效率。

## 相关题目
- 45. 跳跃游戏
- 134. 加油站
- 55. 跳跃游戏