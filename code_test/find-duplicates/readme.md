## 问题链接
[Find All Duplicates in an Array](https://leetcode.com/problems/find-all-duplicates-in-an-array/)

## 问题描述
给定一个整数数组 nums，其中 1 ≤ a[i] ≤ n （n 为数组长度），有些元素出现两次，其他元素出现一次。找出所有出现两次的元素。

你必须在不使用额外空间且时间复杂度为 O(n) 的情况下完成这个任务。

### 示例 1:
输入: [4,3,2,7,8,2,3,1]
输出: [2,3]

### 示例 2:
输入: [1,1,2]
输出: [1]

### 提示：
- n == nums.length
- 1 <= n <= 10^5
- 1 <= nums[i] <= n
- nums 中的每个元素出现 一次 或 两次

## 解题思路
我们可以利用数组元素的值在 1 到 n 之间这个特性来解决问题。基本思路如下：

1. 遍历数组，对于每个元素 num：
   - 计算 index = abs(num) - 1
   - 如果 nums[index] 为正，将其变为负数
   - 如果 nums[index] 已经为负，说明 num 是重复元素，将其添加到结果列表中

2. 返回结果列表

这种方法巧妙地利用了数组本身来标记元素是否出现过，不需要额外的空间。

## 代码实现
```cpp
std::vector<int> findDuplicates(std::vector<int>& nums) {
    std::vector<int> result;
    for (int num : nums) {
        int index = std::abs(num) - 1;
        if (nums[index] > 0) {
            nums[index] = -nums[index];
        } else {
            result.push_back(std::abs(num));
        }
    }
    return result;
}
```
这里可以添加具体的代码实现，可以是 C++、Python 或其他语言的版本。

## 复杂度分析

。


问题的关键