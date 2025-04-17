# 二叉树的最近公共祖先

## 问题链接
[LeetCode 236. 二叉树的最近公共祖先](https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-tree/)

## 问题描述
给定一个二叉树，找到该树中两个指定节点的最近公共祖先。

百度百科中最近公共祖先的定义为："对于有根树 T 的两个节点 p、q，最近公共祖先表示为一个节点 x，满足 x 是 p、q 的祖先且 x 的深度尽可能大（一个节点也可以是它自己的祖先）。"

## 解题思路
1. 使用递归的方法，从根节点开始搜索
2. 对于当前节点：
   - 如果当前节点是null，返回null
   - 如果当前节点是p或q中的任一个，返回当前节点
   - 递归搜索左子树和右子树
3. 根据左右子树的搜索结果：
   - 如果左右子树都找到了结果，说明当前节点就是最近公共祖先
   - 如果只有一边找到了结果，返回那一边的结果
   - 如果都没找到，返回null

## 代码实现
```cpp
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        // 基本情况：如果root为空或者root是p或q中的一个，直接返回root
        if (root == nullptr || root == p || root == q) {
            return root;
        }
        
        // 递归搜索左右子树
        TreeNode* left = lowestCommonAncestor(root->left, p, q);
        TreeNode* right = lowestCommonAncestor(root->right, p, q);
        
        // 如果左右子树都找到了结果，说明当前节点就是LCA
        if (left && right) {
            return root;
        }
        
        // 如果只有一边找到了结果，返回那一边的结果
        return left ? left : right;
    }
};
```

## 复杂度分析
- 时间复杂度：O(N)，其中 N 是二叉树中的节点数
- 空间复杂度：O(H)，其中 H 是树的高度，最坏情况下为 O(N)
