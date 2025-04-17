# 盛最多水的容器

## 问题链接
[LeetCode 11. 盛最多水的容器](https://leetcode.com/problems/container-with-most-water/)

## 问题描述
给定一个长度为 n 的整数数组 height。有 n 条垂直线，第 i 条线的两个端点是 (i, 0) 和 (i, height[i])。

找出其中的两条线，使得它们与 x 轴共同构成的容器可以容纳最多的水。

返回容器可以储存的最大水量。

说明：你不能倾斜容器。

## 解题思路
1. 使用双指针方法求解:
   - 初始时左指针指向数组开始，右指针指向数组末尾
   - 计算当前面积 = min(height[left], height[right]) * (right - left)
   - 更新最大面积
   - 移动较短的那个指针（因为移动较长的指针不可能得到更大的面积）

2. 指针移动策略:
   - 如果左边高度小于右边，移动左指针
   - 如果右边高度小于左边，移动右指针
   - 如果相等，可以移动任意一个指针

## 代码实现
```cpp
class Solution {
public:
    int maxArea(vector<int>& height) {
        int maxArea = 0;
        int left = 0;
        int right = height.size() - 1;
        
        while (left < right) {
            // 计算当前面积
            int width = right - left;
            int area = min(height[left], height[right]) * width;
            maxArea = max(maxArea, area);
            
            // 移动较短的线段
            if (height[left] < height[right]) {
                left++;
            } else {
                right--;
            }
        }
        
        return maxArea;
    }
};
```

## 复杂度分析
- 时间复杂度：O(n)，其中 n 是数组的长度。双指针最多遍历整个数组一次。
- 空间复杂度：O(1)，只需要常数级别的额外空间。
