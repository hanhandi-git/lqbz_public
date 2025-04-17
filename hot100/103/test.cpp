/**
 * @file binary_tree_zigzag_level_order_traversal.cpp
 * @brief 实现二叉树锯齿形层序遍历
 */

#include <iostream>
#include <vector>
#include <queue>
#include <deque>
#include <algorithm>
#include <functional>

// 二叉树节点定义
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
     * @brief 二叉树的锯齿形层序遍历 - 方法1：使用vector预分配
     * @param root 二叉树根节点
     * @return 锯齿形层序遍历结果
     */
    std::vector<std::vector<int>> zigzagLevelOrder(TreeNode* root) 
    {
        std::vector<std::vector<int>> res;
        if (!root) return res;
        std::queue<TreeNode*> que;
        que.push(root);
        bool iszigzag = false;
        while (!que.empty())
        {
            int size = que.size();
            std::vector<int> tmp(size, 0);
            for (int i = 0; i < size; i++)
            {
                auto node = que.front();
                que.pop();
                if (!iszigzag) {
                    tmp[i] = node->val;
                } else {
                    tmp[size - i - 1] = node->val;
                }
                if (node->left) que.push(node->left);
                if (node->right) que.push(node->right);
            }
            res.emplace_back(std::move(tmp));
            iszigzag = !iszigzag;
        }
        return res;
    }
    // std::vector<std::vector<int>> zigzagLevelOrder(TreeNode* root) {
    //     std::vector<std::vector<int>> res;
    //     if (!root) return res;
        
    //     std::queue<TreeNode*> que;
    //     que.push(root);
    //     bool leftToRight = true;
        
    //     while (!que.empty()) {
    //         int size = que.size();
    //         std::vector<int> currentLevel(size);  // 预分配大小
            
    //         for (int i = 0; i < size; i++) {
    //             auto node = que.front();
    //             que.pop();
                
    //             // 根据方向决定位置
    //             int index = leftToRight ? i : (size - 1 - i);
    //             currentLevel[index] = node->val;
                
    //             if (node->left) que.push(node->left);
    //             if (node->right) que.push(node->right);
    //         }
            
    //         res.push_back(std::move(currentLevel));
    //         leftToRight = !leftToRight;
    //     }
        
    //     return res;
    // }

    /**
     * @brief 二叉树的锯齿形层序遍历 - 方法2：使用双端队列
     * @param root 二叉树根节点
     * @return 锯齿形层序遍历结果
     */
    std::vector<std::vector<int>> zigzagLevelOrderDeque(TreeNode* root) {
        std::vector<std::vector<int>> res;
        if (!root) return res;
        
        std::queue<TreeNode*> que;
        que.push(root);
        bool leftToRight = true;
        
        while (!que.empty()) {
            int size = que.size();
            std::deque<int> currentLevel;
            
            for (int i = 0; i < size; i++) {
                auto node = que.front();
                que.pop();
                
                if (leftToRight) {
                    currentLevel.push_back(node->val);
                } else {
                    currentLevel.push_front(node->val);
                }
                
                if (node->left) que.push(node->left);
                if (node->right) que.push(node->right);
            }
            
            res.emplace_back(std::vector<int>(currentLevel.begin(), currentLevel.end()));
            leftToRight = !leftToRight;
        }
        
        return res;
    }

    /**
     * @brief 打印二叉树结构
     * @param root 二叉树根节点
     */
    void printTree(TreeNode* root) {
        if (!root) {
            std::cout << "Empty tree" << std::endl;
            return;
        }

        std::queue<TreeNode*> q;
        q.push(root);
        int level = 0;
        int height = getTreeHeight(root);
        
        while (!q.empty() && level < height) {
            int levelSize = q.size();
            
            // 打印当前层的缩进
            printSpaces(std::pow(2, height - level - 1) - 1);
            
            for (int i = 0; i < levelSize; i++) {
                TreeNode* node = q.front();
                q.pop();
                
                if (node) {
                    std::cout << node->val;
                    q.push(node->left);
                    q.push(node->right);
                } else {
                    std::cout << " ";
                    q.push(nullptr);
                    q.push(nullptr);
                }
                
                // 打印节点之间的空格
                printSpaces(std::pow(2, height - level) - 1);
            }
            std::cout << std::endl;
            level++;
        }
    }

private:
    /**
     * @brief 获取二叉树的高度
     */
    int getTreeHeight(TreeNode* root) {
        if (!root) return 0;
        return 1 + std::max(getTreeHeight(root->left), getTreeHeight(root->right));
    }

    /**
     * @brief 打印指定数量的空格
     */
    void printSpaces(int count) {
        for (int i = 0; i < count; i++) {
            std::cout << " ";
        }
    }
};

class TestFramework {
private:
    Solution solution;
    int passed = 0;
    int total = 0;

    // 辅助函数：创建测试用的二叉树
    TreeNode* createTree(const std::vector<int>& values, int index = 0) {
        if (index >= values.size() || values[index] == -1) return nullptr;
        
        TreeNode* root = new TreeNode(values[index]);
        root->left = createTree(values, 2 * index + 1);
        root->right = createTree(values, 2 * index + 2);
        return root;
    }

    // 辅助函数：清理二叉树内存
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
        // 测试用例1：标准二叉树
        runTest("锯齿形层序遍历测试1", [this]() {
            TreeNode* root = createTree({3,9,20,-1,-1,15,7});
            std::vector<std::vector<int>> result = solution.zigzagLevelOrder(root);
            bool success = (result == std::vector<std::vector<int>>{{3}, {20,9}, {15,7}});
            deleteTree(root);
            return success;
        });

        // 测试用例2：单节点树
        runTest("锯齿形层序遍历测试2", [this]() {
            TreeNode* root = new TreeNode(1);
            std::vector<std::vector<int>> result = solution.zigzagLevelOrder(root);
            bool success = (result == std::vector<std::vector<int>>{{1}});
            delete root;
            return success;
        });

        // 测试用例3：空树
        runTest("锯齿形层序遍历测试3", [this]() {
            std::vector<std::vector<int>> result = solution.zigzagLevelOrder(nullptr);
            return result.empty();
        });

        // 测试用例4：完全二叉树
        runTest("锯齿形层序遍历测试4", [this]() {
            TreeNode* root = createTree({1,2,3,4,5,6,7});
            std::vector<std::vector<int>> result = solution.zigzagLevelOrder(root);
            bool success = (result == std::vector<std::vector<int>>{{1}, {3,2}, {4,5,6,7}});
            deleteTree(root);
            return success;
        });

        // 测试双端队列实现
        runTest("双端队列实现测试", [this]() {
            TreeNode* root = createTree({1,2,3,4,5,6,7});
            std::vector<std::vector<int>> result1 = solution.zigzagLevelOrder(root);
            std::vector<std::vector<int>> result2 = solution.zigzagLevelOrderDeque(root);
            bool success = (result1 == result2);
            deleteTree(root);
            return success;
        });

        // 添加树形打印测试
        std::cout << "\n打印树形结构测试：" << std::endl;
        
        TreeNode* testTree = createTree({3,9,20,-1,-1,15,7});
        std::cout << "Test Tree (锯齿形遍历结果应为: [3], [20,9], [15,7])" << std::endl;
        solution.printTree(testTree);
        deleteTree(testTree);

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
