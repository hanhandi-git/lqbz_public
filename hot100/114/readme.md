## 题目链接
https://leetcode.cn/problems/flatten-binary-tree-to-linked-list/

## 问题描述
给定一个二叉树，将其展开为一个单链表。展开后的链表应当使用树的先序遍历顺序，并且链表的每个节点都应当是树的节点。

### 示例 1：

输入：
```
    1
   / \
  2   5
 / \
3   4
```
输出：
```
1
 \
  2
   \
    3
     \
      4
       \
        5
```

### 解题思路
1. **前序遍历**：通过递归函数 `preOrder(TreeNode* u)` 按照根->左->右的顺序遍历二叉树。

2. **使用 prev 指针**：维护一个指向前一个访问节点的指针，用于构建链表。

3. **节点连接**：对于每个访问的节点，将 prev 节点的左子树置为 NULL，右子树指向当前节点，从而构建链表结构。

4. **递归顺序**：先处理当前节点，然后递归处理左子树，最后递归处理右子树，确保前序遍历顺序。

**复杂度分析**：
- 时间复杂度：O(n)，每个节点只访问一次
- 空间复杂度：O(h)，h 为树的高度，主要是递归栈的开销

### 代码实现
```cpp
class Solution {
public:
    TreeNode*prev=NULL;
    void preOrder(TreeNode*u){
        if(!u)return;
        if(prev){
            prev->left=NULL;
            prev->right=u;
        }
        prev=u;
        TreeNode*r=u->right;
        preOrder(u->left);
        preOrder(r);
    }
    void flatten(TreeNode* root) {
        preOrder(root);
    }
};
```

### 复杂度分析
- 时间复杂度：O(n)，其中 n 是树的节点数。每个节点最多被访问一次。
- 空间复杂度：O(h)，其中 h 是树的高度。递归调用栈的空间复杂度。

通过这些步骤，我们可以将二叉树有效地转化为链表。