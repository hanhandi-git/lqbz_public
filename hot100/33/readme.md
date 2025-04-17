# 搜索插入位置

## 问题链接
[LeetCode 33. 搜索旋转排序数组](https://leetcode.com/problems/search-in-rotated-sorted-array/)

## 问题描述
给定一个可能存在重复元素的旋转排序数组 `nums` 和一个目标值 `target`，如果目标值在数组中，则返回其索引。如果目标值不在数组中，则返回 `-1`。

示例:
输入: `nums = [4,5,6,7,0,1,2], target = 0`  
输出: `4`

输入: `nums = [4,5,6,7,0,1,2], target = 3`  
输出: `-1`

## 解题思路
这道题可以通过二分查找法来高效解决。由于数组是旋转排序的，我们可以利用这一特性来快速找到目标值。

### 二分查找法
1. 初始化两个指针 `left` 和 `right`，分别指向数组的开始和结束位置。
2. 进入循环，直到 `left` 超过 `right`：
   - 计算中间位置 `mid = left + (right - left) / 2`。
   - 判断 `nums[mid]` 所在的区间：
     - 如果 `nums[left] <= nums[mid]`，说明左半部分是有序的。
       - 如果 `nums[left] <= target < nums[mid]`，则目标值在左半部分，更新 `right = mid - 1`。
       - 否则，目标值在右半部分，更新 `left = mid + 1`。
     - 如果 `nums[mid] < nums[left]`，说明右半部分是有序的。
       - 如果 `nums[mid] < target <= nums[right]`，则目标值在右半部分，更新 `left = mid + 1`。
       - 否则，目标值在左半部分，更新 `right = mid - 1`。
3. 如果找到目标值，返回其索引；否则返回 `-1`。


## 示例演示

为了更好地理解上述二分查找法的实现，下面是一个具体的示例演示。

### 示例 1
输入: `nums = [4, 5, 6, 7, 0, 1, 2], target = 0`  
输出: `4`

#### 过程
1. 初始化 `left = 0`, `right = 6`（数组长度为 7）。
2. 计算 `mid = 0 + (6 - 0) / 2 = 3`，`nums[mid] = 7`。
   - 由于 `nums[left] (4) <= nums[mid] (7)`，左半部分有序。
   - 目标值 `0` 不在左半部分，更新 `left = mid + 1 = 4`。
3. 计算 `mid = 4 + (6 - 4) / 2 = 5`，`nums[mid] = 1`。
   - 由于 `nums[left] (0) <= nums[mid] (1)`，左半部分有序。
   - 目标值 `0` 在左半部分，更新 `right = mid - 1 = 4`。
4. 计算 `mid = 4 + (4 - 4) / 2 = 4`，`nums[mid] = 0`。
   - 找到目标值，返回索引 `4`。

### 示例 2
输入: `nums = [4, 5, 6, 7, 0, 1, 2], target = 3`  
输出: `-1`

#### 过程
1. 初始化 `left = 0`, `right = 6`。
2. 计算 `mid = 0 + (6 - 0) / 2 = 3`，`nums[mid] = 7`。
   - 由于 `nums[left] (4) <= nums[mid] (7)`，左半部分有序。
   - 目标值 `3` 不在左半部分，更新 `left = mid + 1 = 4`。
3. 计算 `mid = 4 + (6 - 4) / 2 = 5`，`nums[mid] = 1`。
   - 由于 `nums[left] (0) <= nums[mid] (1)`，左半部分有序。
   - 目标值 `3` 不在左半部分，更新 `left = mid + 1 = 6`。
4. 计算 `mid = 6 + (6 - 6) / 2 = 6`，`nums[mid] = 2`。
   - 由于 `nums[left] (1) <= nums[mid] (2)`，左半部分有序。
   - 目标值 `3` 不在左半部分，更新 `left = mid + 1 = 7`。
5. 此时 `left > right`，返回 `-1`，表示未找到目标值。

## 代码实现
```cpp
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int left = 0, right = nums.size() - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] == target) {
                return mid; // 找到目标值，返回索引
            }

            // 判断左半部分是否有序
            if (nums[left] <= nums[mid]) {
                if (nums[left] <= target && target < nums[mid]) {
                    right = mid - 1; // 目标值在左半部分
                } else {
                    left = mid + 1; // 目标值在右半部分
                }
            } else { // 右半部分有序
                if (nums[mid] < target && target <= nums[right]) {
                    left = mid + 1; // 目标值在右半部分
                } else {
                    right = mid - 1; // 目标值在左半部分
                }
            }
        }

        return -1; // 未找到目标值
    }
};
```

## 复杂度分析
- 时间复杂度：O(log n)
  - 由于使用了二分查找，每次都将搜索范围减半。
- 空间复杂度：O(1)
  - 只使用了常数额外空间。

## 关键点
1. 利用二分查找的思想来高效查找目标值在旋转排序数组中的位置。
2. 注意判断数组的有序性，以决定搜索的方向。
3. 理解返回值的含义，返回目标值的索引或 `-1` 表示未找到。
