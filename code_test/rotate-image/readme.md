# 旋转矩阵

## 问题链接
[LeetCode 48. 旋转图像](https://leetcode.cn/problems/rotate-image/)

## 问题描述
给定一个 n × n 的二维矩阵 matrix 表示一个图像。请你将图像顺时针旋转 90 度。

你必须在 原地 旋转图像，这意味着你需要直接修改输入的二维矩阵。请不要 使用另一个矩阵来旋转图像。

## 解题思路
要在原地旋转矩阵,我们可以通过以下步骤实现:

1. 先沿着主对角线(左上到右下)翻转矩阵
2. 然后将每一行左右翻转

这两步操作的组合效果等同于将矩阵顺时针旋转90度。

## 代码实现
```cpp
class Solution {
public:
    void rotate(vector<vector<int>>& matrix) {
        int n = matrix.size();
        
        // 沿主对角线翻转
        for (int i = 0; i < n; i++) {
            for (int j = i; j < n; j++) {
                swap(matrix[i][j], matrix[j][i]);
            }
        }
        
        // 每行左右翻转
        for (int i = 0; i < n; i++) {
            reverse(matrix[i].begin(), matrix[i].end());
        }
    }
};
```

## 复杂度分析
- 时间复杂度: O(N^2)，其中 N 是矩阵的边长。我们需要遍历矩阵中的每个元素一次。
- 空间复杂度: O(1)，我们是原地旋转矩阵，没有使用额外的空间。

## 扩展：旋转其他角度

如果我们想要实现其他角度的旋转，可以基于90度旋转的方法进行扩展。以下是不同角度旋转的实现方法：

1. 顺时针旋转180度：
   - 可以通过两次90度旋转实现，或者
   - 直接将矩阵上下翻转，然后左右翻转

2. 顺时针旋转270度（逆时针旋转90度）：
   - 先沿着副对角线（右上到左下）翻转矩阵
   - 然后将每一行左右翻转

3. 任意角度旋转：
   - 对于不是90度倍数的旋转，通常需要使用三角函数计算新的坐标位置
   - 这种情况下，很难在原地完成旋转，可能需要额外的空间

以下是实现180度和270度旋转的代码：

```cpp
class Solution {
public:
    // 顺时针旋转180度
    void rotate180(vector<vector<int>>& matrix) {
        int n = matrix.size();
        // 上下翻转
        reverse(matrix.begin(), matrix.end());
        // 每行左右翻转
        for (int i = 0; i < n; i++) {
            reverse(matrix[i].begin(), matrix[i].end());
        }
    }

    // 顺时针旋转270度（逆时针旋转90度）
    void rotate270(vector<vector<int>>& matrix) {
        int n = matrix.size();
        // 沿副对角线翻转
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n - i; j++) {
                swap(matrix[i][j], matrix[n-1-j][n-1-i]);
            }
        }
        // 每行左右翻转
        for (int i = 0; i < n; i++) {
            reverse(matrix[i].begin(), matrix[i].end());
        }
    }
};
```

这些扩展方法的时间复杂度和空间复杂度与原来的90度旋转相同：
- 时间复杂度: O(N^2)
- 空间复杂度: O(1)

对于任意角度的旋转，如果需要精确计算每个像素的新位置，可能需要使用额外的矩阵来存储旋转后的结果，这种情况下的空间复杂度将增加到O(N^2)。
