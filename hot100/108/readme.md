## 题目链接
https://leetcode.cn/problems/convert-sorted-array-to-binary-search-tree/

## 问题描述
给你一个整数数组 `nums` ，其中元素已经按 **升序** 排列，请你将其转换为一棵 **高度平衡** 二叉搜索树。

**高度平衡** 二叉树是一棵满足「每个节点的左右两个子树的高度差的绝对值不超过 1 」的二叉树。

### 示例 1：

输入：
```
nums = [-10,-3,0,5,9]
```
输出：
```
      0
     / \
   -3   9
   /   /
 -10  5
```

### 解题思路
1. **高度平衡二叉搜索树的特点**:
   - 每个节点的左右两个子树的高度差的绝对值不超过 1。
   - 二叉搜索树中，左子树的所有节点值小于根节点值，右子树的所有节点值大于根节点值。
   
2. **有序数组的特点**:
   - 数组已经按升序排列，中间元素可以作为根节点，左半部分作为左子树，右半部分作为右子树。

3. **递归构建树**:
   - 选择数组的中间元素作为根节点，这样可以保证左右子树的节点数量尽可能接近，从而保持树的平衡。
   - 递归地对左半部分和右半部分进行相同的操作。

### 代码实现
```cpp
class Solution {
public:
    TreeNode* sortedArrayToBST(vector<int>& nums) {
        return buildBST(nums, 0, nums.size() - 1);
    }
    
    TreeNode* buildBST(vector<int>& nums, int left, int right) {
        if (left > right) return nullptr;
        
        // 选择中间位置的元素作为根节点
        int mid = left + (right - left) / 2;
        TreeNode* root = new TreeNode(nums[mid]);
        
        // 递归构建左子树和右子树
        root->left = buildBST(nums, left, mid - 1);
        root->right = buildBST(nums, mid + 1, right);
        
        return root;
    }
};
```

这段代码的核心是递归地构建二叉搜索树。让我们分析一下关键部分：

```cpp
int mid = left + (right - left) / 2;
TreeNode* root = new TreeNode(nums[mid]);
```

1. **`mid = left + (right - left) / 2`**: 计算当前区间的中间位置。使用这种方式计算中间位置可以避免整数溢出的问题。
2. **`new TreeNode(nums[mid])`**: 创建一个新的树节点，其值为数组中间位置的元素。

接下来是递归构建左右子树：

```cpp
root->left = buildBST(nums, left, mid - 1);
root->right = buildBST(nums, mid + 1, right);
```

1. **`root->left = buildBST(nums, left, mid - 1)`**: 递归构建左子树，使用数组的左半部分（从left到mid-1）。
2. **`root->right = buildBST(nums, mid + 1, right)`**: 递归构建右子树，使用数组的右半部分（从mid+1到right）。

这种方法确保了构建的二叉搜索树是高度平衡的，因为每次都选择中间元素作为根节点，左右子树的节点数量差最多为1。

### 复杂度分析
- 时间复杂度：O(n)，其中 n 是数组的长度。每个元素最多被访问一次。
- 空间复杂度：O(log n)，递归调用栈的深度为树的高度，而平衡二叉树的高度为 log n。

### 总结
通过选择数组的中间元素作为根节点，并递归地构建左右子树，我们可以将有序数组转换为高度平衡的二叉搜索树。这种方法充分利用了有序数组和二叉搜索树的特性，实现了高效的转换。
