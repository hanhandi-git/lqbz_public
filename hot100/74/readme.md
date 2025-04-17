# 搜索二维矩阵

## 问题链接
[LeetCode 74. 搜索二维矩阵](https://leetcode.com/problems/search-a-2d-matrix/)

## 问题描述
编写一个高效算法来搜索一个二维矩阵中的目标值。该矩阵具有以下特性：
- 每行的元素从左到右升序排列。
- 每列的元素从上到下升序排列。

如果目标值存在于矩阵中，则返回 `true`，否则返回 `false`。

示例:
输入: 
```
matrix = [
  [1, 3, 5, 7],
  [10, 11, 16, 20],
  [23, 30, 34, 60]
]
target = 3
```
输出: `true`

输入: 
```
matrix = [
  [1, 3, 5, 7],
  [10, 11, 16, 20],
  [23, 30, 34, 60]
]
target = 13
```
输出: `false`

## 解题思路
这道题可以通过二分查找法来高效解决。由于矩阵的行和列都是有序的，我们可以利用这一特性来快速找到目标值。

### 二分查找法
1. 将二维矩阵视为一维数组，计算出矩阵的行数和列数。
2. 初始化两个指针 `left` 和 `right`，分别指向矩阵的开始和结束位置。
3. 进入循环，直到 `left` 超过 `right`：
   - 计算中间位置 `mid = left + (right - left) / 2`。
   - 通过 `mid` 计算出对应的行和列索引：
     - `row = mid / number_of_columns`
     - `col = mid % number_of_columns`
   - 如果 `matrix[row][col]` 等于 `target`，则返回 `true`。
   - 如果 `matrix[row][col]` 小于 `target`，则将 `left` 移动到 `mid + 1`。
   - 如果 `matrix[row][col]` 大于 `target`，则将 `right` 移动到 `mid - 1`。
4. 如果循环结束后仍未找到目标值，返回 `false`。

## 代码实现
```cpp
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        if (matrix.empty() || matrix[0].empty()) return false;

        int rows = matrix.size();
        int cols = matrix[0].size();
        int left = 0, right = rows * cols - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;
            int mid_value = matrix[mid / cols][mid % cols];

            if (mid_value == target) {
                return true;
            } else if (mid_value < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }

        return false; // 未找到目标值
    }
};
```

## 复杂度分析
- 时间复杂度：O(log(m * n))
  - 由于使用了二分查找，每次都将搜索范围减半。
- 空间复杂度：O(1)
  - 只使用了常数额外空间。

## 关键点
1. 将二维矩阵视为一维数组，利用二分查找的思想来高效查找目标值。
2. 注意处理边界条件，确保 `left` 和 `right` 的更新逻辑正确。
3. 理解返回值的含义，未找到目标值时返回 `false`。
