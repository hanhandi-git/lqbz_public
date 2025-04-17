# 搜索插入位置

## 问题链接
[LeetCode 34. 在排序数组中查找元素的第一个和最后一个位置](https://leetcode.com/problems/find-first-and-last-position-of-element-in-sorted-array/)

## 问题描述
给定一个排序数组 `nums` 和一个目标值 `target`，如果目标值在数组中，则返回其第一个和最后一个位置。如果目标值不在数组中，则返回 `[-1, -1]`。

你可以假设数组中无重复元素。

示例:
输入: `nums = [5,7,7,8,8,10], target = 8`  
输出: `[3, 4]`

输入: `nums = [5,7,7,8,8,10], target = 6`  
输出: `[-1, -1]`

## 解题思路
这道题可以通过二分查找法来高效解决。由于数组是有序的，我们可以利用这一特性来快速找到目标值的第一个和最后一个位置。

### 二分查找法
1. 初始化两个指针 `left` 和 `right`，分别指向数组的开始和结束位置。
2. 进入循环，直到 `left` 超过 `right`：
   - 计算中间位置 `mid = left + (right - left) / 2`。
   - 如果 `nums[mid]` 等于 `target`，则需要进一步查找：
     - 先查找第一个位置：将 `right` 移动到 `mid - 1`。
     - 记录当前 `mid` 作为第一个位置。
   - 如果 `nums[mid]` 小于 `target`，则将 `left` 移动到 `mid + 1`。
   - 如果 `nums[mid]` 大于 `target`，则将 `right` 移动到 `mid - 1`。
3. 如果找到第一个位置后，再次进行二分查找以找到最后一个位置。

## 代码实现
```cpp
class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        vector<int> result = {-1, -1};
        int left = 0, right = nums.size() - 1;

        // 查找第一个位置
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] == target) {
                result[0] = mid; // 记录第一个位置
                right = mid - 1; // 继续查找左侧
            } else if (nums[mid] < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }

        left = 0, right = nums.size() - 1;

        // 查找最后一个位置
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] == target) {
                result[1] = mid; // 记录最后一个位置
                left = mid + 1; // 继续查找右侧
            } else if (nums[mid] < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }

        return result; // 返回第一个和最后一个位置
    }
};
```

## 复杂度分析
- 时间复杂度：O(log n)
  - 由于使用了二分查找，每次都将搜索范围减半。
- 空间复杂度：O(1)
  - 只使用了常数额外空间。

## 关键点
1. 利用二分查找的思想来高效查找目标值的第一个和最后一个位置。
2. 注意处理边界条件，确保 `left` 和 `right` 的更新逻辑正确。
3. 理解返回值的含义，返回的数组中第一个元素为目标值的第一个位置，第二个元素为目标值的最后一个位置。