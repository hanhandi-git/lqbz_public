# 翻转二叉树

## 问题链接
[LeetCode 226. 翻转二叉树](https://leetcode.com/problems/invert-binary-tree/)

## 问题描述
给你一棵二叉树的根节点 root,翻转这棵二叉树,并返回其根节点。

翻转二叉树的含义是:对于树中的每个节点,交换其左右子节点。

## 解题思路
这道题有两种主要解法:

### 递归解法
1. 基本思路是对每个节点,交换其左右子节点
2. 然后递归地对左右子树进行相同操作
3. 递归的终止条件是遇到空节点

### 迭代解法
1. 使用队列(BFS)或栈(DFS)来存储待处理的节点
2. 每次取出一个节点,交换其左右子节点
3. 将非空的左右子节点加入队列/栈
4. 直到队列/栈为空

## 代码实现

### 递归实现
```cpp
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        // 基本情况:空节点
        if (root == nullptr) {
            return nullptr;
        }
        
        // 交换左右子节点
        TreeNode* temp = root->left;
        root->left = root->right;
        root->right = temp;
        
        // 递归处理左右子树
        invertTree(root->left);
        invertTree(root->right);
        
        return root;
    }
};
```

### 迭代实现(BFS)
```cpp
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (root == nullptr) {
            return nullptr;
        }
        
        queue<TreeNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            TreeNode* curr = q.front();
            q.pop();
            
            // 交换左右子节点
            TreeNode* temp = curr->left;
            curr->left = curr->right;
            curr->right = temp;
            
            // 将非空的子节点加入队列
            if (curr->left) q.push(curr->left);
            if (curr->right) q.push(curr->right);
        }
        
        return root;
    }
};
```

## 复杂度分析
### 递归解法
- 时间复杂度：O(n),其中 n 是二叉树的节点个数
- 空间复杂度：O(h),其中 h 是树的高度。最坏情况下树呈现链状,h=n;最好情况下树为完全二叉树,h=logn

### 迭代解法
- 时间复杂度：O(n),其中 n 是二叉树的节点个数
- 空间复杂度：O(w),其中 w 是树的最大宽度。最坏情况下树为完全二叉树,最后一层的节点数约为 n/2

## 关键点
1. 理解递归解法的简洁性和优雅性
2. 掌握二叉树的递归和迭代两种遍历方式
3. 注意在交换节点时需要使用临时变量
4. 理解BFS和DFS在此问题中的等价性
5. 考虑空节点的处理