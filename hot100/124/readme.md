## 问题链接
https://leetcode.cn/problems/binary-tree-maximum-path-sum/

## 问题描述
给定一个非空二叉树，返回其最大路径和。路径被定义为从树中任意节点出发到任意节点的路径，路径中至少包含一个节点。该路径可以不经过根节点。

### 示例 1：
输入：
```
      1
     / \
    2   3
```
输出：6
解释：最大路径和为 2 -> 1 -> 3。

### 示例 2：
输入：
```
      -10
     /    \
    9      20
          /  \
         15   7
```
输出：42
解释：最大路径和为 15 -> 20 -> 7。

### 提示：
- 节点数范围 [0, 3 * 10^4]
- -1000 <= Node.val <= 1000

## 解题思路

### 方法一：深度优先搜索（DFS）
为了找到最大路径和，我们可以使用深度优先搜索（DFS）遍历树的每个节点。具体步骤如下：

1. 定义一个全局变量 `max_sum` 用于存储当前找到的最大路径和。
2. 对于每个节点，计算以该节点为根的最大路径和：
   - 计算左子树的最大路径和 `left_max`。
   - 计算右子树的最大路径和 `right_max`。
   - 计算当前节点的最大路径和为 `node.val + left_max + right_max`。
   - 更新 `max_sum` 为当前节点的最大路径和与 `max_sum` 的较大值。
   - 返回当前节点的最大贡献值，即 `node.val + max(left_max, right_max)`。

### 代码实现

```cpp
class Solution {
public:
    int maxPathSum(TreeNode* root) {
        int max_sum = INT_MIN;  // 初始化为最小值
        maxGain(root, max_sum);
        return max_sum;
    }

    int maxGain(TreeNode* node, int& max_sum) {
        if (!node) return 0;  // 空节点返回0

        // 递归计算左子树和右子树的最大路径和
        int left_max = max(maxGain(node->left, max_sum), 0);  // 只考虑正值
        int right_max = max(maxGain(node->right, max_sum), 0);

        // 当前节点的最大路径和
        int current_max = node->val + left_max + right_max;

        // 更新全局最大路径和
        max_sum = max(max_sum, current_max);

        // 返回当前节点的最大贡献值
        return node->val + max(left_max, right_max);
    }
};
```

## 复杂度分析

### 时间复杂度
- O(n)：每个节点只被访问一次。

### 空间复杂度
- O(h)：递归调用栈的空间复杂度，h 为树的高度。

## 优化技巧

1. 提前处理特殊情况：
   - 如果树为空，直接返回0。
   
2. 使用全局变量存储最大路径和，避免在每次递归中传递。

3. 只考虑正值的路径和，避免负值影响结果。

## 相关题目
- 124. 二叉树中的最大路径和
- 543. 二叉树的直径
- 337. 打家劫舍 III

这些题目都涉及到树的路径和的特性，值得一起学习和比较。