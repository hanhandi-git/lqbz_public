# 打家劫舍 III

## 问题链接
[LeetCode 337. 打家劫舍 III](https://leetcode.com/problems/house-robber-iii/)

## 问题描述
在上次打劫完一条街道之后，小偷又发现了一个新的可行窃的地区。这个地区只有一个入口，我们称之为"根"。 除了"根"之外，每栋房子有且只有一个"父"房子与之相连。一番侦察之后，聪明的小偷意识到"这个地方的所有房屋的排列类似于一棵二叉树"。 如果两个直接相连的房子在同一天晚上被打劫，房屋将自动报警。

计算在不触动警报的情况下，小偷一晚能够盗取的最高金额。

## 解题思路
1. 这是一个关于二叉树的动态规划问题。
2. 对于每个节点，我们有两种选择：抢劫该节点或不抢劫该节点。
3. 如果抢劫当前节点，那么就不能抢劫其左右子节点。
4. 如果不抢劫当前节点，那么可以选择抢劫或不抢劫其左右子节点。
5. 使用递归+记忆化（或动态规划）来解决此问题。

### 详细解析
对于每个节点，我们可以定义两个状态：
- `rob`：选择抢劫当前节点时能获得的最大金额
- `not_rob`：选择不抢劫当前节点时能获得的最大金额

递归关系如下：
- 如果抢劫当前节点，那么不能抢劫其左右子节点：`rob = node->val + left.not_rob + right.not_rob`
- 如果不抢劫当前节点，那么可以选择抢劫或不抢劫其左右子节点（取最大值）：`not_rob = max(left.rob, left.not_rob) + max(right.rob, right.not_rob)`

最终返回 `max(rob, not_rob)`，即在根节点处能获得的最大金额。

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
    struct Result {
        int rob;     // 抢劫当前节点时的最大金额
        int not_rob; // 不抢劫当前节点时的最大金额
    };
    
    Result dfs(TreeNode* node) {
        if (!node) return {0, 0};
        
        Result left = dfs(node->left);
        Result right = dfs(node->right);
        
        // 抢劫当前节点，则不能抢劫子节点
        int rob = node->val + left.not_rob + right.not_rob;
        
        // 不抢劫当前节点，则可以选择抢或不抢子节点
        int not_rob = max(left.rob, left.not_rob) + max(right.rob, right.not_rob);
        
        return {rob, not_rob};
    }
    
    int rob(TreeNode* root) {
        Result result = dfs(root);
        return max(result.rob, result.not_rob);
    }
};
```

## 复杂度分析
- 时间复杂度：O(n)，其中 n 是二叉树中的节点数。我们只需要遍历每个节点一次。
- 空间复杂度：O(h)，其中 h 是二叉树的高度。这是由于递归调用栈的开销。在最坏情况下，二叉树可能是一条链，此时 h = n，空间复杂度为 O(n)。

## 优化思路
我们也可以使用哈希表来记忆化递归结果，避免重复计算。但在这个问题中，由于我们使用的是后序遍历（先处理左右子树，再处理当前节点），每个节点只会被访问一次，因此并不需要额外的记忆化。

通过返回包含两种状态（抢劫和不抢劫）的结构体，我们可以在一次递归中同时计算出这两种情况，使算法更加高效。