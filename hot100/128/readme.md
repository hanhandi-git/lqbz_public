```thinking
让我思考一下如何写LC128最长连续序列的题解...

首先需要理解题目的关键点:
1. 是找最长连续序列长度
2. 数组是无序的
3. 要求O(n)时间复杂度
4. 数组中可能有重复元素

解题思路的关键在于:
- 如何在O(n)内找到连续序列
- 如何避免重复计算
- 如何处理重复元素

可以用哈希表来优化查找,主要思路:
1. 先把所有数字放入哈希表
2. 对每个数x,看x+1,x+2...是否存在
3. 优化:只从序列起点开始计算(x-1不存在时)

需要写清楚:
1. 问题描述和限制条件
2. 暴力解法和优化思路
3. 哈希表解法的详细步骤
4. 复杂度分析
5. 代码实现
6. 一些优化技巧

让我开始写具体内容...
```

# 以下是修改后的readme.md内容：

## 问题链接
https://leetcode.cn/problems/longest-consecutive-sequence/

## 问题描述
给定一个未排序的整数数组 nums，找出数字连续的最长序列（不要求序列元素在原数组中连续）的长度。

请你设计并实现时间复杂度为 O(n) 的算法解决此问题。

### 示例 1：
输入：nums = [100,4,200,1,3,2]
输出：4
解释：最长数字连续序列是 [1, 2, 3, 4]。它的长度为 4。

### 示例 2：
输入：nums = [0,3,7,2,5,8,4,6,0,1]
输出：9

### 提示：
- 0 <= nums.length <= 105
- -109 <= nums[i] <= 109

## 解题思路

### 方法一：排序（不符合题目要求）
最直观的方法是先将数组排序，然后遍历一遍找出最长连续序列。但这种方法的时间复杂度是 O(nlogn)，不符合题目要求。

### 方法二：哈希表
为了实现 O(n) 的时间复杂度，我们可以使用哈希表，具体步骤如下：

1. 首先将所有数字存入哈希表中，用于O(1)时间的查找

2. 遍历数组中的每个数字x，如果x-1不存在于哈希表中，说明x是一个连续序列的起点：
   - 从x开始，不断查找x+1, x+2, ...是否存在
   - 更新最长连续序列的长度

3. 优化：只有当x是连续序列的第一个数（即x-1不存在）时才开始计数，这样可以避免重复计算

## 代码实现

```cpp
class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        unordered_set<int> num_set;
        for (const int& num : nums) {
            num_set.insert(num);
        }
        
        int longest_streak = 0;
        
        for (const int& num : nums) {
            // 只有当num-1不存在时，才开始计数
            if (!num_set.count(num - 1)) {
                int current_num = num;
                int current_streak = 1;
                
                // 不断查找下一个数是否存在
                while (num_set.count(current_num + 1)) {
                    current_num += 1;
                    current_streak += 1;
                }
                
                longest_streak = max(longest_streak, current_streak);
            }
        }
        
        return longest_streak;
    }
};
```

## 复杂度分析

### 时间复杂度
- O(n)：虽然有两层循环，但内层循环只会在数字是连续序列的第一个数时才会执行
- 每个数字最多只会被访问两次：一次在外层循环，一次在内层循环查找连续序列时

### 空间复杂度
- O(n)：需要哈希表存储所有数字

## 优化技巧

1. 提前处理特殊情况：
   - 如果数组为空，直接返回0
   - 如果数组长度为1，直接返回1

2. 可以在插入哈希表时去除重复元素，减少后续处理

3. 可以使用unordered_set代替unordered_map，因为我们只需要判断数字是否存在

4. 如果数组中数字范围较小且密集，可以考虑使用数组代替哈希表

5. 实现细节优化：
```cpp
// 优化后的版本
class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        if (nums.empty()) return 0;
        if (nums.size() == 1) return 1;
        
        unordered_set<int> num_set(nums.begin(), nums.end());
        int longest_streak = 1;
        
        for (const int& num : num_set) {  // 直接遍历集合避免重复
            if (!num_set.count(num - 1)) {
                int current_streak = 1;
                int current_num = num;
                
                while (num_set.count(current_num + 1)) {
                    current_streak++;
                    current_num++;
                }
                
                longest_streak = max(longest_streak, current_streak);
            }
        }
        
        return longest_streak;
    }
};
```

## 相关题目
- 128. 最长连续序列
- 674. 最长连续递增序列
- 300. 最长递增子序列

这些题目都涉及到寻找序列的特性，但要求和解法各不相同，值得一起学习和比较。
