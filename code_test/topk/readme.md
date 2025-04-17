# 数组中的第K个最大元素

## 问题链接
[LeetCode 215. 数组中的第K个最大元素](https://leetcode.com/problems/kth-largest-element-in-an-array/)

## 问题描述
给定整数数组 nums 和整数 k，请返回数组中第 k 个最大的元素。

请注意，你需要找的是数组排序后的第 k 个最大的元素，而不是第 k 个不同的元素。

你必须设计并实现时间复杂度为 O(n) 的算法解决此问题。

## 解题思路
1. 使用快速选择算法（Quick Select）来解决这个问题。
2. 快速选择算法是快速排序的一个变体，可以在平均情况下达到 O(n) 的时间复杂度。
3. 算法的核心思想是通过分区操作，每次将数组分为两部分，然后只在其中一部分继续查找。

## 代码实现
```cpp
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        // 函数寻找的是第nums.size() - k小的元素，
        // 这等价于第k大的元素。这是因为快速选择算法默认是按照升序排列的，所以我们需要做这个转换。
        return quickSelect(nums, 0, nums.size() - 1, nums.size() - k);
    }
    
private:
    int quickSelect(vector<int>& nums, int left, int right, int k) {
        if (left == right) return nums[left];
        // 根据分区后基准元素的位置，我们可以决定是直接返回结果，还是在左半部分或右半部分继续寻找。
        int pivotIndex = partition(nums, left, right);
        
        if (k == pivotIndex) {
            return nums[k];
        } else if (k < pivotIndex) {
            return quickSelect(nums, left, pivotIndex - 1, k);
        } else {
            return quickSelect(nums, pivotIndex + 1, right, k);
        }
    }
    
    int partition(vector<int>& nums, int left, int right) {
        int pivot = nums[right];
        int i = left - 1;
        
        for (int j = left; j < right; j++) {
            if (nums[j] <= pivot) {
                i++;
                swap(nums[i], nums[j]);
            }
        }
        
        swap(nums[i + 1], nums[right]);
        return i + 1;
    }
};
```

## 复杂度分析
- 时间复杂度：平均情况 O(n)，最坏情况 O(n^2)
- 空间复杂度：O(1)，原地操作，只使用常数额外空间

## 扩展：其他相关问题
1. 最小的k个数
2. 数据流中的第K大元素
3. 前K个高频元素
解题思路：
* 使用哈希表统计每个元素的频率
* 使用小顶堆,按照元素频率维护前K个高频元素
    * 遍历hash表
        * 如果堆大小小于k，直接将元素加入堆
        * 如果堆的大小>=k,将当前的元素频率于堆顶元素频率比较
            * 大于top，弹出top，将当前加入
            * 跳过当前元素

这些变体问题都是基于"第K个最大元素"的基本思想，但可能需要使用不同的数据结构（如堆）或算法来解决。
堆的特性使得我们可以在O(logk)的时间内维护k个元素，这在处理大规模数据或数据流时特别有用。同时，这些问题也展示了如何灵活运用大顶堆和小顶堆来解决不同类型的"Top K"问题。