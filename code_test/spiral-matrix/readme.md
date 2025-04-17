## 问题链接
https://leetcode.cn/problems/spiral-matrix/

## 问题描述
给你一个 m x n 的矩阵 matrix ，请按照顺时针螺旋顺序返回矩阵中的所有元素。

### 示例 1：

输入：
```
[
 [ 1, 2, 3 ],
 [ 4, 5, 6 ],
 [ 7, 8, 9 ]
]
```
输出：[1, 2, 3, 6, 9, 8, 7, 4, 5]

### 示例 2：

输入：
```
[
 [1, 2, 3, 4],
 [5, 6, 7, 8],
 [9, 10, 11, 12]
]
```
输出：[1, 2, 3, 4, 8, 12, 11, 10, 9, 5, 6, 7]

### 提示：

- m == matrix.length
- n == matrix[i].length
- 1 <= m, n <= 10^4
- -10^5 <= matrix[i][j] <= 10^5

## 解题思路
本题可以通过定义四个边界（上、下、左、右）来控制遍历的范围，逐步收缩边界，直到遍历完所有元素。主要步骤如下：

1. 初始化四个边界：上边界 `top`、下边界 `bottom`、左边界 `left`、右边界 `right`。
2. 使用一个循环，按照顺时针方向遍历矩阵：
   - 从左到右遍历上边界，遍历完后上边界下移。
   - 从上到下遍历右边界，遍历完后右边界左移。
   - 从右到左遍历下边界（如果下边界仍然有效），遍历完后下边界上移。
   - 从下到上遍历左边界（如果左边界仍然有效），遍历完后左边界右移。
3. 重复以上步骤，直到所有元素都被遍历。

## 代码实现
```cpp
class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        vector<int> result;
        if (matrix.empty()) return result;

        int top = 0, bottom = matrix.size() - 1;
        int left = 0, right = matrix[0].size() - 1;

        while (top <= bottom && left <= right) {
            // 从左到右遍历上边界
            for (int i = left; i <= right; i++) {
                result.push_back(matrix[top][i]);
            }
            top++;

            // 从上到下遍历右边界
            for (int i = top; i <= bottom; i++) {
                result.push_back(matrix[i][right]);
            }
            right--;

            // 从右到左遍历下边界
            if (top <= bottom) {
                for (int i = right; i >= left; i--) {
                    result.push_back(matrix[bottom][i]);
                }
                bottom--;
            }

            // 从下到上遍历左边界
            if (left <= right) {
                for (int i = bottom; i >= top; i--) {
                    result.push_back(matrix[i][left]);
                }
                left++;
            }
        }

        return result;
    }
};
```

## 复杂度分析
- 时间复杂度：O(m * n)，其中 m 是矩阵的行数，n 是矩阵的列数。每个元素都被访问一次。
- 空间复杂度：O(1)，除了存储结果的空间外，我们只需要常数级的额外空间。

## 优化思路
1. 对于特殊情况的处理：
   - 如果矩阵为空，直接返回空结果。
   - 如果矩阵只有一行或一列，直接返回该行或列的元素。

2. 可以考虑使用更高效的方式来处理大矩阵，例如在遍历过程中直接修改原矩阵的值，避免额外的空间开销。

通过这些优化，我们可以在某些情况下进一步提高算法的效率。

## 扩展问题及解题思路

1. **逆时针遍历矩阵**
   问题：按照逆时针方向遍历矩阵中的所有元素。
   
   解题思路：可以通过调整遍历顺序来实现，具体步骤与顺时针遍历类似，只需改变遍历的方向。
通过这些扩展问题展示了如何将螺旋遍历的思路应用到更广泛的场景中。通过理解和实现这些变体，可以加深对矩阵操作和遍历技巧的理解，提高解决复杂问题的能力。
