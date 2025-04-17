# 二叉树的锯齿形层序遍历

## 问题链接
[LeetCode 103. 二叉树的锯齿形层序遍历](https://leetcode.com/problems/binary-tree-zigzag-level-order-traversal/)

## 问题描述
给你二叉树的根节点 root ，返回其节点值的 锯齿形层序遍历 。（即先从左往右，再从右往左进行下一层遍历，以此类推，层与层之间交替进行）。

## 解题思路
1. BFS（广度优先搜索）+ 层次标记
   - 基本思路与普通层序遍历相同
   - 使用一个布尔变量记录当前层的遍历方向
   - 偶数层从左到右，奇数层从右到左
   - 可以在每层结果生成后再根据层次决定是否反转

2. BFS + 双端队列
   - 使用双端队列（deque）处理每一层
   - 根据层次决定从队列头部还是尾部插入元素
   - 避免了反转操作，效率更高

## 代码实现
```cpp
class Solution {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        vector<vector<int>> result;
        if (!root) return result;
        
        queue<TreeNode*> q;
        q.push(root);
        bool leftToRight = true;  // 控制遍历方向
        
        while (!q.empty()) {
            int levelSize = q.size();
            vector<int> currentLevel(levelSize);  // 预分配大小
            
            for (int i = 0; i < levelSize; i++) {
                TreeNode* node = q.front();
                q.pop();
                
                // 根据方向决定位置
                int index = leftToRight ? i : (levelSize - 1 - i);
                currentLevel[index] = node->val;
                
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
            
            result.push_back(currentLevel);
            leftToRight = !leftToRight;  // 切换方向
        }
        
        return result;
    }
};
```

```cpp
// 使用双端队列的实现
class Solution {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        vector<vector<int>> result;
        if (!root) return result;
        
        queue<TreeNode*> q;
        q.push(root);
        bool leftToRight = true;
        
        while (!q.empty()) {
            int levelSize = q.size();
            deque<int> currentLevel;
            
            for (int i = 0; i < levelSize; i++) {
                TreeNode* node = q.front();
                q.pop();
                
                if (leftToRight) {
                    currentLevel.push_back(node->val);
                } else {
                    currentLevel.push_front(node->val);
                }
                
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
            
            result.push_back(vector<int>(currentLevel.begin(), currentLevel.end()));
            leftToRight = !leftToRight;
        }
        
        return result;
    }
};
```

## 复杂度分析
- 时间复杂度：O(n)，其中n是二叉树的节点数
  - 第一种方法：每个节点被访问一次，vector预分配避免了反转操作
  - 第二种方法：使用双端队列，插入操作为O(1)
- 空间复杂度：O(w)，其中w是二叉树的最大宽度
  - 队列中最多存储一层的节点
  - 结果数组不计入空间复杂度

## 解题技巧
1. 预分配vector大小可以避免频繁的内存分配
2. 使用双端队列可以避免反转操作
3. 使用布尔变量控制方向比用层数判断奇偶更直观
4. 可以在遍历时就确定元素最终位置，避免后续处理

## 相关问题
1. 二叉树的层序遍历 (LC 102)
2. 二叉树的层序遍历 II (LC 107)
3. 二叉树的右视图 (LC 199)

这些问题都是层序遍历的变体，掌握本题的解法对解决相关问题很有帮助。关键是理解如何根据具体需求修改基本的层序遍历算法。