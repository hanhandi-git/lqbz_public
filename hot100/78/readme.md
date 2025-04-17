# LeetCode 78 - 子集问题

## 问题描述

给你一个整数数组 `nums` ，数组中的元素 **互不相同** 。返回该数组所有可能的子集（幂集）。

解集 **不能** 包含重复的子集。你可以按 **任意顺序** 返回解集。

### 示例

```
输入：nums = [1,2,3]
输出：[[],[1],[2],[1,2],[3],[1,3],[2,3],[1,2,3]]
```

## 解题思路

本题采用**回溯法**解决，主要思路如下：

1. 对于每个元素，我们都有两种选择：
   - 将其加入当前子集
   - 不将其加入当前子集

2. 使用回溯法的关键点：
   - 使用`start`参数避免重复
   - 每个状态都是一个有效的子集
   - 使用`current`数组记录当前路径

### 复杂度分析

- 时间复杂度：**O(2^n)**，其中n是数组长度
- 空间复杂度：**O(n)**，递归调用栈的深度

### 核心代码

```cpp
void backtrack(const vector<int>& nums, int start, 
              vector<int>& current, 
              vector<vector<int>>& result) {
    result.push_back(current);
    
    for (int i = start; i < nums.size(); i++) {
        current.push_back(nums[i]);
        backtrack(nums, i + 1, current, result);
        current.pop_back();
    }
}
```

## 测试说明

测试用例覆盖以下场景：

- 空集合测试
- 单个元素测试
- 多个元素测试
- 包含重复元素的测试

### 运行测试

```bash
# 编译
g++ -std=c++11 test.cpp -o test

# 运行测试
./test
```

## 优化方案

1. **位运算优化**
   - 可以使用二进制位表示元素的选择状态
   - 时间复杂度不变，但常数时间更优

2. **迭代法**
   - 可以避免递归调用栈的开销
   - 实现更简单，效率可能更高

3. **内存优化**
   - 可以预先分配结果数组大小为2^n
   - 减少动态扩容的开销

## 注意事项

1. 输入数组中的元素互不相同
2. 返回的子集可以是任意顺序
3. 空集也是一个有效的子集
4. 需要C++11或更高版本支持

## 参考资料

- [LeetCode 78题](https://leetcode.com/problems/subsets/)
- [回溯算法介绍](https://en.wikipedia.org/wiki/Backtracking)