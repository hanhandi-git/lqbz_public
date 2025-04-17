# 移动零

## 问题链接
https://leetcode.cn/problems/move-zeroes/description/

## 问题描述
给定一个数组 `nums`，编写一个函数将所有 `0` 移动到数组的末尾，同时保持非零元素的相对顺序。

请注意，必须在不复制数组的情况下原地对数组进行操作。

### 示例 1：
```
输入: nums = [0,1,0,3,12]
输出: [1,3,12,0,0]
```

### 示例 2：
```
输入: nums = [0]
输出: [0]
```

### 提示：
- 1 <= nums.length <= 10^4
- -2^31 <= nums[i] <= 2^31 - 1

## 解题思路

### 解法1：双指针法
这是最优的解法,使用两个指针:
1. 慢指针(slow)：指向当前应该存放非零元素的位置
2. 快指针(fast)：遍历整个数组

算法步骤:
1. 初始化慢指针 slow = 0
2. 快指针 fast 遍历数组
3. 当遇到非零元素时:
   - 将该元素移动到 slow 指针位置
   - slow 指针向前移动一位
4. 最后将 slow 之后的所有元素置为 0

时间复杂度: O(n) - 只需要遍历一次数组
空间复杂度: O(1) - 只使用了常数额外空间

```cpp
class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int slow = 0;
        // 将非零元素移动到数组前面
        for(int fast = 0; fast < nums.size(); fast++) {
            if(nums[fast] != 0) {
                nums[slow++] = nums[fast];
            }
        }
        // 将剩余位置填充0
        while(slow < nums.size()) {
            nums[slow++] = 0;
        }
    }
};
```

### 解法2：计数法
这种方法需要两次遍历:
1. 第一次遍历统计0的个数
2. 第二次遍历移动非零元素

虽然也能解决问题,但效率不如双指针法。

```cpp
class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int zeroCount = 0;
        // 统计0的个数
        for(int num : nums) {
            if(num == 0) zeroCount++;
        }
        
        // 移动非零元素
        int writePos = 0;
        for(int num : nums) {
            if(num != 0) {
                nums[writePos++] = num;
            }
        }
        
        // 填充0
        while(zeroCount--) {
            nums[writePos++] = 0;
        }
    }
};
```

## 总结
1. 双指针是处理数组元素移动的常用技巧
2. 在这道题中,双指针法既保证了元素的相对顺序,又实现了原地操作
3. 时间复杂度和空间复杂度都达到了最优
