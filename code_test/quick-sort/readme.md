# 快速排序

## 问题链接
[LeetCode 912. 排序数组](https://leetcode.com/problems/sort-an-array/)

## 问题描述
给你一个整数数组 nums，请你将该数组升序排列。

## 解题思路
1. 使用快速排序算法来解决这个问题。
2. 快速排序的基本思想是：
   - 选择一个基准元素（通常选择数组的第一个元素）
   - 将数组分区，使得基准元素左边的所有元素都小于它，右边的所有元素都大于它
   - 递归地对左右两个子数组进行快速排序
3. 分区过程是快速排序的核心，它通过一次遍历就能将数组分成两部分。
  算法思想：
    1. 选择基准(pivot),这里选择最右边的元素作为基准
    2.
      i始终指向小于等于基准区域的最后一个元素 
      遍历数组
      对于每个元素nums[j],如果它小于等于pivot，就将它swap到小于等于基准区域的下一个位子i+1
      i++
      i 左侧的所有元素都小于或等于基准
      遍历结束后，i+1 就是基准元素的正确位置
      将基准元素（最初在 right 位置）与 nums[i+1] 交换
    3. 返回 i+1，这是基准元素的最终位置
这个分区过程是快速排序效率高的关键。它只需要一次遍历就能将数组分成两部分，每部分可以独立地进行进一步的排序。
## 代码实现
```cpp
class Solution {
public:
    vector<int> sortArray(vector<int>& nums) {
        quickSort(nums, 0, nums.size() - 1);
        return nums;
    }
    
private:
    void quickSort(vector<int>& nums, int left, int right) {
        if (left < right) {
            int pivotIndex = partition(nums, left, right);
            quickSort(nums, left, pivotIndex - 1);
            quickSort(nums, pivotIndex + 1, right);
        }
    }
    
    int partition(vector<int>& nums, int left, int right) {
        int pivot = nums[right];  // 选择最右边的元素作为基准
        int i = left - 1;         // i 是小于等于基准的元素的最后位置
        
        for (int j = left; j < right; j++) {
            if (nums[j] <= pivot) {
                i++;
                swap(nums[i], nums[j]); // 将小于等于基准的元素交换到前面
            }
        }
        
        swap(nums[i + 1], nums[right]); // 将基准元素放到正确的位置
        return i + 1;               // 返回基准元素的最终位置
    }
};
```

## 复杂度分析
- 时间复杂度：平均情况下为O(n log n)，最坏情况下为O(n^2)，其中n是数组的长度。
- 空间复杂度：O(log n)，递归调用栈的深度。

## 扩展：其他相关问题

### 1. 第K个最大元素

问题描述：在未排序的数组中找到第 k 个最大的元素。

例子：
```
输入: [3,2,1,5,6,4] 和 k = 2
输出: 5
```

解题思路：
* 可以使用快速选择算法，这是快速排序的一个变体。
* 每次分区后，我们只需要递归地在一边继续查找，而不是两边都排序。

### 2. 颜色分类

问题描述：给定一个包含红色、白色和蓝色，一共 n 个元素的数组，原地对它们进行排序，使得相同颜色的元素相邻，并按照红色、白色、蓝色顺序排列。

例子：
```
输入：nums = [2,0,2,1,1,0]
输出：[0,0,1,1,2,2]
```

解题思路：
* 这个问题也被称为"荷兰国旗问题"。
* 可以使用三路快速排序的思想，将数组分成三个区域。

### 3. 数组中的第K个最大元素

问题描述：给定整数数组 nums 和整数 k，请返回数组中第 k 个最大的元素。

例子：
```
输入: [3,2,3,1,2,4,5,5,6] 和 k = 4
输出: 4
```

解题思路：
* 可以使用快速选择算法，这是快速排序的一个变体。
* 每次分区后，我们可以确定pivot的最终位置，然后决定是继续在左边还是右边寻找。

这些变体问题都是基于快速排序的基本思想，但可能需要使用不同的技巧或算法来解决。它们展示了如何将快速排序的思想应用到各种排序和选择问题中。
quickSort和quickSelect的主要区别
1、目的不同：
quickSort（快速排序）的目的是对整个数组进行排序。
quickSelect（快速选择）的目的是找到数组中第k小（或第k大）的元素。
2、处理范围不同：
quickSort需要处理整个数组，直到所有元素都排好序
quickSelect只需要处理包含目标元素的那部分数组，不需要对整个数组排序。
3、递归方式不同：
quickSort在partition后，会对pivot左右两侧的子数组都进行递归
quickSelect在partition后，只会对包含目标元素k的那一侧进行递归。
4、时间复杂度不同：
quickSort的平均时间复杂度是O(nlogn)。
quickSelect的平均时间复杂度是O(n)。
