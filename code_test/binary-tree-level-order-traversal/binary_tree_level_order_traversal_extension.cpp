/**
 * @file binary_tree_level_order_traversal_extension.cpp
 * @brief 实现二叉树层序遍历的扩展解法和变体
 */

#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <algorithm>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution {
public:
    /**
     * @brief 二叉树的层序遍历 - DFS实现
     */
    std::vector<std::vector<int>> levelOrderDFS(TreeNode* root) {
        std::vector<std::vector<int>> result;
        dfs(root, 0, result);
        return result;
    }

    /**
     * @brief 二叉树的锯齿形层序遍历
     */
    std::vector<std::vector<int>> zigzagLevelOrder(TreeNode* root) {
        std::vector<std::vector<int>> result;
        if (!root) return result;
        
        std::queue<TreeNode*> q;
        q.push(root);
        bool isLeftToRight = true;
        
        while (!q.empty()) {
            int levelSize = q.size();
            std::vector<int> currentLevel;
            
            for (int i = 0; i < levelSize; i++) {
                TreeNode* node = q.front();
                q.pop();
                currentLevel.push_back(node->val);
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
            if (isLeftToRight == false)
            {
                std::reverse(currentLevel.begin(), currentLevel.end());
            }
            result.emplace_back(std::move(currentLevel));
            isLeftToRight = !isLeftToRight;
        }
        
        return result;
    }

    /**
     * @brief 二叉树的右视图
     */
    std::vector<int> rightSideView(TreeNode* root) {
        std::vector<int> result;
        if (!root) return result;
        
        std::queue<TreeNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            int levelSize = q.size();
            
            for (int i = 0; i < levelSize; i++) {
                TreeNode* node = q.front();
                q.pop();
                
                // 只添加每层的最后一个节点
                if (i == levelSize - 1) {
                    result.push_back(node->val);
                }
                
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
        }
        
        return result;
    }

    /**
     * @brief 打印二叉树结构（带有更多视觉效果）
     * @param root 二叉树根节点
     */
    void prettyPrintTree(TreeNode* root) {
        if (!root) {
            std::cout << "Empty tree" << std::endl;
            return;
        }

        std::vector<std::string> lines;
        prettyPrintTreeHelper(root, 0, "", "", lines);
        
        for (const auto& line : lines) {
            std::cout << line << std::endl;
        }
    }

private:
    void dfs(TreeNode* node, int level, std::vector<std::vector<int>>& result) {
        if (!node) return;
        if (result.size() == level)
        {
            result.push_back(std::vector<int>());
        }
        result[level].push_back(node->val);
        dfs(node->left, level + 1, result);
        dfs(node->right, level + 1, result);
    }

    /**
     * @brief 辅助函数：生成树的可视化字符串
     */
    void prettyPrintTreeHelper(TreeNode* node, int level, std::string prefix, 
                             std::string childrenPrefix, std::vector<std::string>& lines) {
        if (!node) return;

        std::string line = prefix + std::to_string(node->val);
        lines.push_back(line);

        if (node->left && node->right) {
            prettyPrintTreeHelper(node->left, level + 1, 
                                childrenPrefix + "├── ", 
                                childrenPrefix + "│   ", lines);
            prettyPrintTreeHelper(node->right, level + 1, 
                                childrenPrefix + "└── ", 
                                childrenPrefix + "    ", lines);
        }
        else if (node->left) {
            prettyPrintTreeHelper(node->left, level + 1, 
                                childrenPrefix + "└── ", 
                                childrenPrefix + "    ", lines);
        }
        else if (node->right) {
            prettyPrintTreeHelper(node->right, level + 1, 
                                childrenPrefix + "└── ", 
                                childrenPrefix + "    ", lines);
        }
    }
};

class TestFramework {
private:
    Solution solution;
    int passed = 0;
    int total = 0;

    TreeNode* createTree(const std::vector<int>& values, int index = 0) {
        if (index >= values.size() || values[index] == -1) return nullptr;
        
        TreeNode* root = new TreeNode(values[index]);
        root->left = createTree(values, 2 * index + 1);
        root->right = createTree(values, 2 * index + 2);
        return root;
    }

    void deleteTree(TreeNode* root) {
        if (!root) return;
        deleteTree(root->left);
        deleteTree(root->right);
        delete root;
    }

    void runTest(const std::string& testName, std::function<bool()> test) {
        total++;
        if (test()) {
            std::cout << "通过: " << testName << std::endl;
            passed++;
        } else {
            std::cout << "失败: " << testName << std::endl;
        }
    }

public:
    void runAllTests() {
        // DFS实现测试
        runTest("DFS层序遍历测试", [this]() {
            TreeNode* root = createTree({3,9,20,-1,-1,15,7});
            std::vector<std::vector<int>> result = solution.levelOrderDFS(root);
            bool success = (result == std::vector<std::vector<int>>{{3}, {9,20}, {15,7}});
            deleteTree(root);
            return success;
        });

        // 锯齿形层序遍历测试
        runTest("锯齿形层序遍历测试", [this]() {
            TreeNode* root = createTree({3,9,20,-1,-1,15,7});
            std::vector<std::vector<int>> result = solution.zigzagLevelOrder(root);
            solution.prettyPrintTree(root);
            bool success = (result == std::vector<std::vector<int>>{{3}, {20,9}, {15,7}});
            deleteTree(root);
            return success;
        });

        // 右视图测试
        runTest("右视图测试", [this]() {
            TreeNode* root = createTree({1,2,3,-1,5,-1,4});
            std::vector<int> result = solution.rightSideView(root);
            bool success = (result == std::vector<int>{1,3,4});
            deleteTree(root);
            return success;
        });

        // 添加树形打印测试
        // std::cout << "\n美化打印树形结构测试：" << std::endl;
        
        // TreeNode* testTree1 = createTree({3,9,20,-1,-1,15,7});
        // std::cout << "Test Tree 1:" << std::endl;
        // solution.prettyPrintTree(testTree1);
        // deleteTree(testTree1);

        // TreeNode* testTree2 = createTree({1,2,3,4,5,6,7});
        // std::cout << "\nTest Tree 2:" << std::endl;
        // solution.prettyPrintTree(testTree2);
        // deleteTree(testTree2);

        std::cout << "\n测试结果: " << passed << " 通过, " 
                  << (total - passed) << " 失败, " 
                  << total << " 总计" << std::endl;
    }
};

int main() {
    TestFramework testFramework;
    testFramework.runAllTests();
    return 0;
}
