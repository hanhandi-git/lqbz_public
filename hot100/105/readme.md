## 题目链接
https://leetcode.cn/problems/construct-binary-tree-from-preorder-and-inorder-traversal/

## 问题描述
给定两个整数数组 `preorder` 和 `inorder`，其中 `preorder` 是二叉树的先序遍历，`inorder` 是二叉树的中序遍历。请构造二叉树并返回其根节点。

### 示例 1：

输入：
```
preorder = [3,9,20,15,7]
inorder = [9,3,15,20,7]
```
输出：
```
    3
   / \
  9   20
     /  \
    15   7
```

### 解题思路
1. **先序遍历的特点**:
   - 先序遍历的第一个元素是树的根节点。
   
2. **中序遍历的特点**:
   - 中序遍历中，根节点左侧的元素是左子树的节点，右侧的元素是右子树的节点。

3. **递归构建树**:
   - 通过先序遍历的第一个元素确定根节点，然后在中序遍历中找到根节点的位置，从而分割出左子树和右子树的节点。
   - 递归地对左子树和右子树进行相同的操作。

### 代码实现
```cpp
class Solution {
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        if (preorder.empty() || inorder.empty()) return nullptr;
        return build(preorder, 0, preorder.size() - 1, inorder, 0, inorder.size() - 1);
    }

    TreeNode* build(vector<int>& preorder, int preStart, int preEnd, vector<int>& inorder, int inStart, int inEnd) {
        if (preStart > preEnd || inStart > inEnd) return nullptr;

        TreeNode* root = new TreeNode(preorder[preStart]);
        int inRootIndex = find(inorder.begin() + inStart, inorder.begin() + inEnd + 1, root->val) - inorder.begin();
        int leftTreeSize = inRootIndex - inStart;

        root->left = build(preorder, preStart + 1, preStart + leftTreeSize, inorder, inStart, inRootIndex - 1);
        root->right = build(preorder, preStart + leftTreeSize + 1, preEnd, inorder, inRootIndex + 1, inEnd);

        return root;
    }
};
```

这两行代码是构建二叉树的关键部分，具体来说，它们负责递归地构建当前节点的左子树和右子树。让我们逐行分析一下：

```cpp
root->left = build(preorder, preStart + 1, preStart + leftTreeSize, inorder, inStart, inRootIndex - 1);
```

1. **`root->left`**: 这表示当前节点的左子树。
2. **`build(...)`**: 调用 `build` 函数来构建左子树。
3. **`preorder, preStart + 1, preStart + leftTreeSize`**:
   - `preorder`: 先序遍历数组。
   - `preStart + 1`: 左子树的根节点在先序遍历中的起始索引。由于当前根节点是 `preorder[preStart]`，左子树的根节点是 `preorder[preStart + 1]`。
   - `preStart + leftTreeSize`: 左子树的根节点在先序遍历中的结束索引。`leftTreeSize` 是左子树的节点数，因此左子树的结束索引是 `preStart + leftTreeSize`。
4. **`inorder, inStart, inRootIndex - 1`**:
   - `inorder`: 中序遍历数组。
   - `inStart`: 左子树在中序遍历中的起始索引。
   - `inRootIndex - 1`: 左子树在中序遍历中的结束索引。`inRootIndex` 是当前根节点在中序遍历中的索引，左子树的结束索引是 `inRootIndex - 1`。

这行代码的作用是根据先序和中序遍历的信息递归构建当前节点的左子树。

接下来是右子树的构建：

```cpp
root->right = build(preorder, preStart + leftTreeSize + 1, preEnd, inorder, inRootIndex + 1, inEnd);
```

1. **`root->right`**: 这表示当前节点的右子树。
2. **`build(...)`**: 调用 `build` 函数来构建右子树。
3. **`preorder, preStart + leftTreeSize + 1, preEnd`**:
   - `preorder`: 先序遍历数组。
   - `preStart + leftTreeSize + 1`: 右子树的根节点在先序遍历中的起始索引。左子树的节点已经被处理，因此右子树的根节点是 `preorder[preStart + leftTreeSize + 1]`。
   - `preEnd`: 右子树的根节点在先序遍历中的结束索引，保持为 `preEnd`，因为右子树的节点可能会占用整个剩余部分。
4. **`inorder, inRootIndex + 1, inEnd`**:
   - `inorder`: 中序遍历数组。
   - `inRootIndex + 1`: 右子树在中序遍历中的起始索引。右子树的节点在根节点的右侧，因此起始索引是 `inRootIndex + 1`。
   - `inEnd`: 右子树在中序遍历中的结束索引，保持为 `inEnd`，因为右子树的节点可能会占用整个剩余部分。

这行代码的作用是根据先序和中序遍历的信息递归构建当前节点的右子树。

### 总结
这两行代码通过递归调用 `build` 函数，分别构建当前节点的左子树和右子树，利用先序遍历和中序遍历的特性来确定每个子树的节点范围。


### 复杂度分析
- 时间复杂度：O(n)，其中 n 是树的节点数。每个节点最多被访问一次。
- 空间复杂度：O(h)，其中 h 是树的高度。递归调用栈的空间复杂度。

### 总结
通过以上步骤，我们可以有效地根据先序和中序遍历构建出二叉树。