# 螺旋矩阵

## 问题链接
https://leetcode.cn/problems/spiral-matrix/

## 问题描述
给你一个 `m x n` 的矩阵 `matrix` ，请按照 **顺时针螺旋顺序** ，返回矩阵中的所有元素。

### 示例 1：
```
输入：matrix = [[1,2,3],[4,5,6],[7,8,9]]
输出：[1,2,3,6,9,8,7,4,5]
```
![示例1](https://assets.leetcode.com/uploads/2020/11/13/spiral1.jpg)

### 示例 2：
```
输入：matrix = [[1,2,3,4],[5,6,7,8],[9,10,11,12]]
输出：[1,2,3,4,8,12,11,10,9,5,6,7]
```
![示例2](https://assets.leetcode.com/uploads/2020/11/13/spiral.jpg)

### 提示：
- `m == matrix.length`
- `n == matrix[i].length`
- `1 <= m, n <= 10`
- `-100 <= matrix[i][j] <= 100`

## 解题思路

这个问题主要有两种常见解法：

### 1. 模拟法（方向遍历）
- 定义四个方向：右、下、左、上
- 按照这个顺序遍历矩阵
- 当走到边界或已访问的元素时，改变方向
- 直到访问完所有元素


## 代码实现

### 1. 模拟法（方向遍历）
```cpp
class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        // 处理空矩阵情况
        if (matrix.empty()) return {};
        
        vector<int> result;
        int rows = matrix.size(), cols = matrix[0].size();
        
        // 定义四个边界
        int left = 0, right = cols - 1;
        int top = 0, bottom = rows - 1;
        
        // 当边界合法时继续遍历
        while (left <= right && top <= bottom) 
        {
            // 1. 向右遍历当前顶部行
            for (int j = left; j <= right; j++) {
                result.push_back(matrix[top][j]);
            }
            top++; // 收缩上边界
            
            // 2. 向下遍历当前最右列
            for (int i = top; i <= bottom; i++) {
                result.push_back(matrix[i][right]);
            }
            right--; // 收缩右边界
            
            // 3. 向左遍历当前底部行(如果还有行未遍历)
            if (top <= bottom) {
                for (int j = right; j >= left; j--) {
                    result.push_back(matrix[bottom][j]);
                }
                bottom--; // 收缩下边界
            }
            
            // 4. 向上遍历当前最左列(如果还有列未遍历)
            if (left <= right) {
                for (int i = bottom; i >= top; i--) {
                    result.push_back(matrix[i][left]);
                }
                left++; // 收缩左边界
            }
        }
        
        return result;
    }
};
```

## 复杂度分析

对于所有解法：
- 时间复杂度：O(m×n)，其中 m 和 n 分别是矩阵的行数和列数。矩阵中的每个元素都被访问一次。
- 空间复杂度：O(1)，不考虑返回的结果数组。方向数组解法会使用 O(m×n) 的空间来存储访问状态。

## 优化思路

1. 边界优化：
   - 可以通过不断收缩边界，避免使用额外的visited数组

2. 代码简化：
   - 使用方向数组可以使代码更加简洁

3. 特殊情况处理：
   - 检查空矩阵的情况
   - 处理单行或单列的特殊情况

## 相关题目
- [59. 螺旋矩阵 II](https://leetcode.cn/problems/spiral-matrix-ii/) - 按螺旋顺序生成矩阵
- [885. 螺旋矩阵 III](https://leetcode.cn/problems/spiral-matrix-iii/) - 螺旋遍历的变种
- [2326. 螺旋矩阵 IV](https://leetcode.cn/problems/spiral-matrix-iv/) - 根据链表创建螺旋矩阵