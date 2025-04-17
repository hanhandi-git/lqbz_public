## 问题链接
[LeetCode - 二分查找](https://leetcode.cn/problems/binary-search/)

## 问题描述
给定一个升序排列的整数数组 nums 和一个目标值 target，写一个函数搜索 target 在 nums 中的位置。如果目标值存在返回它的索引，否则返回 -1。

### 示例 1：
输入：nums = [-1, 0, 3, 5, 9, 12], target = 9  
输出：4  
解释：9 在 nums 中的索引是 4。

### 示例 2：
输入：nums = [-1, 0, 3, 5, 9, 12], target = 2  
输出：-1  
解释：2 不在 nums 中，所以返回 -1。

### 提示：
- 你可以假设 nums 中的所有元素都是唯一的。
- n 将在 [1, 10000] 之间。
- 你可以认为 nums 是按升序排列的。

## 解题思路

### 1. 基本思路
- 二分查找是一种高效的查找算法，适用于在有序数组中查找元素。
- 通过不断缩小查找范围，最终找到目标值或确定目标值不存在。

### 2. 具体步骤
1. 初始化两个指针，left 和 right，分别指向数组的开始和结束。
2. 计算中间位置 mid = left + (right - left) / 2。
3. 比较 nums[mid] 和 target：
   - 如果相等，返回 mid。
   - 如果 nums[mid] < target，说明目标值在右半部分，更新 left = mid + 1。
   - 如果 nums[mid] > target，说明目标值在左半部分，更新 right = mid - 1。
4. 重复步骤 2 和 3，直到 left 超过 right。
5. 如果未找到目标值，返回 -1。

### 3. 代码实现
```cpp
class Solution {
public:
    int search(std::vector<int>& nums, int target) {
        int left = 0, right = nums.size() - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2; // 防止溢出

            if (nums[mid] == target) {
                return mid; // 找到目标值
            } else if (nums[mid] < target) {
                left = mid + 1; // 目标值在右半部分
            } else {
                right = mid - 1; // 目标值在左半部分
            }
        }

        return -1; // 未找到目标值
    }
};
```

## 复杂度分析
1. **时间复杂度**：O(log n)，每次查找都将搜索范围减半。
2. **空间复杂度**：O(1)，只使用了常数级别的额外空间。

## 扩展问题

### 1. 寻找旋转排序数组中的最小值
#### 问题描述
给定一个可能存在重复元素的旋转排序数组，寻找其中的最小值。

#### 分析
这样的数组有个重要特点:
1. 最小值左边的所有元素都大于数组最右端的元素
2. 最小值右边的所有元素都小于数组最右端的元素

这就给了我们一个判断的依据 - 通过与最右端元素比较,我们可以知道最小值在哪一侧。
#### 示例
输入：nums = [3, 4, 5, 1, 2]  
输出：1

#### 代码实现
```cpp
class Solution {
public:
    int findMin(std::vector<int>& nums) {
        int left = 0, right = nums.size() - 1;

        while (left < right) {
            int mid = left + (right - left) / 2;

            if (nums[mid] > nums[right]) {
                left = mid + 1; // 最小值在右半部分
            } else if (nums[mid] < nums[right]) {
                right = mid; // 最小值在左半部分或mid
            } else {
                right--; // 当 nums[mid] == nums[right] 时，缩小右边界
                // 无法确定最小值是在右半部分还是在左半部分。通过缩小右边界，我们可以逐步排除重复的元素，最终找到最小值。
                // 如果不缩小右边界，可能会导致在存在大量重复元素的情况下，算法陷入无限循环。例如，假设数组是 [2, 2, 2, 2, 2]，如果我们不缩小右边界，left 和 right 可能永远不会相遇，导致算法无法终止。
            }
        }

        return nums[left]; // 返回最小值
    }
};
```

### 2. 寻找峰值元素
#### 问题描述
峰值元素是指其值大于或等于其相邻元素的元素。给定一个输入数组 nums，找到峰值元素并返回其索引。
#### 分析
这道题乍看不像是二分查找的题目,因为数组并不是有序的。但仔细分析会发现:
1. 如果mid位置的元素小于右边元素,那么右边一定存在峰值
2. 如果mid位置的元素大于右边元素,那么左边一定存在峰值(可能是mid本身)

这个性质保证了我们可以用二分查找来缩小范围。
#### 示例
输入：nums = [1, 2, 3, 1]  
输出：2

#### 代码实现
```cpp
class Solution {
public:
    int findPeakElement(std::vector<int>& nums) {
        int left = 0, right = nums.size() - 1;

        while (left < right) {
            int mid = left + (right - left) / 2;

            if (nums[mid] < nums[mid + 1]) {
                left = mid + 1; // 峰值在右半部分
            } else {
                right = mid; // 峰值在左半部分或mid
            }
        }

        return left; // 返回峰值元素的索引
    }
};
```

## 复杂度分析
- 时间复杂度：O(log n)
- 空间复杂度：O(1)
