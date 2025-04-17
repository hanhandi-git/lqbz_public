# 二叉树的层序遍历

## 问题链接
[LeetCode 102. 二叉树的层序遍历](https://leetcode.com/problems/binary-tree-level-order-traversal/)

## 问题描述
给你二叉树的根节点 root ，返回其节点值的 层序遍历 。 （即逐层地，从左到右访问所有节点）。

## 解题思路
1. 使用队列实现广度优先搜索（BFS）
   - 将根节点加入队列
   - 每次处理当前层的所有节点
   - 对于当前层的每个节点：
     - 将其值加入当前层的结果
     - 将其左右子节点（如果存在）加入队列
   - 重复直到队列为空

2. 使用递归实现深度优先搜索（DFS）
   - 记录每个节点的层级
   - 将节点值添加到对应层级的结果中

## 代码实现
```cpp
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> result;
        if (!root) return result;
        
        queue<TreeNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            int levelSize = q.size();
            vector<int> currentLevel;
            
            for (int i = 0; i < levelSize; i++) {
                TreeNode* node = q.front();
                q.pop();
                
                currentLevel.push_back(node->val);
                
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
            
            result.push_back(currentLevel);
        }
        
        return result;
    }
};
```

```cpp
// DFS实现方式
class Solution {
public:
    void dfs(TreeNode* node, int level, vector<vector<int>>& result) {
        if (!node) return;
        
        // 如果当前层级还没有对应的vector，就创建一个
        if (result.size() == level) {
            result.push_back(vector<int>());
        }
        
        // 将当前节点值加入对应层级的vector
        result[level].push_back(node->val);
        
        // 递归处理左右子树
        dfs(node->left, level + 1, result);
        dfs(node->right, level + 1, result);
    }
    
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> result;
        dfs(root, 0, result);
        return result;
    }
};
```

## 复杂度分析
- BFS实现：
  - 时间复杂度：O(n)，其中n是二叉树的节点数
  - 空间复杂度：O(w)，其中w是二叉树的最大宽度
- DFS实现：
  - 时间复杂度：O(n)，其中n是二叉树的节点数
  - 空间复杂度：O(h)，其中h是二叉树的高度，最坏情况下为O(n)

## 扩展：其他相关问题
1. 二叉树的锯齿形层序遍历（Z字形遍历）
2. 二叉树的右视图
3. 二叉树的层平均值

解题思路：
* 层序遍历的变体
    * 可以通过修改基本的层序遍历来实现
    * 需要注意每层的处理方式可能不同
    * 可能需要额外的数据结构或标记来满足特定要求

这些变体问题都是基于层序遍历的基本思想，但可能需要添加额外的处理逻辑。
掌握层序遍历的基本实现对于解决树的遍历问题特别重要，尤其是在需要按层处理节点时。
