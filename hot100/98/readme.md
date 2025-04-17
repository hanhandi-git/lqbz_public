## 问题链接
https://leetcode.cn/problems/validate-binary-search-tree/

## 问题描述
给定一个二叉树，判断其是否是一个有效的二叉搜索树（BST）。有效的二叉搜索树定义如下：

- 节点的左子树只包含小于当前节点的数。
- 节点的右子树只包含大于当前节点的数。
- 所有左子树和右子树自身必须也是二叉搜索树。

### 示例 1：

输入：
```
    2
   / \
  1   3
```
输出：true

### 示例 2：

输入：
```
    5
   / \
  1   4
     / \
    3   6
```
输出：false  
解释：输入中的根节点 5 的右子节点 4 小于根节点 5。

### 提示：

- 树中节点的数目范围是 [0, 10^4]。
- -10^4 <= Node.val <= 10^4

## 解题思路
本题可以使用递归的方法来解决。主要步骤如下：

1. 定义一个辅助函数，接收当前节点及其值的范围（最小值和最大值）。
2. 对于每个节点，检查其值是否在范围内。
3. 递归检查左子树和右子树，更新范围。

## 代码实现
```cpp
class Solution {
public:
    bool isValidBST(TreeNode* root) {
        return isValidBSTHelper(root, LONG_MIN, LONG_MAX);
    }
    
    bool isValidBSTHelper(TreeNode* node, long minVal, long maxVal) {
        if (!node) return true; // 空节点是有效的
        
        if (node->val <= minVal || node->val >= maxVal) {
            return false; // 当前节点不在有效范围内
        }
        
        // 递归检查左子树和右子树
        return isValidBSTHelper(node->left, minVal, node->val) &&
               isValidBSTHelper(node->right, node->val, maxVal);
    }
};
```

## 复杂度分析
- 时间复杂度：O(n)，其中 n 是树的节点数。每个节点最多被访问一次。
- 空间复杂度：O(h)，其中 h 是树的高度。递归调用栈的空间复杂度。

## 优化思路
1. 可以使用迭代的方法来避免递归带来的栈空间消耗。
2. 使用中序遍历的方式，检查遍历过程中节点值的顺序是否递增。

通过这些优化，我们可以在某些情况下进一步提高算法的效率。
