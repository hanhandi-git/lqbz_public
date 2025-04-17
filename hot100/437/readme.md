# 路径总和 III
请你修改test.cpp中的算法实现以及测试样子，用于测试readme.md中的算法题

## 问题链接
[LeetCode 437. 路径总和 III](https://leetcode.com/problems/path-sum-iii/)

## 问题描述
给定一个二叉树和一个目标和，找出该树中节点值之和等于目标和的路径的数量。路径不需要从根节点开始，也不需要到叶子节点结束，但路径方向必须向下（即只能从父节点到子节点）。

## 解题思路
1. 使用深度优先搜索（DFS）遍历树的每个节点。
2. 在每个节点上，检查以该节点为起点的路径和是否等于目标和。
3. 使用哈希表记录从根节点到当前节点的路径和的出现次数，以便快速查找。

### 算法流程
1. 定义一个递归函数，接受当前节点、当前路径和以及目标和作为参数。
2. 在递归函数中，更新当前路径和，并检查当前路径和是否等于目标和。
3. 递归遍历左子树和右子树，同时更新路径和。
4. 在返回时，更新哈希表，确保路径和的计数正确。

## 代码实现
```cpp
class Solution {
public:
    int pathSum(TreeNode* root, int sum) {
        unordered_map<int, int> prefixSum;
        prefixSum[0] = 1; // 处理从根节点到当前节点的路径和
        return dfs(root, sum, 0, prefixSum);
    }

    int dfs(TreeNode* node, int target, int currentSum, unordered_map<int, int>& prefixSum) {
        if (!node) return 0;

        currentSum += node->val;
        int count = prefixSum[currentSum - target]; // 查找当前路径和减去目标和的出现次数

        prefixSum[currentSum]++; // 更新当前路径和的计数

        count += dfs(node->left, target, currentSum, prefixSum); // 遍历左子树
        count += dfs(node->right, target, currentSum, prefixSum); // 遍历右子树

        prefixSum[currentSum]--; // 回溯，减少当前路径和的计数
        return count;
    }
};
```

## 复杂度分析
- 时间复杂度：O(n)，其中n是二叉树的节点数。
- 空间复杂度：O(n)，最坏情况下哈希表存储所有路径和。

## 示例说明
让我们通过一个具体的例子来说明算法的工作过程：

输入：
- 二叉树：
```
      10
     /  \
    5   -3
   / \    \
  3   2   11
 / \
3  -2
   \
    1
```
- sum = 8

过程：
1. 从根节点10开始，当前路径和为10。
2. 继续到左子节点5，当前路径和为15。
3. 继续到左子节点3，当前路径和为18。
4. 继续到左子节点3，当前路径和为21（找到一个路径和为8的路径：5->3->-2->1）。
5. 继续遍历，找到其他路径和为8的路径。

输出：3（路径为10->-3->11，5->3->-2->1，5->3->3）

这个例子展示了如何通过DFS和哈希表来找到所有路径和等于目标和的路径。



### 方法一：使用深度优先搜索（DFS）和哈希表
在 `pathSum` 方法中，使用了一个哈希表来记录从根节点到当前节点的路径和的出现次数。这个方法的核心思路是：
1. 通过 DFS 遍历每个节点。
2. 在每个节点上，更新当前路径和，并检查以该节点为起点的路径和是否等于目标和。
3. 使用哈希表快速查找当前路径和减去目标和的出现次数，从而计算出符合条件的路径数量。
4. 递归遍历左右子树，确保每个节点都被考虑到。

这种方法的时间复杂度为 O(n)，空间复杂度为 O(n)，因为哈希表可能存储所有路径和。

### 方法二：逐节点搜索
在 `pathSum` 方法中，使用了一个递归方法 `pathSumFrom` 来计算以当前节点为起点的路径数。这个方法的思路是：
1. 对于每个节点，检查其值是否等于目标值，如果相等，则找到一条有效路径。
2. 递归调用 `pathSumFrom` 方法，继续向下搜索左子树和右子树，目标值减去当前节点值。
3. 在 `pathSum` 方法中，计算以当前节点为起点的路径数，加上左子树和右子树中所有节点为起点的路径数。

这种方法的时间复杂度为 O(n²)，因为每个节点都可能作为起点被多次计算，导致重复计算。

### 补充第二种解法的思路
第二种解法的思路可以总结为以下几点：
1. **逐节点计算**：对于每个节点，调用 `pathSumFrom` 方法来计算以该节点为起点的路径数。
2. **递归遍历**：在 `pathSum` 方法中，递归遍历左子树和右子树，确保每个节点都被考虑到。
3. **路径和更新**：在 `pathSumFrom` 方法中，更新目标值，检查当前节点值是否等于目标值，并继续向下搜索。
4. **避免重复计算**：虽然这种方法会导致 O(n²) 的时间复杂度，但它的实现相对简单，易于理解。

### 代码更新
为了更好地理解第二种解法，我们可以在代码中添加一些注释，帮助解释每一步的逻辑。以下是更新后的代码片段：

```cpp:hot100/437/test.cpp
class Solution {
public:
    // 计算以当前节点为起点的路径数
    int pathSumFrom(TreeNode* node, long long targetSum) {
        if (node == nullptr) return 0;
        
        int cnt = 0;
        // 如果当前节点值等于目标值，找到一条有效路径
        if (node->val == targetSum) {
            cnt = 1;
        }
        
        // 继续向下搜索，目标值减去当前节点值
        cnt += pathSumFrom(node->left, targetSum - node->val);   // 左子树中的路径数
        cnt += pathSumFrom(node->right, targetSum - node->val);  // 右子树中的路径数
        
        return cnt;
    }

    // 计算整个树中路径和等于目标值的路径数
    int pathSum(TreeNode* root, int targetSum) {
        if (root == nullptr) return 0;
        
        // 路径总数 = 以当前节点为起点的路径数 + 左子树中的路径数 + 右子树中的路径数
        return pathSumFrom(root, targetSum) +      // 以当前节点为起点的路径数
               pathSum(root->left, targetSum) + // 左子树中所有节点为起点的路径数
               pathSum(root->right, targetSum); // 右子树中所有节点为起点的路径数 
    }
};
```

通过这种方式，第二种解法的思路得到了清晰的阐述，同时也保留了原有的逻辑结构。希望这能帮助你更好地理解这两种解法的区别和实现思路！
