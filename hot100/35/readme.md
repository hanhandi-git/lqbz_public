# 搜索插入位置

## 问题链接
[LeetCode 35. 搜索插入位置](https://leetcode.com/problems/search-insert-position/)

## 问题描述
给定一个排序数组 `nums` 和一个目标值 `target`，如果目标值在数组中，则返回其索引。否则，返回它将会被按顺序插入的位置。

你可以假设数组中无重复元素。

示例:
输入: `nums = [1,3,5,6], target = 5`  
输出: `2`

输入: `nums = [1,3,5,6], target = 2`  
输出: `1`

输入: `nums = [1,3,5,6], target = 7`  
输出: `4`

输入: `nums = [1,3,5,6], target = 0`  
输出: `0`

## 解题思路
这道题可以通过二分查找法来高效解决。由于数组是有序的，我们可以利用这一特性来快速找到目标值的位置。

### 二分查找法
1. 初始化两个指针 `left` 和 `right`，分别指向数组的开始和结束位置。
2. 进入循环，直到 `left` 超过 `right`：
   - 计算中间位置 `mid = left + (right - left) / 2`。
   - 如果 `nums[mid]` 等于 `target`，则返回 `mid`。
   - 如果 `nums[mid]` 小于 `target`，则将 `left` 移动到 `mid + 1`。
   - 如果 `nums[mid]` 大于 `target`，则将 `right` 移动到 `mid - 1`。
3. 如果循环结束后仍未找到目标值，`left` 就是目标值应该插入的位置。

## 代码实现
```cpp
class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        int left = 0, right = nums.size() - 1;
        
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] == target) {
                return mid;
            } else if (nums[mid] < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        return left; // 返回插入位置
    }
};
```

## 复杂度分析
- 时间复杂度：O(log n)
  - 由于使用了二分查找，每次都将搜索范围减半。
- 空间复杂度：O(1)
  - 只使用了常数额外空间。

## 关键点
1. 利用二分查找的思想来高效查找目标值或插入位置。
2. 注意处理边界条件，确保 `left` 和 `right` 的更新逻辑正确。
3. 理解返回值的含义，`left` 在未找到目标值时即为插入位置。
