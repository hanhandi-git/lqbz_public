## 问题链接
https://leetcode.cn/problems/3sum/

## 问题描述
给你一个整数数组 nums ，判断是否存在三元组 [nums[i], nums[j], nums[k]] 满足 i != j、i != k 且 j != k ，同时还满足 nums[i] + nums[j] + nums[k] == 0 。请你返回所有和为 0 且不重复的三元组。

注意：答案中不可以包含重复的三元组。

### 示例 1：

输入：nums = [-1,0,1,2,-1,-4]
输出：[[-1,-1,2],[-1,0,1]]
解释：
nums[0] + nums[1] + nums[2] = (-1) + 0 + 1 = 0
nums[1] + nums[2] + nums[4] = 0 + 1 + (-1) = 0
nums[0] + nums[3] + nums[4] = (-1) + 2 + (-1) = 0
不同的三元组是 [-1,0,1] 和 [-1,-1,2]
注意，输出的顺序和三元组的顺序并不重要。

### 示例 2：

输入：nums = [0,1,1]
输出：[]
解释：唯一可能的三元组和不为 0 。

### 示例 3：

输入：nums = [0,0,0]
输出：[[0,0,0]]
解释：唯一可能的三元组和为 0 。

### 提示：

- 3 <= nums.length <= 3000
- -10^5 <= nums[i] <= 10^5

## 解题思路
本题使用排序 + 双指针的方法来解决。主要步骤如下：

1. 首先对数组进行排序，这样可以方便我们去重和使用双指针。
2. 遍历排序后的数组，固定第一个数 nums[i]，然后使用左右指针 left 和 right 分别指向 i+1 和数组末尾。
3. 计算三数之和 sum = nums[i] + nums[left] + nums[right]：
   - 如果 sum == 0，将结果加入答案，并移动左右指针。
   - 如果 sum < 0，说明和太小，左指针右移。
   - 如果 sum > 0，说明和太大，右指针左移。
4. 注意去重：对于第一个数和左右指针都要处理重复元素。

## 代码实现
```cpp
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> result;
        int n = nums.size();
        if (n < 3) return result;
        
        sort(nums.begin(), nums.end());
        
        for (int i = 0; i < n - 2; i++) {
            if (i > 0 && nums[i] == nums[i-1]) continue; // 去重
            
            int left = i + 1, right = n - 1;
            while (left < right) {
                int sum = nums[i] + nums[left] + nums[right];
                if (sum == 0) {
                    result.push_back({nums[i], nums[left], nums[right]});
                    while (left < right && nums[left] == nums[left+1]) left++; // 去重
                    while (left < right && nums[right] == nums[right-1]) right--; // 去重
                    left++;
                    right--;
                } else if (sum < 0) {
                    left++;
                } else {
                    right--;
                }
            }
        }
        
        return result;
    }
};
```

## 复杂度分析
- 时间复杂度：O(n^2)，其中 n 是数组的长度。排序的时间复杂度是 O(nlogn)，双指针遍历的时间复杂度是 O(n^2)。
- 空间复杂度：O(1)，除了存储答案的空间外，我们只需要常数级的额外空间。

## 优化思路
1. 可以在遍历过程中加入一些剪枝条件，例如：
   - 如果 nums[i] > 0，由于数组已排序，后面不可能有三个数和为 0，可以直接退出循环。
   - 如果 nums[i] + nums[i+1] + nums[i+2] > 0，可以直接退出循环。
   - 如果 nums[i] + nums[n-2] + nums[n-1] < 0，可以直接跳过当前的 i。

2. 对于特殊情况的处理：
   - 如果数组长度小于 3，直接返回空结果。
   - 如果所有元素都是正数或者都是负数，直接返回空结果。

3. 使用哈希表：虽然在这个问题中双指针方法已经很高效，但在某些情况下，使用哈希表可能会有更好的性能，特别是当数组中有大量重复元素时。

通过这些优化，我们可以在某些情况下进一步提高算法的效率。
