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

## 扩展：相关问题
1. 二叉搜索树的最近公共祖先
2. 带有父指针的二叉树的最近公共祖先
3. N叉树的最近公共祖先

## 扩展问题详解

### 1. 二叉搜索树的最近公共祖先
#### 问题链接
[LeetCode 235. 二叉搜索树的最近公共祖先](https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-search-tree/)

#### 解题思路
- 利用BST的性质：左子树所有节点值小于根节点，右子树所有节点值大于根节点
- 如果p、q都小于当前节点，则LCA在左子树
- 如果p、q都大于当前节点，则LCA在右子树
- 否则当前节点就是LCA

```cpp
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if (root == nullptr) return nullptr;
        
        // 如果p、q都在左子树
        if (p->val < root->val && q->val < root->val) {
            return lowestCommonAncestor(root->left, p, q);
        }
        
        // 如果p、q都在右子树
        if (p->val > root->val && q->val > root->val) {
            return lowestCommonAncestor(root->right, p, q);
        }
        
        // 如果p、q分别在左右子树，或者其中一个就是root
        return root;
    }
};
```

### 2. 带有父指针的二叉树的最近公共祖先
#### 解题思路
- 可以转化为找两个链表的第一个公共节点
- 从p和q分别向上遍历到根节点，记录路径
- 从后向前比较两个路径，找到最后一个相同的节点

```cpp
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* p, TreeNode* q) {
        unordered_set<TreeNode*> ancestors;
        
        // 记录p的所有祖先节点
        while (p != nullptr) {
            ancestors.insert(p);
            p = p->parent;
        }
        
        // 检查q的祖先节点是否在p的祖先集合中
        while (q != nullptr) {
            if (ancestors.count(q)) {
                return q;
            }
            q = q->parent;
        }
        
        return nullptr;
    }
};
```

### 3. N叉树的最近公共祖先
#### 问题链接
[LeetCode 1644. 二叉树的最近公共祖先 II](https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-tree-ii/)

#### 解题思路
- 基本思路与二叉树类似，但需要遍历所有子节点
- 需要额外处理p或q不在树中的情况

```cpp
class Solution {
public:
    Node* lowestCommonAncestor(Node* root, Node* p, Node* q) {
        if (root == nullptr || root == p || root == q) {
            return root;
        }
        
        Node* found = nullptr;
        int count = 0;
        
        // 遍历所有子节点
        for (Node* child : root->children) {
            Node* result = lowestCommonAncestor(child, p, q);
            if (result != nullptr) {
                count++;
                found = result;
            }
        }
        
        // 如果找到两个节点，返回当前root
        if (count == 2) return root;
        // 如果只找到一个节点，返回那个节点
        if (count == 1) return found;
        // 如果都没找到，返回null
        return nullptr;
    }
};
```

### 总结
最近公共祖先问题是树结构中的经典问题，主要有以下几个要点：

1. 基本的递归解法：自底向上查找目标节点，根据查找结果判断LCA位置
2. 对于特殊的树结构（如BST），可以利用其特性优化解法
3. 不同变体（如带父指针、N叉树）需要相应调整解法，但核心思想类似
4. 需要注意边界情况的处理，如节点不存在于树中的情况

理解这些变体有助于加深对树结构的理解，同时也展示了如何根据具体问题特点选择合适的解决方案。
