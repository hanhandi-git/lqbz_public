## 问题链接
https://leetcode.cn/problems/pascals-triangle/

## 问题描述
给定一个非负整数 numRows，生成杨辉三角的前 numRows 行。

在杨辉三角中，每个数字是它上方两个数字的和。具体来说，杨辉三角的第 n 行由以下公式生成：
- C(n, k) = C(n-1, k-1) + C(n-1, k)，其中 C(n, k) 表示第 n 行第 k 列的值。

### 示例 1：
输入：numRows = 5
输出：
```
[
     [1],
    [1, 1],
   [1, 2, 1],
  [1, 3, 3, 1],
 [1, 4, 6, 4, 1]
]
```

### 示例 2：
输入：numRows = 1
输出：
```
[
 [1]
]
```

### 提示：
- 1 <= numRows <= 30

## 解题思路

### 方法一：动态规划
为了生成杨辉三角，我们可以使用动态规划的方法。具体步骤如下：

1. 创建一个二维数组 `triangle`，其中 `triangle[i]` 表示第 i 行的元素。
2. 对于每一行，初始化第一个和最后一个元素为 1。
3. 对于中间的元素，使用公式 `triangle[i][j] = triangle[i-1][j-1] + triangle[i-1][j]` 进行计算。
4. 返回生成的杨辉三角。

### 代码实现

```cpp
class Solution {
public:
    vector<vector<int>> generate(int numRows) {
        vector<vector<int>> triangle(numRows);
        for (int i = 0; i < numRows; i++) {
            triangle[i].resize(i + 1);  // 每行的大小为 i + 1
            triangle[i][0] = triangle[i][i] = 1;  // 每行的首尾元素为 1
            for (int j = 1; j < i; j++) {
                triangle[i][j] = triangle[i - 1][j - 1] + triangle[i - 1][j];  // 计算中间元素
            }
        }
        return triangle;
    }
};
```

## 复杂度分析

### 时间复杂度
- O(numRows^2)：生成杨辉三角需要遍历每一行和每一列。

### 空间复杂度
- O(numRows^2)：存储杨辉三角的二维数组。

## 优化技巧

1. 直接使用一维数组来存储当前行的值，减少空间复杂度。
2. 只需在每一行的末尾向前更新值，避免使用额外的二维数组。

## 相关题目
- 119. 杨辉三角 II
- 118. 杨辉三角
- 56. 合并区间

这些题目都涉及到组合数学和动态规划的特性，值得一起学习和比较。